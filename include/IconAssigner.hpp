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
#ifndef _ICONASSIGNER_H_
#define _ICONASSIGNER_H_

#include "PreReq.hpp"
#include <vector>
#include <string>
#include "wx/dir.h"


namespace FolderIconAssigner
{
	class ImageWorker;

	/************************************************************************/
	/* Manages Files and Images operations									*/
	/* Singleton Class														*/
	/************************************************************************/
	class IconAssigner
	{
	public:
		///Get the operator class instance as pointer, if not exist, create it.
		static IconAssigner * getInstancePtr()
		{
			return &getInstance();
		}

		///Get the operator class instance as reference, if not exist, create it.
		static IconAssigner & getInstance()
		{
			static IconAssigner instance;
			return instance;
		}

		///Count characters of type in string. Used for looking for / or \ chars
		static unsigned int slashCount(const char* s)
		{
			unsigned int n = 0;
			while(*s != '\0') 
			{
				if((*s == '\\') || (*s == '/'))
					n++;
				*s++;
			}
			return n;
		}

		///Return the relative path on parentpath. return = (fullpath - parentPath) 
		std::string getRelativePath(const std::string & parentPath, const std::string & fullpath);

		///Set the path for the folder we are working on.
		void setFolderPath(const std::string & path) { m_folderPath = path;};
		
		///Get the path of the folder we are working on.
		const std::string & getFolderPath() { return m_folderPath;};
		
		///Set the path for the image we are using.
		void setImagePath(const std::string & path) { m_imagePath = path;};
		
		///Get the path of the image we are using.
		const std::string & getImagePath() { return m_imagePath;};

		///Set Icon index
		void setContainerIndex(int index) { m_containerIconIndex = index; };

		///Get Icon index
		const int & getContainerIndex() { return m_containerIconIndex; };
		
		///Icon should be copied to the folder
		void setCopyToFolder(bool logic) { m_copyToFolder = logic; };

		///Should icon be copied to the folder
		bool getCopyToFolder() { return m_copyToFolder; };
		
		///Icon should be applied to the subfolder of this folder
		void setRecursive(bool logic) { m_recursive = logic; };

		///Should icon be applied to the subfolder of this folder
		bool getRecursive() { return m_recursive; };

		///Icon has a unique name 
		bool getUniqueIconNames() { return m_icon_names_unique; };
		
		///Set icon has unique names
		void setUniqueIconNames(bool on) { m_icon_names_unique = on; };

		///Must we resample all the icons or only pictures.
		bool getResampleAll() { return m_resampleAll; };

		///Must we resample all the icons or only pictures.
		void setResampleAll(bool on) { m_resampleAll = on; };

		///Subfolders should reference their root folder icon. (Only 1 root icon for tree)
		void setReference(bool logic) { m_rootRef = logic; };

		///Subfolders should reference their root folder icon.
		bool getReference() { return m_rootRef; };
		
		///Subfolder depth of the recursion, 0 means inifinite.
		void setFolderDepth(unsigned short depth) { m_folderDepth = depth;};

		///Get the subfolder depth of the recursion, 0 means inifinite.
		unsigned short getFolderDepth() { return m_folderDepth;};

		///Validate the input data and return true if valid else return false with error message.
		bool validateData(std::string & errMSg);
		
		///Install the icons and return true if succeeded else return false with error message.
		bool installIcons(std::string & errMSg);

		///Check if a file exits
		bool FileExists(const std::string & strFilename);

		///Check if a folder exits
		bool FolderExists(const std::string & strFolderName);
		
		///Get applications folder within APPDATA folder, will create one it we can.
		std::string getAppDataFolder();

		///Generate Preview Icon in the given path returns false if failed.
		bool generatePreviewIcon(const std::string & path, std::string errMsg);

		///Extract icon from executable file
		bool extractIcon(const std::string & containerPath, const int & index ,const std::string & saveIconPath);

		///Fixes folders with defined icons. Fix attributes and hides desktop.ini files
		bool fixFolderIcons(std::string & errMSg);

		///Removed all desktop.ini files from the folders.
		bool cleanFolderIcons(std::string & errMSg);

		static const std::string cDESKTOP_INI_NAME;
	protected:
		static unsigned int uuid;

		bool	m_copyToFolder;
		bool	m_recursive;
		bool	m_removeIcoAlso;
		bool	m_rootRef;
		bool	m_resampleAll;
		bool	m_icon_names_unique;
		bool	m_bitMapIcon;
		unsigned short m_folderDepth;
		int m_containerIconIndex;
		std::string m_IconUniqueName;
		std::string m_folderPath;
		std::string m_imagePath;
		std::string m_rootIconPath;


		///Generates the desktopIniFiles contents.
		std::string generateDesktopIniFile(const std::string & iconpath, const int & index, 
			const std::string & infotip = "", const std::string & alias = "");

		///Copy and icon file from source to destination. Returns false with errMsg if fail.
		bool copyFile(const std::string & src, const std::string & dest, std::string & errMsg);

		///Write text file to storage. @parm file filename @parm lines List of strings
		bool writeTextFile(const std::string & file, const std::string & lines);

		///Hides/shows a file depending on arguments. Hide sets it as system and hidden for desktop.ini
		bool setMyFileAttributes(const std::string & file, bool readonly = false,bool visible = false, bool dir = false);

		///Apply Icons the subdirs, returns the total dirs processed.
		unsigned int IconAssigner::applySubdirsIcons();

		///Prepare a folder with an icon. This copies the icon to the folder if specified and generates the desktop.ini
		bool prepFolder(std::string & folderPath, bool isSubdir = false);

		///Prepare Icons, copy and convert a icon.
		bool prepIcon(std::string & errMsg);

		///Make new UUID for name
		void genNewName();

	private:
		IconAssigner();
		~IconAssigner();

		
	};

	/************************************************************************/
	/* Simple class used with wxDir.Tranverse for getting dirs              */
	/************************************************************************/
	class wxDirRecorder : public wxDirTraverser
	{
	public:
		wxDirRecorder(std::string rootdir, unsigned short dirDepth = 0) 
		{ 
			m_bepthControl = (dirDepth != 0);
			unsigned int cnt = IconAssigner::slashCount(rootdir.c_str()); //get depth of root
			m_maxGrowDepth = cnt + dirDepth;
			
		};

		~wxDirRecorder() { };

		virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename))
		{
			return wxDIR_CONTINUE;
		};

		virtual wxDirTraverseResult OnDir(const wxString& dirname)
		{
			std::string str = dirname.ToStdString();
			unsigned int cnt = IconAssigner::slashCount(str.c_str());

			if (!m_bepthControl || cnt <= m_maxGrowDepth)
			{
				m_paths.push_back(str);
				return wxDIR_CONTINUE;
			}
			else
				return wxDIR_IGNORE;
		};

		std::vector<std::string> & getPaths() { return m_paths; };

	private:
		std::vector<std::string> m_paths;
		unsigned int m_maxGrowDepth;
		bool m_bepthControl;
	};


};


#endif