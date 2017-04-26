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
#include "IconAssigner.hpp"
#include <sys/stat.h>
#include <sstream>
#include <stdafx.h>
#include <fstream>
#include <iostream>
#include "ImageWorker.hpp"
#include <algorithm>
#include <shlobj.h>
#include <shellapi.h>
#include <wx/image.h>

namespace FolderIconAssigner
{
	unsigned int IconAssigner::uuid = 0;
	const std::string IconAssigner::cDESKTOP_INI_NAME = "desktop.ini";
	//--------------------------------------------------------------------------------------------------
	IconAssigner::IconAssigner()
	{
		m_resampleAll = false; //by default, don't resample all
		m_containerIconIndex = 0;
		m_folderPath = "";
		m_imagePath = "";
		m_recursive = false;
		m_removeIcoAlso = true;
		m_copyToFolder = true;
		m_rootIconPath = "";
		m_rootRef = false;
		m_icon_names_unique = false; //All icons use same name by default
		m_bitMapIcon = false; //Never save as bitmap, always as icon.
		genNewName();
	}
	//--------------------------------------------------------------------------------------------------
	IconAssigner::~IconAssigner()
	{

	}
	//--------------------------------------------------------------------------------------------------
	void IconAssigner::genNewName()
	{
		if (m_icon_names_unique)
		{
			std::stringstream strmName;
			strmName << "." << DIR_SLASH << "FIA3_";
			strmName << uuid++;
			strmName << ".ico";
			m_IconUniqueName = strmName.str();
		}
		else
		{
			m_IconUniqueName = "FIA3.ico";
		}
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::validateData(std::string & errMSg)
	{
		if (!FileExists(m_imagePath))
		{
			errMSg = "Image does not exit or cannot be read: \n" + m_imagePath;
			return false;
		}

		if (!FolderExists(m_folderPath))
		{
			errMSg = "Folder does not exit: \n" + m_folderPath;
			return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::installIcons(std::string & errMSg)
	{
		bool succeeded = true;
		genNewName(); //Make new UUID name.

		succeeded = prepIcon(errMSg); //Prepare the root icon, resample if needed and copy it to the root folder.

		if (!prepFolder(m_folderPath))
		{
			errMSg = "Could not install icon to the root folder!";
			succeeded = false;
		}

		unsigned int dirsProcessed = 0;
		if (m_recursive)
			dirsProcessed = applySubdirsIcons();
		
		if (dirsProcessed == 0 && m_recursive)
		{
			errMSg = "No subdirectories were processed!";
			succeeded = false;
		}
		
		return succeeded;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::prepFolder(std::string & folderPath, bool isSubdir)
	{
		bool succeeded = true;
		std::string errMsg;
		std::string desktopIniPath = folderPath + DIR_SLASH + cDESKTOP_INI_NAME;
		if (!isSubdir || !m_copyToFolder)
		{
			std::string iconName = m_IconUniqueName;
			if (!m_copyToFolder)
				iconName = m_imagePath;
			std::string desktopINI = generateDesktopIniFile(iconName, m_containerIconIndex);
			succeeded =  writeTextFile(desktopIniPath, desktopINI) && setMyFileAttributes(desktopIniPath);

			//Since we are not subdir, no need to copy icon, it's allready there!
		}
		else
		{
			if (!m_rootRef) //every icon must have it's own icon file?
			{
				std::string subIcon = folderPath + DIR_SLASH + m_IconUniqueName;
				succeeded = copyFile(m_rootIconPath, subIcon, errMsg) && setMyFileAttributes(subIcon);
				if (succeeded)
				{
					std::string desktopINI = generateDesktopIniFile(m_IconUniqueName, m_containerIconIndex);
					succeeded = writeTextFile(desktopIniPath,desktopINI) && setMyFileAttributes(desktopIniPath);

				}
			}
			else //we reference the root icon
			{
				int backjumps = (slashCount(folderPath.c_str()) - slashCount(m_folderPath.c_str()));
				std::string icoRelPath = "";
				for (int i = 0; i < backjumps; i++)
					icoRelPath += ".." + std::string(DIR_SLASH);

				icoRelPath += m_IconUniqueName; 
				std::string desktopINI = generateDesktopIniFile(icoRelPath, m_containerIconIndex);
				succeeded = writeTextFile(desktopIniPath,desktopINI)&& setMyFileAttributes(desktopIniPath);
			}
		}
		//Last, set directory flags for folders
		setMyFileAttributes(folderPath, false, false, true);
		return succeeded;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::prepIcon(std::string & errMsg)
	{
		bool succeeded = false;
		bool resample = true; //resample by default unless extention is ico
		bool libExtract = false; //don't extract out of lib unless we are a lib.
		m_rootIconPath = m_folderPath + DIR_SLASH + m_IconUniqueName;

		if (!m_copyToFolder) //Ignore if we don't copy
		{
			m_rootIconPath = m_imagePath;
			succeeded = true;
			resample = false;
			//When we don't do any copy or extracting.
			return succeeded; //This is the only time we drop out and return true!! 
		}

		//Check file extention
		std::string fileext = "";
		if(m_imagePath.find_last_of(".") != std::string::npos)
			fileext = m_imagePath.substr(m_imagePath.find_last_of(".")+1);

		#ifndef APPLICATION_PREFER_WINAPI
		for (unsigned int i=0;i< fileext.length();i++) //make lowercase
			if (fileext[i] >= 0x41 && fileext[i] <= 0x5A)
				fileext[i] = fileext[i] + 0x20;
		#else
		std::transform(fileext.begin(),fileext.end(),fileext.begin(),tolower); //also make lowercase
		#endif

		//Copy icon if it is and icon image.
		if (m_copyToFolder && (!m_resampleAll && fileext.compare("ico") == 0 )) //Only if we copy and file ext match
		{
			resample = false; //no resample, just copy icon.
			succeeded = copyFile(m_imagePath,m_rootIconPath,errMsg) && setMyFileAttributes(m_rootIconPath);
		}
		else if (fileext.compare("exe") == 0 )
			libExtract = true;
		else if (fileext.compare("dll") == 0 )
			libExtract = true;
		else if (fileext.compare("icl") == 0 )
			libExtract = true;

		//Resample if not icon image or overrided.
		if (!libExtract)
		{
			if (resample || m_resampleAll) //Will sample by default unless not copy.
			{
				if (FileExists(m_rootIconPath))
				{
					DeleteFileA(m_rootIconPath.c_str());
				}

				succeeded = ImageWorker::getInstance().imageToIcon(m_imagePath, m_rootIconPath, 256);
				if (!succeeded)
					errMsg = ImageWorker::getInstance().getAllErrorsAndDump();
				setMyFileAttributes(m_rootIconPath);
			}
		}
		else
		{
			if (FileExists(m_rootIconPath))
			{
				DeleteFileA(m_rootIconPath.c_str());
			}
			succeeded = this->extractIcon(m_imagePath, m_containerIconIndex, m_rootIconPath);
			if (succeeded)
				m_containerIconIndex = 0; //reset to 0 because we already extracted the icon 
											// and in the desktop.ini it must be 0
		}
		
		return succeeded;
	}
	//--------------------------------------------------------------------------------------------------
	std::string IconAssigner::generateDesktopIniFile(const std::string & iconpath, 
		const int & index, const std::string & infotip, const std::string & alias)
	{
		std::stringstream stream;
		stream << "#File generated by Folder Icon Assigner 3\n";
		stream << "[.ShellClassInfo]\n";
		stream << "IconResource=" << iconpath << "," << index << "\n";
		stream << "IconFile=" << iconpath << "\n";
		stream << "IconIndex=" << index << "\n";
		//stream << "InfoTip=" << infotip << "\n";
		//stream << "LocalizedResourceName=" << alias << "\n";
		//stream << "#File generated by Folder Icon Assigner 3\n";
		//stream << "#Anything else does not belong to folder icon assigner";
		//stream << "\n";
		return stream.str();
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::generatePreviewIcon(const std::string & path, std::string errMsg)
	{
		bool succeeded = false;
		bool resample = true; //resample by default unless extention is ico
		bool libExtract = false; //don't extract out of lib unless we are a lib.
		//Check file extention
		std::string fileext = "";
		if(m_imagePath.find_last_of(".") != std::string::npos)
			fileext = m_imagePath.substr(m_imagePath.find_last_of(".")+1);

		#ifndef APPLICATION_PREFER_WINAPI
				for (unsigned int i=0;i< fileext.length();i++) //make lowercase
					if (fileext[i] >= 0x41 && fileext[i] <= 0x5A)
						fileext[i] = fileext[i] + 0x20;
		#else
				std::transform(fileext.begin(),fileext.end(),fileext.begin(),tolower); //also make lowercase
		#endif

		//Copy icon if it is and icon image.
		if (fileext.compare("ico") == 0 ) 
		{
			resample = false; //no resample, just copy icon.
			succeeded = copyFile(m_imagePath,path,errMsg);
		}
		else if (fileext.compare("exe") == 0 )
			libExtract = true;
		else if (fileext.compare("dll") == 0 )
			libExtract = true;
		else if (fileext.compare("icl") == 0 )
			libExtract = true;
	
		//Resample if not icon image or overridden.
		if (resample && !libExtract) //Will sample by default unless not copy.
		{
			if (FileExists(path))
			{
				DeleteFileA(path.c_str());
			}

			succeeded = ImageWorker::getInstance().imageToIcon(m_imagePath, path);
			if (!succeeded)
				errMsg = ImageWorker::getInstance().getAllErrorsAndDump();
		}
		else if (libExtract)
		{
			if (FileExists(path))
			{
				DeleteFileA(path.c_str());
			}
			succeeded = this->extractIcon(m_imagePath, m_containerIconIndex, path);
		}

		return succeeded;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::FileExists(const std::string & strFilename) 
	{
		struct stat stFileInfo;
		int intStat;

		intStat = stat(strFilename.c_str(),&stFileInfo); //Lets try and read

		if(intStat == 0) //Can we read the info...
			return true;
		 else 
			return false;

	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::FolderExists(const std::string & strFolderName)
	{   
		return FileExists(strFolderName);
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::copyFile(const std::string & src, const std::string & dest, std::string & errMsg)
	{
		bool succeeded = true;
		if (FileExists(dest))
			DeleteFileA(dest.c_str());
	#ifndef APPLICATION_PREFER_WINAPI
		try
		{
			std::ifstream infs(src.c_str(), std::ios::binary);
			std::ofstream outfs(dest.c_str(), std::ios::binary);

			outfs << infs.rdbuf();
			outfs.close();
			succeeded = true;
		}
		catch (std::exception * ex)
		{
			errMsg = ex->what();
			succeeded = false;
		}
	#else
		succeeded = (bool)CopyFileA(src.c_str(), dest.c_str(), false);
		succeeded = true; // above this for whatever weird reason returns false when it shouldn't.
	#endif

		return succeeded;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::writeTextFile(const std::string & file, const std::string & lines)
	{
		if (FileExists(file))
			DeleteFileA(file.c_str());

		std::ofstream filestream (file.c_str());
		if (filestream.is_open())
		{
			/*std::vector<std::string>::const_iterator iter;
			for ( iter = lines.begin(); iter != lines.end(); iter++)
			{
				filestream << *iter << "\n";
			}*/
			filestream << lines;
			filestream.close();
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------
	bool IconAssigner::setMyFileAttributes(const std::string & file, bool readonly,bool visible, bool dir)
	{
		bool result = false;
		DWORD attr = 0;

		if (!visible)
			attr += FILE_ATTRIBUTE_HIDDEN + FILE_ATTRIBUTE_SYSTEM;
		else
			attr = FILE_ATTRIBUTE_NORMAL;

		if (readonly)
			attr += FILE_ATTRIBUTE_READONLY;

		if (dir)
			attr = FILE_ATTRIBUTE_READONLY; //FILE_ATTRIBUTE_DIRECTORY + 
			//The special flag is readonly, without it windows does not show icons for the folder.

		return (bool)SetFileAttributesA(file.c_str(),  attr);
	}
	//--------------------------------------------------------------------------------------------------
	unsigned int IconAssigner::applySubdirsIcons()
	{
		unsigned int dirCount = 0;
		wxDir dir(m_folderPath);
		if ( !dir.IsOpened() )
			return 0; //Fail

		wxString filename;
		wxDirRecorder dirPaths(m_folderPath, m_folderDepth);
		dir.Traverse(dirPaths);
		std::vector<std::string> list = dirPaths.getPaths();

		//Loop through paths and install their icons.
		std::vector<std::string>::iterator iter;
		for (iter = list.begin(); iter != list.end(); iter++)
		{
			prepFolder(*iter, true);
			dirCount++;
		}
		return dirCount;
	}
	//--------------------------------------------------------------------------------------------------
	std::string IconAssigner::getRelativePath(const std::string & parentPath, const std::string & fullpath)
	{
		if (slashCount(fullpath.c_str()) < slashCount(parentPath.c_str()) )
			return "";

		size_t pos = 0;
		pos = fullpath.find(parentPath);
		if (pos != std::string::npos)
		{
			return fullpath.substr(parentPath.length(), fullpath.length()-pos);
		}

		return "";
	}
	//--------------------------------------------------------------------------------------------------
	std::string IconAssigner::getAppDataFolder()
	{
		char szAppDPath[MAX_PATH];
		std::string fullpath;
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT,  szAppDPath);
		fullpath = std::string(szAppDPath) + std::string(DIR_SLASH) + std::string(APPLICATION_DATAFOLDERNAME);

		if (!FolderExists(fullpath))
			CreateDirectoryA(fullpath.c_str(), NULL);

		return fullpath;
	}
	//-------------------------------------------------------------------------------------------------- 
	bool IconAssigner::extractIcon(const std::string & containerPath, const int & index ,const std::string & saveIconPath)
	{
		bool succeeded = false;
		HICON hIconLarge, hIconSmall;
		ICONINFO oIconInfoL,oIconInfoS;
		ExtractIconExA(containerPath.c_str(), index, &hIconLarge, &hIconSmall, 1);
		
		
		if (hIconLarge  && GetIconInfo(hIconLarge, &oIconInfoL))
		{
			succeeded = ImageWorker::getInstance().writeHBITMAP(saveIconPath, oIconInfoL.hbmColor, true);
		}
		else if (hIconSmall  && GetIconInfo(hIconSmall, &oIconInfoS))
		{
			succeeded = ImageWorker::getInstance().writeHBITMAP(saveIconPath, oIconInfoS.hbmColor, true);
		}

		DestroyIcon(hIconLarge);
		DestroyIcon(hIconSmall);

		return succeeded;
	}
	//-------------------------------------------------------------------------------------------------- 
	bool IconAssigner::fixFolderIcons(std::string & errMSg)
	{
		if (!FolderExists(m_folderPath))
		{
			errMSg = "Folder does not exist: \n" + m_folderPath;
			return false;
		}
		std::string desktopIniPath = m_folderPath + DIR_SLASH + cDESKTOP_INI_NAME;
		if (!FileExists(desktopIniPath))
		{
			errMSg = "desktop.ini does not exist: \n" + desktopIniPath;
			return false;
		}

		setMyFileAttributes(desktopIniPath);
		setMyFileAttributes(m_folderPath, false, false, true);

		//Sub Dir Icons
		if(m_recursive)
		{
			unsigned int dirCount = 0;
			wxDir dir(m_folderPath);
			if ( !dir.IsOpened() )
			{
				errMSg = "Could not access subdirectories!: \n" + m_folderPath;
				return false; //Fail
			}
			wxString filename;
			wxDirRecorder dirPaths(m_folderPath, m_folderDepth);
			dir.Traverse(dirPaths);
			std::vector<std::string> list = dirPaths.getPaths();
			//Loop through paths and install their icons.
			std::vector<std::string>::iterator iter;
			for (iter = list.begin(); iter != list.end(); iter++)
			{
				std::string iniPath = *iter + DIR_SLASH + cDESKTOP_INI_NAME;
				if (FileExists(iniPath))
				{
					setMyFileAttributes(iniPath);
				}
				setMyFileAttributes(*iter, false, false, true);
				dirCount++;
			}
		}

		errMSg = "Succeeded!";
	
		return true;
	}
	//-------------------------------------------------------------------------------------------------- 
	bool IconAssigner::cleanFolderIcons(std::string & errMSg)
	{
		if (!FolderExists(m_folderPath))
		{
			errMSg = "Folder does not exist: \n" + m_folderPath;
			return false;
		}
		std::string desktopIniPath = m_folderPath + DIR_SLASH + cDESKTOP_INI_NAME;
		if (!FileExists(desktopIniPath))
		{
			errMSg = "desktop.ini does not exist: \n" + desktopIniPath;
			return false;
		}

		DeleteFileA(desktopIniPath.c_str());
		std::string icoPath = m_folderPath + DIR_SLASH + m_IconUniqueName;
		if (m_removeIcoAlso && FileExists(icoPath))
		{
			DeleteFileA(icoPath.c_str());
		}

		//Sub Dir Icons
		if(m_recursive)
		{
			unsigned int dirCount = 0;
			wxDir dir(m_folderPath);
			if ( !dir.IsOpened() )
			{
				errMSg = "Could not access subdirectories!: \n" + m_folderPath;
				return false; //Fail
			}
			wxString filename;
			wxDirRecorder dirPaths(m_folderPath, m_folderDepth);
			dir.Traverse(dirPaths);
			std::vector<std::string> list = dirPaths.getPaths();
			//Loop through paths and install their icons.
			std::vector<std::string>::iterator iter;
			for (iter = list.begin(); iter != list.end(); iter++)
			{
				std::string iniPath = *iter + DIR_SLASH + cDESKTOP_INI_NAME;
				if (FileExists(iniPath))
				{
					DeleteFileA(iniPath.c_str());
				}
				std::string icoPath = m_folderPath + DIR_SLASH + m_IconUniqueName;
				if (m_removeIcoAlso && FileExists(icoPath))
				{
					DeleteFileA(icoPath.c_str());
				}
				dirCount++;
			}
		}

		errMSg = "";
	
		return true;
	}
	//-------------------------------------------------------------------------------------------------- 

	//--------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------
};