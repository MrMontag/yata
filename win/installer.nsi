;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

!define APPNAME "Yata"
!define REGISTRYKEY "Software\${APPNAME}"

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name ${APPNAME}
  OutFile "Yata Setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${APPNAME}"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU ${REGISTRYKEY} ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "yata.ico"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section /o "Add shortcut to desktop" AddShortcutToDesktop
SectionEnd

Section /o "Add shortcut to quickstart menu" AddShortcutToQuickstart
SectionEnd

ShowInstDetails show
Section
  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...

  ;Store installation folder
  WriteRegStr HKCU ${REGISTRYKEY} "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_AddShortcutToDesktop ${LANG_ENGLISH} "Add a shortcut to Yata on the desktop."
  LangString DESC_AddShortcutToQuickstart ${LANG_ENGLISH} "Add a shortcut to Yata in the quickstart menu."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${AddShortcutToDesktop} $(DESC_AddShortcutToDesktop)
    !insertmacro MUI_DESCRIPTION_TEXT ${AddShortcutToQuickstart} $(DESC_AddShortcutToQuickstart)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU ${REGISTRYKEY}

SectionEnd
