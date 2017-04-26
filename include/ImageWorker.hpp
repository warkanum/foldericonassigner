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
#ifndef _IMAGEWORKER_H_
#define _IMAGEWORKER_H_
#include "PreReq.hpp"

#include <string>
#include <stack>
#include <stdafx.h>

#ifdef APPLICATION_USE_FREEIMAGE //Use freeimage instead of wxwigets
#include <FreeImage.h>
#else
#include <wx/image.h> 
#endif

namespace FolderIconAssigner
{
	/************************************************************************/
	/* Class that converts images to icons using freeimage                  */
	/************************************************************************/
	class ImageWorker
	{
	public:
		///Get the operator class instance as pointer, if not exist, create it.
		static ImageWorker & getInstance()
		{
			static ImageWorker instance;

			return instance;
		}



		/*
			Convert any supported image to and icon file.
			@param infile The source image file.
			@param iconfile The icon file to be saved.
			@param res The resolution to scale the image 2. 256 by 256 is the max.
		*/
		bool imageToIcon(const std::string & infile, const std::string & iconfile, unsigned short res = 256, bool bitmap = false);

		bool writeHBITMAP(const std::string & filename, HBITMAP hbmp, bool asIcon);

		std::string getLastError() { return m_ErrorStack.top();};
		std::string getAllErrorsAndDump();
		
		std::stack<std::string> & getErrorStack() { return m_ErrorStack;};
	protected:
		///Create a new ImageWorker class
		ImageWorker();
		///Destructor
		~ImageWorker();

		#ifdef APPLICATION_USE_FREEIMAGE
			FIBITMAP * m_pImage;
		#else
			wxImage * m_pImage;
		#endif
		std::stack<std::string> m_ErrorStack;

	};

};

#endif