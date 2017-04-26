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
#ifndef _PREREQ_H_
#define _PREREQ_H_

namespace FolderIconAssigner
{

	#define APPLICATION_VERSION 3.3
	#define APPLICATION_VERSIONNAME "beta"
	#define APPLICATION_NAME "Folder Icon Assigner 3.3"
	#define APPLICATION_TITLE "Folder Icon Assigner 3.3"

	#define APPLICATION_DATAFOLDERNAME "FolderIconAssigner3"
	#define APPLICATION_AUTHOR "Warkanum (H.Puth)"
	#define APPLICATION_ICON_FILE "icon.png"
	#define APPLICATION_PLATFORM WIN32


	#define NIX(p)		p = nullptr;	//Make nullptr
	#define ISNIX(p)	((p) == nullptr)
	#define ISNNIX(p)	((p) != nullptr)
	#define SDELETE(p)      if ( (p) != nullptr ) { delete p; p = nullptr; } //safe delete by checking if null
	#define SDELETEA(p)     if ( (p) ) { delete [] (p); p = nullptr; } //array safe del
	#define myWXDESTROY(p)      if ( (p) != nullptr ) { p->Destroy(); p = nullptr; } //safe destroy of wx object.

	#define DIR_SLASH "\\"
	#define APPLICATION_PREFER_WINAPI
	#define _AFXDLL
	#define APPLICATION_USE_FREEIMAGE FREEIMAGE_LIB

//About the application
#define APPLICATION_ABOUT "\
Folder Icon Assigner was developed by me, Warkanum also \
known as H.Puth. The idea was to customise my folders with icons \
and I came up with folder icon assigner. This program serves only \
as a tool to assigned icons to folders. These icon will always be there \
once you assign them. You can remove icons from folders by using the \
remove button. Please note the this program modifies the desktop.ini file \
inside folder and also copies an icon with the folder name into the folder.\
\nThanks for using Folder Icon Assigner."


};

#endif