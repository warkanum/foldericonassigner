; IconAssigner 3 installer script based of example2.nsi
;

;--------------------------------

; The name of the installer
Name "Folder Icon Assigner 3"

; The file to write
OutFile "FolderIconAssigner3_installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\FolderIconAssigner3

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\FolderIconAssigner3" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "FolderIconAssigner3 (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "FolderIconAssigner3.exe"
  File "readme.txt"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\FolderIconAssigner3 "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FolderIconAssigner3" "DisplayName" "Folder Icon Assigner 3"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FolderIconAssigner3" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FolderIconAssigner3" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FolderIconAssigner3" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\FolderIconAssigner3"
  CreateShortCut "$SMPROGRAMS\FolderIconAssigner3\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\FolderIconAssigner3\Folder Icon Assigner 3.lnk" "$INSTDIR\FolderIconAssigner3.exe" "" "$INSTDIR\FolderIconAssigner3.exe" 0
  CreateShortCut "$SMPROGRAMS\FolderIconAssigner3\Readme.lnk" "$INSTDIR\readme.txt"
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FolderIconAssigner3"
  DeleteRegKey HKLM SOFTWARE\FolderIconAssigner3

  ; Remove files and uninstaller
  Delete $INSTDIR\FolderIconAssigner3.exe
  Delete $INSTDIR\readme.txt
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\FolderIconAssigner3\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\FolderIconAssigner3"
  RMDir "$INSTDIR"

SectionEnd
