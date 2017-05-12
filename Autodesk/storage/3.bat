@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script
set PROJECT_CENTRIC=d:\CodeSite\ACE\E-Branches\B\AcadE\AcadE\AceProjectCentric
set wix_toolkit="c:\Program Files (x86)\WiX Toolset v3.7\bin\"
set AdConcDocStore=%PROJECT_CENTRIC%\AdConcDocStore
set NODE_MODULES=%AdConcDocStore%\node_modules
set "OUT_FOLDER=%script_location%\zzWixFiles"
set result_files=%script_location%\result_files

rem dir /b %result_files%
for  %%G in ("%result_files%\*.wix") do (
	set name=%%~nG
	set name=!name:.=!
	ren %%G !name!.wix
	)
pause
endlocal
exit /b 0

:test
setlocal
set full=%1
set parent_folder=%~dp1
set folder_name=%~nx1

echo %full%
echo %parent_folder%
echo %folder_name%
endlocal
pause