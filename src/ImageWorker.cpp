/*
-----------------------------------------------------------------------------
This source file is part of Folder Icon Assigner 3
Created by Warkanum (H.Puth)

Copyright (c) 2010-2011 Alienate Effect

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "ImageWorker.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>


namespace FolderIconAssigner
{

#ifdef APPLICATION_USE_FREEIMAGE
	#define LOG_FREEIMAGE "freeimage.log"
	/**
	FreeImage error handler
	@param fif Format / Plugin responsible for the error
	@param message Error message
	*/
	void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
	{
		try
		{

			std::string errorMsg = "";
			printf("\n*** ");
			errorMsg += "** ";

			if(fif != FIF_UNKNOWN) 
			{
				printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
				errorMsg += "** " +  std::string(FreeImage_GetFormatFromFIF(fif)) + " format" ;
			}

			printf(message);
			errorMsg += "\n" + std::string(message);
			printf(" ***\n");
			errorMsg += " **";

			ImageWorker::getInstance().getErrorStack().push(errorMsg);
		}
		catch (std::exception * ex)
		{
			printf("Exception while logging FreeImage error: %s", ex->what());
		}

	}
#endif

	ImageWorker::ImageWorker()
	{
		#ifdef APPLICATION_USE_FREEIMAGE
			FreeImage_Initialise(false);
			FreeImage_SetOutputMessage(FreeImageErrorHandler);
			NIX(m_pImage);
		#endif
	
	}

	ImageWorker::~ImageWorker()
	{
		#ifdef APPLICATION_USE_FREEIMAGE
			if (ISNNIX(m_pImage)) //unload image if loaded
			{
				FreeImage_Unload(m_pImage);
				NIX(m_pImage);
			}

			FreeImage_DeInitialise();
		#endif
	}

	bool ImageWorker::imageToIcon(const std::string & infile, const std::string & iconfile, unsigned short res, bool bitmap)
	{
		//if bitmap is true, we create a bitmap image instead of icon.
		bool succeeded = false;

		#ifdef APPLICATION_USE_FREEIMAGE

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(infile.c_str());

		m_pImage = FreeImage_Load(format, infile.c_str()); //try and load an image
		if (m_pImage) 
		{
			if (!bitmap){if (res > 128) {res = 128;};} //The limit is 127
			else {if (res > 256) {res = 256;};} //No limit but make it 256

			FIBITMAP * pScaledImage = FreeImage_Rescale(m_pImage, res, res, FILTER_BICUBIC);
			
			if (pScaledImage)//(pScaledImage)
			{
				if (!bitmap)
				{
					if (FreeImage_Save(FIF_ICO, pScaledImage, iconfile.c_str(), ICO_MAKEALPHA)) 
						succeeded = true;
				}
				else
				{
					if (FreeImage_Save(FIF_BMP, pScaledImage, iconfile.c_str(), BMP_DEFAULT)) 
						succeeded = true;
				}
				// Now lets free up resources.
				FreeImage_Unload(pScaledImage);
				NIX(pScaledImage);
			}
			else
			{
				if (!bitmap)
				{
					if (FreeImage_Save(FIF_ICO, m_pImage, iconfile.c_str(), ICO_MAKEALPHA)) 
						succeeded = true;
				}
				else
				{
					if (FreeImage_Save(FIF_BMP, m_pImage, iconfile.c_str(), BMP_DEFAULT)) 
						succeeded = true;
				}
			}
			
			// Now lets free up resources.
			FreeImage_Unload(m_pImage);
			NIX(m_pImage);
		}
		else
		{
			m_ErrorStack.push("Freeimage could not load the file! Try another file or filetype.");
		}

		#else //Using Freeimage
		//Using wxwigets

		if (!bitmap)
			if (res > 127) {res = 127;}; //Nope this is the limit, as far as I know.
		else
			if (res > 256) {res = 256;}; //No limit but make it 256

		wxInitAllImageHandlers();
		m_pImage = new wxImage();
		m_pImage->LoadFile(infile);

		if (m_pImage && m_pImage->IsOk()) 
		{
			wxImage & newImage = m_pImage->ResampleBicubic(res, res);
			if (!bitmap)
				succeeded = newImage.SaveFile(iconfile,wxBITMAP_TYPE_ICO);
			else
				succeeded = newImage.SaveFile(iconfile,wxBITMAP_TYPE_BMP);

			newImage.Destroy();
		}
		else
		{
			m_ErrorStack.push("WxWigets could not load the file! Try another file or filetype.");
		}

		m_pImage->Destroy();
		SDELETE(m_pImage);

		#endif

		return succeeded;
	}

	std::string ImageWorker::getAllErrorsAndDump()
	{
		std::string errors = "";
		while (!m_ErrorStack.empty())
		{
			errors += m_ErrorStack.top() + "\n";
			m_ErrorStack.pop();
		}

		return errors;
	}

	///To implement: Alpha mask for transparency
	bool ImageWorker::writeHBITMAP(const std::string & filename, HBITMAP hbmp, bool asIcon)
	{
		bool succeeded = false;
		// the following code assumes that you have a valid HBITMAP loaded into the memory
		FIBITMAP *dib = NULL;
		if(hbmp) 
		{
			BITMAP bm;
			GetObject(hbmp, sizeof(BITMAP), (LPSTR) &bm);
			dib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
			// The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
			// So we save these infos below. This is needed for palettized images only.
			int nColors = FreeImage_GetColorsUsed(dib);
			HDC dc = GetDC(NULL);
			int Success = GetDIBits(dc, hbmp, 0, FreeImage_GetHeight(dib),
				FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
			ReleaseDC(NULL, dc);
			// restore BITMAPINFO members
			FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
			FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;

			if (asIcon)
			{
				FIBITMAP * pScaledImage = FreeImage_Rescale(dib, 128, 128, FILTER_BICUBIC);
				if (FreeImage_Save(FIF_ICO, pScaledImage, filename.c_str(), ICO_MAKEALPHA)) 
				{
					succeeded = true;
					FreeImage_Unload(pScaledImage);
				}
			}
			else
			{
				if (FreeImage_Save(FIF_BMP, dib, filename.c_str())) 
					succeeded = true;
			}

		}

		FreeImage_Unload(dib);

		return succeeded;
	}

};
