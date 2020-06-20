@rem run following lines one folder above
pushd %~dp0\..\ 
call "C:\Program Files (x86)\NSIS\makensis.exe" "installer\LevelEditor.nsi"
popd
PAUSE
