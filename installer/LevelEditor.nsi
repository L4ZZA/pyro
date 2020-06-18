;Written by JPietro Lazzarotto
;Credits https://youtu.be/RLv0JJKb8KU?t=1116

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Global definitions
 !define APP_NAME "Level Editor"
 !define APP_NAME_SLUG "level_editor"
 !define INST_FOLDER_NAME "LevelEditor"
 !define FILE_NAME "LevelEditorSetup.exe"
 !define ROOT_DIR ".."
 !define PROJECT_DIR "${ROOT_DIR}\${APP_NAME_SLUG}"
 !define CONFIG_DIR "Release-windows-x86_64"
 !define BIN_DIR "${ROOT_DIR}\bin\${CONFIG_DIR}"
 !define OUTPUT_DIR "${BIN_DIR}\installers"
 ; http://www.differencebetween.net/technology/hardware-technology/difference-between-hkey_current_user-and-hkey_local_machine/#:~:text=Difference%20Between%20HKEY_CURRENT_USER%20and%20HKEY_LOCAL_MACHINE,-•%20Categorized%20under&text=HKEY_LOCAL_MACHINE%20holds%20information%20that%20is,is%20specific%20to%20the%20user.
 !define REG_ROOT "HKLM"
 !define REG_KEY "Software\${INST_FOLDER_NAME}"

;Scripts
 ; Reference: https://nsis-dev.github.io/NSIS-Forums/html/t-302311.html
 !system 'md ${OUTPUT_DIR}'

;General

  ;Name and file
  Name ${APP_NAME}
  OutFile "${OUTPUT_DIR}\${FILE_NAME}"
  Unicode True
  ShowInstDetails show
  ShowUninstDetails show

  ;Default installation folder
  InstallDir "$DESKTOP\${INST_FOLDER_NAME}"
  
  ;Get installation folder from registry if available
  InstallDirRegKey ${REG_ROOT} ${REG_KEY} ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  ; Default, name is used if not defined
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${INST_FOLDER_NAME}" 

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "${ROOT_DIR}\license.md"
  ; !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT ${REG_ROOT} 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY ${REG_KEY} 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

; Remove empty quotes and uncomment the code after it
; if want to bring back the component selection section
; [don't forget the descriptions constants]
Section "" ;"${APP_NAME}" ComponentsSec

  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  
  ; remove all files inside installer folder
  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR\assets"

  ; Editor
  File "${BIN_DIR}\level_editor\level_editor.exe"
  ; Assets
  File /r "${PROJECT_DIR}\assets"

  ;Store installation folder
  WriteRegStr ${REG_ROOT} ${REG_KEY} "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Descriptions

  ; Remove uncomment the code the following six lines
  ; if want to bring back the component selection section

  ; ;Language strings
  ; LangString DESC_ComponentsSec ${LANG_ENGLISH} "A level editor for 2D games."

  ; ;Assign language strings to sections
  ; !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  ;   !insertmacro MUI_DESCRIPTION_TEXT ${ComponentsSec} $(DESC_ComponentsSec)
  ; !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey /ifempty ${REG_ROOT} ${REG_KEY}

SectionEnd