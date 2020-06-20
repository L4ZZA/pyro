@echo off
@rem credits: https://superuser.com/a/392082

@SETLOCAL ENABLEDELAYEDEXPANSION
@SET LinkName=%1
@SET Esc_LinkDest=%2\!LinkName!.lnk
@SET Esc_LinkTarget=%3
@SET cSctVBS=CreateShortcut.vbs
@SET LOG=".\%~N0_runtime.log"
((
  echo Set oWS = WScript.CreateObject^("WScript.Shell"^) 
  echo sLinkFile = oWS.ExpandEnvironmentStrings^("!Esc_LinkDest!"^)
  echo Set oLink = oWS.CreateShortcut^(sLinkFile^) 
  echo oLink.TargetPath = oWS.ExpandEnvironmentStrings^("!Esc_LinkTarget!"^)
  echo oLink.Save
)1>!cSctVBS!
@cscript //nologo .\!cSctVBS!
@DEL !cSctVBS! /f /q
)