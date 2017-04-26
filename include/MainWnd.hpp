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
#ifndef _MAINWND_H_
#define _MAINWND_H_
#include "PreReq.hpp"

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/dialog.h>

namespace FolderIconAssigner
{

	/************************************************************************/
	/* Main window class for aplication                                     */
	/************************************************************************/
	class MainWindow : public wxFrame 
	{
	private:

	protected:
		enum
		{
			ID_CLEAR = 1000,
			ID_APPLY,
			ID_EXIT,
			ID_ABOUT,
			ID_FIX,
		};

		wxPanel* m_pnlFolderSelect;
		wxStaticText* m_lblFolderSelect;
		wxDirPickerCtrl* m_dirPickFolder;
		wxPanel* m_pnlIconSelect;
		wxStaticText* m_lblIconSelect;
		wxFilePickerCtrl* m_fileIcon;
		wxStaticText* m_lblIconIndex;
		wxSpinCtrl* m_spinIndex;
		wxCheckBox* m_chkExportFromContainer;
		wxPanel* m_pnlOptions;
		wxCheckBox* m_chkCopyToFolder;
		wxCheckBox* m_chkRecursive;
		wxCheckBox* m_chkReferenceRoot;
		wxStaticText* m_lblFolderDepth;
		wxSpinCtrl* m_spinFolderDepth;
		wxCheckBox* m_chkResampleAll;
		wxCheckBox* m_chkUniqueName;
		wxButton* m_btnQuit;
		wxButton* m_btnAbout;
		wxButton* m_btnClear;
		wxButton* m_btnFix;
		wxButton* m_btnApply;
		wxStatusBar* m_statusBar;
		wxMenuBar* m_menubar;
		wxMenu* mnuFile;
		wxMenu* mnuHelp;

		void folderChanged( wxFileDirPickerEvent& event );
		void sourceChanged( wxFileDirPickerEvent& event );
		void sourceIndexChanged( wxSpinEvent& event );
		void chkRecursiveChecked( wxCommandEvent& event );
		void exitClick( wxCommandEvent& event );
		void aboutClick( wxCommandEvent& event );
		void fixClick( wxCommandEvent& event );
		void clearClicked( wxCommandEvent& event );
		void applyIconClick( wxCommandEvent& event );


	public:
		wxStaticBitmap* m_IconPreview;

		void updatePreview();

		MainWindow( wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT(APPLICATION_TITLE), 
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), 
			long style = wxDEFAULT_FRAME_STYLE|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
		~MainWindow();

	};

};



#endif