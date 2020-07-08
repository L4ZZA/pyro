@rem run following lines one folder above
pushd %~dp0..\ 
call vendor\premake\premake5.exe vs2019
popd
IF %ERRORLEVEL% NEQ 0 (
    @rem silent mode
    if "%~1"=="" (
        PAUSE
    )
)