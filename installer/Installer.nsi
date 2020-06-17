;Written by JPietro Lazzarotto
;Credits https://youtu.be/RLv0JJKb8KU?t=1116

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Global definitions
 !define ROOT_DIR ".."
 !define PROJECT_DIR "${ROOT_DIR}\level_editor"
 !define OUTPUT_DIR "${ROOT_DIR}\bin\installers"
 !define APP_NAME "Level Editor"
 !define INST_FOLDER_NAME "LevelEditor"
 !define FILE_NAME "LevelEditorSetup.exe"
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

Section "Dummy Section" ComponentsSec

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  File "${ROOT_DIR}\bin\Release-windows-x86_64\level_editor\level_editor.exe"
  
  SetOutPath "$INSTDIR\assets\shaders\"
  File "${PROJECT_DIR}\assets\shaders\texture_2d.glsl"

  SetOutPath "$INSTDIR\assets\textures\"
  File "${PROJECT_DIR}\assets\textures\*.png"

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

  ;Language strings
  LangString DESC_ComponentsSec ${LANG_ENGLISH} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${ComponentsSec} $(DESC_ComponentsSec)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey /ifempty ${REG_ROOT} ${REG_KEY}

SectionEnd