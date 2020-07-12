@echo OFF
@rem Credits to Garret Wilson
@rem https://stackoverflow.com/questions/26551/how-can-i-pass-arguments-to-a-batch-file/50653047#50653047

@rem Prevent env variables to escape to the calling environment
SETLOCAL

@rem redefine variables in case already defined in the calling environment
SET CONFIG=%~1
SET DEFAULT_CONFIG=Release
SET SILENT_MODE=false

IF "%CONFIG%" == "" (
    SET CONFIG=%DEFAULT_CONFIG%
    echo No Configuration passed. Defaulted to %CONFIG%
    echo Usage: create_installer.bat ^<configuration^> [-s]
) ELSE IF "%CONFIG%" == "-s" (
    SET SILENT_MODE=true
    SET CONFIG=%DEFAULT_CONFIG%
    echo No Configuration passed. Defaulted to %CONFIG%
    echo Usage: create_installer.bat ^<configuration^> [-s]
) ELSE IF "%CONFIG:~0,1%" == "-" (
    SET CONFIG=%DEFAULT_CONFIG%
    echo No Configuration passed. Defaulted to %CONFIG%
    echo Usage: create_installer.bat ^<configuration^> [-s]
)

SHIFT
:args
SET PARAM=%~1
IF "%PARAM%" == "-s" (
  SHIFT
  SET SILENT_MODE=true
) ELSE IF "%PARAM%" == "" (
  GOTO endargs
) ELSE (
  echo Unrecognized option: %1. 1>&2
  echo:
  GOTO usage
)
GOTO args
:endargs

echo Configuration: %CONFIG%
IF "%SILENT_MODE%" == "true" (
  echo Silent mode active
) ELSE (
    echo Diagnostic mode active
)

@rem -- Run your logic here --

@rem run following lines one folder above
pushd %~dp0..\
echo %~dp0
@rem variables have to be defined before passing the installer path
call "C:\Program Files (x86)\NSIS\makensis.exe" "/DCONFIG_PARAM=%CONFIG%" "installer\Ember.nsi"
popd

IF %ERRORLEVEL% NEQ 0 (
    @rem silent mode
    if "%SILENT_MODE%" == "false" (
        PAUSE
    )
)

@rem -- End of your logic section --
GOTO :eof

:usage
echo Usage: create_installer.bat ^<configuration^> [-s]

@rem terminate script with specific error code https://ss64.com/nt/exit.html
EXIT /B 1
