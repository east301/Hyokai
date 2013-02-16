!include MUI2.nsh
!include "FileAssociation.nsh"

;--------------------------------

; The name of the installer
Name "Table View"

; Icons
Icon  "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
UninstallIcon  "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

; The file to write
OutFile "inst_tableview.exe"

; The default installation directory
InstallDir $PROGRAMFILES\TableView

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\info_informationsea_tableview" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin


; Compressor
SetCompressor lzma


!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp" ; optional
!define MUI_ABORTWARNING

;--------------------------------

; Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\COPYING"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH


; Language
!insertmacro MUI_LANGUAGE "English"

;--------------------------------

; The stuff to install
Section "Table View (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "release\TableView.exe"
  File "License.txt"
  File "\Qt\4.8.4\bin\QtCore4.dll"
  File "\Qt\4.8.4\bin\QtGui4.dll"
  File "\Qt\4.8.4\bin\QtSql4.dll"
  
  ;; Register Extension
  ${registerExtension} $INSTDIR\TableView.exe ".sqlite3" "SQLite3 Database"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\info_informationsea_tableview "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\info_informationsea_tableview" "DisplayName" "Table View"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\info_informationsea_tableview" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\info_informationsea_tableview" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\info_informationsea_tableview" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\TableView"
  CreateShortCut "$SMPROGRAMS\TableView\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\TableView\TablewView.lnk" "$INSTDIR\TableView.exe" "" "$INSTDIR\TableView.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Example2"
  DeleteRegKey HKLM SOFTWARE\info_informationsea_tableview

  ; Remove files and uninstaller
  Delete $INSTDIR\TableView.exe
  Delete $INSTDIR\QtCore4.dll
  Delete $INSTDIR\QtGui4.dll
  Delete $INSTDIR\QtSql4.dll
  Delete $INSTDIR\License.txt
  Delete $INSTDIR\uninstall.exe
  
  ${unregisterExtension} ".sqlite3" "SQLite3 Database"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\TableView\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\TableView"
  RMDir "$INSTDIR"

SectionEnd
