; This file is part of yata -- Yet Another Tail Application
; Copyright 2010-2011 James Smith
;
; Licensed under the GNU General Public License.  See license.txt for details.

;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

!define APPNAME "Yata"
!define VERSION "0.1"
!define FULLAPPNAME "${APPNAME} ${VERSION}"
!define REGISTRYKEY "Software\${APPNAME}"
!ifndef EXESRCDIR
    !define EXESRCDIR "..\release"
!endif
!ifndef OUTDIR
    !define OUTDIR "."
!endif
!define QTDIR "C:\QtSDK"
!define MINGWDIR "${QTDIR}\mingw\bin"
!define QTBINDIR "${QTDIR}\Desktop\Qt\4.7.4\mingw\bin"

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "${FULLAPPNAME}"
  OutFile "${OUTDIR}\${FULLAPPNAME} Setup.exe"

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
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  ;!insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  ;!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section /o "Add shortcut to desktop" AddShortcutToDesktop
    CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
SectionEnd

Section /o "Add shortcut to quickstart menu" AddShortcutToQuickstart
    CreateShortCut "$QUICKLAUNCH\${APPNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
SectionEnd

ShowInstDetails show
Section
    SetOutPath "$INSTDIR"
    File "${MINGWDIR}\mingwm10.dll"
    File "${MINGWDIR}\libgcc_s_dw2-1.dll"

    File "${QTBINDIR}\QtCore4.dll"
    File "${QTBINDIR}\QtGui4.dll"

    File "${EXESRCDIR}\${APPNAME}.exe"

    ;Store installation folder
    WriteRegStr HKCU ${REGISTRYKEY} "" $INSTDIR

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ;Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\${APPNAME}"
    CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
    CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

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

    Delete "$INSTDIR\mingwm10.dll"
    Delete "$INSTDIR\libgcc_s_dw2-1.dll"

    Delete "$INSTDIR\QtCore4.dll"
    Delete "$INSTDIR\QtGui4.dll"

    Delete "$INSTDIR\Yata.exe"

    Delete "$INSTDIR\Uninstall.exe"

    RMDir "$INSTDIR"

    Delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
    Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${APPNAME}"

    Delete "$DESKTOP\${APPNAME}.lnk"
    Delete "$QUICKLAUNCH\${APPNAME}.lnk"

    DeleteRegKey /ifempty HKCU ${REGISTRYKEY}

SectionEnd
