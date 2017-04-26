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
#include "MainWnd.hpp"
#include "IconAssigner.hpp"
#include "ImageWorker.hpp"
#include "wx/msgdlg.h"

namespace FolderIconAssigner
{
	//Always add:
	//SetIcon(wxIcon("ICON_MAIN"));


	MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
		: wxFrame( parent, id, title, pos, size, style )
	{
		SetIcon(wxIcon("ICON_MAIN"));

		this->SetSizeHints( wxSize( 500,340 ), wxSize( 550,400 ) );
		this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

		wxFlexGridSizer* fgSizerPanels;
		fgSizerPanels = new wxFlexGridSizer( 4, 1, 1, 1 );
		fgSizerPanels->SetFlexibleDirection( wxBOTH );
		fgSizerPanels->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		m_pnlFolderSelect = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
		m_pnlFolderSelect->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
		m_pnlFolderSelect->SetMaxSize( wxSize( -1,30 ) );

		wxBoxSizer* bSizerFolderSelect;
		bSizerFolderSelect = new wxBoxSizer( wxHORIZONTAL );

		m_lblFolderSelect = new wxStaticText( m_pnlFolderSelect, wxID_ANY, wxT("Select Folder:"), wxDefaultPosition, wxDefaultSize, 0 );
		m_lblFolderSelect->Wrap( -1 );
		bSizerFolderSelect->Add( m_lblFolderSelect, 0, wxALIGN_CENTER|wxALL, 5 );

		m_dirPickFolder = new wxDirPickerCtrl( m_pnlFolderSelect, wxID_ANY, wxEmptyString, wxT("Select the folder that you want to assign an Icon to."), wxPoint( -1,-1 ), wxSize( -1,-1 ), wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST );
		m_dirPickFolder->SetToolTip( wxT("Select or paste folder path to use in here!") );
		m_dirPickFolder->SetMinSize( wxSize( 300,20 ) );
		m_dirPickFolder->SetMaxSize( wxSize( -1,30 ) );

		bSizerFolderSelect->Add( m_dirPickFolder, 0, wxALIGN_CENTER|wxALL, 5 );

		m_pnlFolderSelect->SetSizer( bSizerFolderSelect );
		m_pnlFolderSelect->Layout();
		bSizerFolderSelect->Fit( m_pnlFolderSelect );
		fgSizerPanels->Add( m_pnlFolderSelect, 1, wxEXPAND | wxALL, 5 );

		m_pnlIconSelect = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
		m_pnlIconSelect->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

		wxBoxSizer* bSizerIconSelect;
		bSizerIconSelect = new wxBoxSizer( wxVERTICAL );

		bSizerIconSelect->SetMinSize( wxSize( -1,80 ) ); 
		wxBoxSizer* bSizerIconSelected;
		bSizerIconSelected = new wxBoxSizer( wxHORIZONTAL );

		m_lblIconSelect = new wxStaticText( m_pnlIconSelect, wxID_ANY, wxT("Select Icon source:"), wxDefaultPosition, wxDefaultSize, 0 );
		m_lblIconSelect->Wrap( -1 );
		bSizerIconSelected->Add( m_lblIconSelect, 0, wxALIGN_CENTER|wxALL, 5 );

		m_fileIcon = new wxFilePickerCtrl( m_pnlIconSelect, wxID_ANY, wxEmptyString, wxT("Select a file to be imported as a Icon."), wxT("Icon Files (ico, icon)|*.ico;*.icon|Image Files (jpg,bmp,gif,tif,tga)|*.jpg;*.bmp;*.png;*.gif;*.tif;*.tga|Icon Containers (exe,dll,icl)|*.exe;*.dll;*.icl|All Files|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
		m_fileIcon->SetToolTip( wxT("Select a file to be imported as a Icon. If the file is a icon, it will be used. If it is a container it will be referenced only or exported depending on your later choice. If the file is a image, it will be converted.") );
		m_fileIcon->SetMinSize( wxSize( 300,20 ) );
		m_fileIcon->SetMaxSize( wxSize( -1,30 ) );

		bSizerIconSelected->Add( m_fileIcon, 0, wxALIGN_CENTER|wxALL, 5 );

		bSizerIconSelect->Add( bSizerIconSelected, 1, wxEXPAND, 5 );

		wxBoxSizer* bSizerFolderIconSelected;
		bSizerFolderIconSelected = new wxBoxSizer( wxHORIZONTAL );

		m_lblIconIndex = new wxStaticText( m_pnlIconSelect, wxID_ANY, wxT("Icon Container Index:"), wxDefaultPosition, wxDefaultSize, 0 );
		m_lblIconIndex->Wrap( -1 );
		bSizerFolderIconSelected->Add( m_lblIconIndex, 0, wxALIGN_CENTER|wxALL, 5 );

		m_spinIndex = new wxSpinCtrl( m_pnlIconSelect, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 100000000, -3 );
		m_spinIndex->SetToolTip( wxT("The index (id) of the icon in the container file. Starts at 0 and increments usually.") );

		bSizerFolderIconSelected->Add( m_spinIndex, 0, wxALIGN_CENTER|wxALL, 5 );

		m_chkExportFromContainer = new wxCheckBox( m_pnlIconSelect, wxID_ANY, wxT("Copy out of container:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		m_chkExportFromContainer->SetValue(true); 
		m_chkExportFromContainer->SetToolTip( wxT("Copies the icon out of the container instead of using it from within the container. This option overrides \"Copy icon to folder\".") );

		bSizerFolderIconSelected->Add( m_chkExportFromContainer, 0, wxALIGN_CENTER|wxALL, 5 );

		bSizerIconSelect->Add( bSizerFolderIconSelected, 1, wxEXPAND, 5 );

		m_pnlIconSelect->SetSizer( bSizerIconSelect );
		m_pnlIconSelect->Layout();
		bSizerIconSelect->Fit( m_pnlIconSelect );
		fgSizerPanels->Add( m_pnlIconSelect, 1, wxEXPAND | wxALL, 5 );

		m_pnlOptions = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
		m_pnlOptions->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
		m_pnlOptions->SetMinSize( wxSize( -1,100 ) );

		wxFlexGridSizer* fgSizerOptions;
		fgSizerOptions = new wxFlexGridSizer( 1, 3, 0, 0 );
		fgSizerOptions->SetFlexibleDirection( wxBOTH );
		fgSizerOptions->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		fgSizerOptions->SetMinSize( wxSize( -1,80 ) ); 
		m_IconPreview = new wxStaticBitmap( m_pnlOptions, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
		m_IconPreview->SetToolTip( wxT("Preview of the icon!") );
		m_IconPreview->SetMinSize( wxSize( 64,64 ) );
		m_IconPreview->SetMaxSize( wxSize( 64,64 ) );

		fgSizerOptions->Add( m_IconPreview, 0, wxALL, 5 );

		wxGridSizer* gSizerCheckBoxes1;
		gSizerCheckBoxes1 = new wxGridSizer( 3, 1, 0, 0 );

		m_chkCopyToFolder = new wxCheckBox( m_pnlOptions, wxID_ANY, wxT("Copy icon to folder:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		m_chkCopyToFolder->SetValue(true); 
		m_chkCopyToFolder->SetToolTip( wxT("When you copy the icon to the folder, you can use the folder on other computers and the icon will stay.\nThis will be on by default because that is the purpose of this program.\nIf you don't copy it to the folder, exporting and using the root folder's icon won't work. Thus all the folders references the location.") );

		gSizerCheckBoxes1->Add( m_chkCopyToFolder, 0, wxALIGN_RIGHT|wxALL, 5 );

		m_chkRecursive = new wxCheckBox( m_pnlOptions, wxID_ANY, wxT("Recursive (Sub Folders):"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		m_chkRecursive->SetToolTip( wxT("Apply the icon to the selected folder and also to it's subfolder. The Subfolder depth controls the depth. Example: c:Mydir(0)Subdir(1)SubSubDir(2)") );

		gSizerCheckBoxes1->Add( m_chkRecursive, 0, wxALL, 5 );

		m_chkReferenceRoot = new wxCheckBox( m_pnlOptions, wxID_ANY, wxT("Use root folder's icon:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
		m_chkReferenceRoot->SetValue(true); 
		m_chkReferenceRoot->Enable( false );
		m_chkReferenceRoot->SetToolTip( wxT("This option lets you use the root folder's icon on all the subfolders if you are applying the icons recursively.") );

		gSizerCheckBoxes1->Add( m_chkReferenceRoot, 0, wxALIGN_RIGHT|wxALL, 5 );

		fgSizerOptions->Add( gSizerCheckBoxes1, 1, wxEXPAND, 5 );

		wxFlexGridSizer* fgSizerAdvOptions;
		fgSizerAdvOptions = new wxFlexGridSizer( 2, 2, 0, 0 );
		fgSizerAdvOptions->SetFlexibleDirection( wxBOTH );
		fgSizerAdvOptions->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		m_lblFolderDepth = new wxStaticText( m_pnlOptions, wxID_ANY, wxT("Subfolder Depth:"), wxDefaultPosition, wxDefaultSize, 0 );
		m_lblFolderDepth->Wrap( -1 );
		m_lblFolderDepth->SetToolTip( wxT(" The Subfolder depth controls the depth on the folder level. Example: c:Mydir(0)Subdir(1)SubSubDir(2).\n0 Means all subfolders with infinite depth.") );

		fgSizerAdvOptions->Add( m_lblFolderDepth, 0, wxALIGN_CENTER|wxALL, 5 );

		m_spinFolderDepth = new wxSpinCtrl( m_pnlOptions, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
		m_spinFolderDepth->Enable( false );
		m_spinFolderDepth->SetToolTip( wxT(" The Subfolder depth controls the depth on the folder level. Example: c:Mydir(0)Subdir(1)SubSubDir(2).\n0 Means all subfolders with infinite depth.") );
		m_spinFolderDepth->SetMaxSize( wxSize( 100,-1 ) );

		fgSizerAdvOptions->Add( m_spinFolderDepth, 0, wxALL, 5 );

		m_chkResampleAll = new wxCheckBox( m_pnlOptions, wxID_ANY, wxT("Resample All"), wxDefaultPosition, wxDefaultSize, 0 );
		m_chkResampleAll->SetToolTip( wxT("Resize, sample and save the source icon to a new one.\nThis is automatically done for pictures. \nPrefer this to be off unless you want to resample your existing icons.") );

		fgSizerAdvOptions->Add( m_chkResampleAll, 0, wxALIGN_CENTER|wxALL, 5 );

		m_chkUniqueName = new wxCheckBox( m_pnlOptions, wxID_ANY, wxT("Unique Icon Names"), wxDefaultPosition, wxDefaultSize, 0 );
		m_chkUniqueName->SetToolTip( wxT("Should the icon that is copied to the folders have a unique name.\nSet when you have problems with windows icon cache.") );

		fgSizerAdvOptions->Add( m_chkUniqueName, 0, wxALIGN_CENTER|wxALL, 5 );

		fgSizerOptions->Add( fgSizerAdvOptions, 1, wxEXPAND, 5 );

		m_pnlOptions->SetSizer( fgSizerOptions );
		m_pnlOptions->Layout();
		fgSizerOptions->Fit( m_pnlOptions );
		fgSizerPanels->Add( m_pnlOptions, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );

		wxFlexGridSizer* fgSizerButtons;
		fgSizerButtons = new wxFlexGridSizer( 0, 5, 1, 3 );
		fgSizerButtons->SetFlexibleDirection( wxBOTH );
		fgSizerButtons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		fgSizerButtons->SetMinSize( wxSize( -1,30 ) ); 
		m_btnQuit = new wxButton( this, wxID_ANY, wxT("&Quit"), wxDefaultPosition, wxDefaultSize, 0 );
		m_btnQuit->SetMinSize( wxSize( 80,-1 ) );

		m_btnAbout = new wxButton( this, wxID_ANY, wxT("Abou&t"), wxDefaultPosition, wxDefaultSize, 0 );
		m_btnAbout->SetMinSize( wxSize( 80,-1 ) );

		m_btnFix = new wxButton( this, wxID_ANY, wxT("Fi&x"), wxDefaultPosition, wxDefaultSize, 0 );
		m_btnFix->SetMinSize( wxSize( 80,-1 ) );

		m_btnClear = new wxButton( this, wxID_ANY, wxT("&Remove"), wxDefaultPosition, wxDefaultSize, 0 );
		m_btnClear->SetMinSize( wxSize( 80,-1 ) );

		m_btnApply = new wxButton( this, wxID_ANY, wxT("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
		m_btnApply->SetDefault(); 
		m_btnApply->SetMinSize( wxSize( 80,-1 ) );

		fgSizerButtons->Add( m_btnApply, 0, wxALL, 5 );
		fgSizerButtons->Add( m_btnFix, 0, wxALL, 5 );
		fgSizerButtons->Add( m_btnClear, 0, wxALL, 5 );
		fgSizerButtons->Add( m_btnAbout, 0, wxALL, 5 );
		fgSizerButtons->Add( m_btnQuit, 0, wxALL, 5 );

		fgSizerPanels->Add( fgSizerButtons, 1, wxEXPAND, 5 );

		this->SetSizer( fgSizerPanels );
		this->Layout();
		m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
		m_menubar = new wxMenuBar( 0 );
		mnuFile = new wxMenu();

		wxMenuItem* apply;
		apply = new wxMenuItem( mnuFile, ID_APPLY, wxString( wxT("Apply") ) , wxT("Apply Icon"), wxITEM_NORMAL );
		mnuFile->Append( apply );
		
		wxMenuItem* clear;
		clear = new wxMenuItem( mnuFile, ID_CLEAR, wxString( wxT("Remove") ) , wxT("Remove the icons"), wxITEM_NORMAL );
		mnuFile->Append( clear );

		wxMenuItem* mifix;
		mifix = new wxMenuItem( mnuFile, ID_FIX, wxString( wxT("Fix Icons") ) , wxT("Fixes the icons if they are not displayed anymore."), wxITEM_NORMAL );
		mnuFile->Append( mifix );

		wxMenuItem* exit;
		exit = new wxMenuItem( mnuFile, ID_EXIT, wxString( wxT("Exit") ) , wxT("Exit Applications"), wxITEM_NORMAL );
		mnuFile->Append( exit );

		m_menubar->Append( mnuFile, wxT("File") );

		
		mnuHelp = new wxMenu();
		wxMenuItem* about;
		about = new wxMenuItem( mnuHelp, ID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
		mnuHelp->Append( about );

		m_menubar->Append( mnuHelp, wxT("Help") ); 

		this->SetMenuBar( m_menubar );


		this->Centre( wxBOTH );

		// Connect Events
		m_dirPickFolder->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainWindow::folderChanged ), NULL, this );
		m_fileIcon->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainWindow::sourceChanged ), NULL, this );
		m_spinIndex->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainWindow::sourceIndexChanged ), NULL, this );
		m_chkRecursive->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainWindow::chkRecursiveChecked ), NULL, this );
		m_btnQuit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::exitClick ), NULL, this );
		m_btnAbout->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::aboutClick ), NULL, this );
		m_btnFix->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::fixClick ), NULL, this );
		m_btnClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::clearClicked ), NULL, this );
		m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::applyIconClick ), NULL, this );
		this->Connect( clear->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::clearClicked ) );
		this->Connect( apply->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::applyIconClick ) );
		this->Connect( exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::exitClick ) );
		this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::aboutClick ) );
		this->Connect( mifix->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::fixClick ) );
		
	}

	MainWindow::~MainWindow()
	{
		// Disconnect Events
		m_dirPickFolder->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainWindow::folderChanged ), NULL, this );
		m_fileIcon->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainWindow::sourceChanged ), NULL, this );
		m_spinIndex->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainWindow::sourceIndexChanged ), NULL, this );
		m_chkRecursive->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainWindow::chkRecursiveChecked ), NULL, this );
		m_btnQuit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::exitClick ), NULL, this );
		m_btnAbout->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::aboutClick ), NULL, this );
		m_btnClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::clearClicked ), NULL, this );
		m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::applyIconClick ), NULL, this );
		this->Disconnect( ID_CLEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::clearClicked ) );
		this->Disconnect( ID_APPLY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::applyIconClick ) );
		this->Disconnect( ID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::exitClick ) );
		this->Disconnect( ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::aboutClick ) );


	}

	void MainWindow::updatePreview()
	{
		//When we change the paths, this gets called.
		if (ISNNIX(m_dirPickFolder))
			IconAssigner::getInstancePtr()->setFolderPath(m_dirPickFolder->GetPath().ToStdString());

		if (ISNNIX(m_fileIcon))
			IconAssigner::getInstancePtr()->setImagePath(m_fileIcon->GetPath().ToStdString());

		if (ISNNIX(m_spinIndex))
			IconAssigner::getInstancePtr()->setContainerIndex(m_spinIndex->GetValue());

		std::string errMsg; 
		std::string path = IconAssigner::getInstancePtr()->getAppDataFolder() + std::string(DIR_SLASH) 
			+ "previewtempicon.ico";
		try
		{
			if (IconAssigner::getInstancePtr()->generatePreviewIcon(path, errMsg))
			{
				wxInitAllImageHandlers();
				wxImage image = wxImage();
				image.LoadFile(path);

				wxBitmap bitmap = wxBitmap(image.ResampleBicubic(64,64));
				m_IconPreview->SetBitmap(bitmap);
			}
		}
		catch (std::exception * e)
		{
			//silently ignore, this is only for icon preview.
		}
	}

	void MainWindow::sourceChanged( wxFileDirPickerEvent& event )
	{
		updatePreview();
		
	}

	void MainWindow::folderChanged( wxFileDirPickerEvent& event )
	{

	}

	void MainWindow::sourceIndexChanged( wxSpinEvent& event )
	{
		updatePreview();
	}

	void MainWindow::chkRecursiveChecked( wxCommandEvent& event )
	{
		if (m_chkRecursive->GetValue())
		{
			m_chkReferenceRoot->Enable(true);
			m_spinFolderDepth->Enable(true);
		}
		else
		{
			m_chkReferenceRoot->Enable(false);
			m_spinFolderDepth->Enable(false);
		}
		
	}

	void MainWindow::exitClick( wxCommandEvent& event )
	{
		this->Close();
	}

	void MainWindow::aboutClick( wxCommandEvent& event )
	{
		wxMessageBox(_(APPLICATION_ABOUT),
			_("About"),wxICON_NONE | wxOK, this);
	}

	void MainWindow::clearClicked( wxCommandEvent& event )
	{
		try
		{
			if (ISNNIX(m_dirPickFolder))
				IconAssigner::getInstancePtr()->setFolderPath(m_dirPickFolder->GetPath().ToStdString());

			if (ISNNIX(m_spinIndex))
				IconAssigner::getInstancePtr()->setContainerIndex(m_spinIndex->GetValue());

			if (ISNNIX(m_spinFolderDepth))
				IconAssigner::getInstancePtr()->setFolderDepth((unsigned short)m_spinFolderDepth->GetValue());
	
			if (ISNNIX(m_chkResampleAll))
				IconAssigner::getInstancePtr()->setResampleAll(m_chkResampleAll->GetValue());

			if (ISNNIX(m_chkUniqueName))
				IconAssigner::getInstancePtr()->setUniqueIconNames(m_chkUniqueName->GetValue());

			if (ISNNIX(m_chkCopyToFolder))
				IconAssigner::getInstancePtr()->setCopyToFolder(m_chkCopyToFolder->GetValue());

			if (ISNNIX(m_chkRecursive))
				IconAssigner::getInstancePtr()->setRecursive(m_chkRecursive->GetValue());

			if (ISNNIX(m_chkReferenceRoot))
				IconAssigner::getInstancePtr()->setReference(m_chkReferenceRoot->GetValue());

			std::string errMsg;

			if (IconAssigner::getInstancePtr()->cleanFolderIcons(errMsg))
				wxMessageBox(_("Icons successfully deleted!"),
				_("Icons successfully deleted!"),wxICON_INFORMATION | wxOK, this);
			else
				wxMessageBox(_(errMsg), _("Icons could not be deleted!"),wxICON_ERROR | wxOK, this);

		}
		catch (std::exception * ex)
		{
			wxMessageBox(_(ex->what()),
				_("An error occured during icon fix! "),wxICON_ERROR | wxOK, this);
		}
	}

	void MainWindow::fixClick( wxCommandEvent& event )
	{
		try
		{
			if (ISNNIX(m_dirPickFolder))
				IconAssigner::getInstancePtr()->setFolderPath(m_dirPickFolder->GetPath().ToStdString());

			if (ISNNIX(m_spinIndex))
				IconAssigner::getInstancePtr()->setContainerIndex(m_spinIndex->GetValue());

			if (ISNNIX(m_spinFolderDepth))
				IconAssigner::getInstancePtr()->setFolderDepth((unsigned short)m_spinFolderDepth->GetValue());
	
			if (ISNNIX(m_chkResampleAll))
				IconAssigner::getInstancePtr()->setResampleAll(m_chkResampleAll->GetValue());

			if (ISNNIX(m_chkUniqueName))
				IconAssigner::getInstancePtr()->setUniqueIconNames(m_chkUniqueName->GetValue());

			if (ISNNIX(m_chkCopyToFolder))
				IconAssigner::getInstancePtr()->setCopyToFolder(m_chkCopyToFolder->GetValue());

			if (ISNNIX(m_chkRecursive))
				IconAssigner::getInstancePtr()->setRecursive(m_chkRecursive->GetValue());

			if (ISNNIX(m_chkReferenceRoot))
				IconAssigner::getInstancePtr()->setReference(m_chkReferenceRoot->GetValue());

			std::string errMsg;

			if (IconAssigner::getInstancePtr()->fixFolderIcons(errMsg))
				wxMessageBox(_("Icons successfully fixed!"),
				_("Icons successfully fixed!"),wxICON_INFORMATION | wxOK, this);
			else
				wxMessageBox(_(errMsg), _("Icons could not be fixed!"),wxICON_ERROR | wxOK, this);

		}
		catch (std::exception * ex)
		{
			wxMessageBox(_(ex->what()),
				_("An error occured during icon fix! "),wxICON_ERROR | wxOK, this);
		}
	}

	void MainWindow::applyIconClick( wxCommandEvent& event )
	{
		try
		{
			if (ISNNIX(m_dirPickFolder))
				IconAssigner::getInstancePtr()->setFolderPath(m_dirPickFolder->GetPath().ToStdString());

			if (ISNNIX(m_fileIcon))
				IconAssigner::getInstancePtr()->setImagePath(m_fileIcon->GetPath().ToStdString());

			if (ISNNIX(m_spinIndex))
				IconAssigner::getInstancePtr()->setContainerIndex(m_spinIndex->GetValue());

			if (ISNNIX(m_spinFolderDepth))
				IconAssigner::getInstancePtr()->setFolderDepth((unsigned short)m_spinFolderDepth->GetValue());
	
			if (ISNNIX(m_chkResampleAll))
				IconAssigner::getInstancePtr()->setResampleAll(m_chkResampleAll->GetValue());

			if (ISNNIX(m_chkUniqueName))
				IconAssigner::getInstancePtr()->setUniqueIconNames(m_chkUniqueName->GetValue());

			if (ISNNIX(m_chkCopyToFolder))
				IconAssigner::getInstancePtr()->setCopyToFolder(m_chkCopyToFolder->GetValue());

			if (ISNNIX(m_chkRecursive))
				IconAssigner::getInstancePtr()->setRecursive(m_chkRecursive->GetValue());

			if (ISNNIX(m_chkReferenceRoot))
				IconAssigner::getInstancePtr()->setReference(m_chkReferenceRoot->GetValue());


			std::string errMsg;
			if (IconAssigner::getInstancePtr()->validateData(errMsg))
			{
				if (IconAssigner::getInstancePtr()->installIcons(errMsg))
					wxMessageBox(_("Icons successfully installed!"),
					_("Icons successfully installed!"),wxICON_INFORMATION | wxOK, this);
				else
					wxMessageBox(_(errMsg), _("Icons could not be installed!"),wxICON_ERROR | wxOK, this);
			}
			else
			{
				wxMessageBox(_(errMsg), _("Operation not successful!"),wxICON_ERROR | wxOK, this);
			}

		}
		catch (std::exception * ex)
		{
			wxMessageBox(_(ex->what()),
				_("An error occured during icon install"),wxICON_ERROR | wxOK, this);
		}
	}

};