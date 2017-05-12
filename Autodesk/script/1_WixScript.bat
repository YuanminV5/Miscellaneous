@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script
set wix_toolkit="c:\Program Files (x86)\WiX Toolset v3.7\bin\"

set NODE_MODULES=U:\Schematics\3rdParty\node_modules

rem OUT_FOLDER to save raw wix files newly created by wix_tool
set "OUT_FOLDER=%script_location%\1_output_RawWixFiles"

rem %RESULT_FILES% folder to save wix files after removing directory, replacing SourceDir
set RESULT_FILES=%script_location%\1_output_ResultWixFiles

rem %file_list% list of wix files in %RESULT_FILES%, for step3
set file_list=%script_location%\1_output_wixfilelist.txt

set source_dir=$(env.ACEROOT)\3rdParty\node_modules

if exist %OUT_FOLDER% (
del /Q "%OUT_FOLDER%"
)
mkdir %OUT_FOLDER%

rem *************************************************************
rem       Generate wxs files via calling wxs harvest tool
rem *************************************************************
echo %NODE_MODULES%
echo Generating wxs files via calling wxs harvest tool...

setlocal EnableDelayedExpansion

for /D %%G in ("%NODE_MODULES%\*.*") do (
    :: Generate wxs files for each folder inside node_modules
	set name=%%~nxG
	call %wix_toolkit%\heat.exe dir %%~fG -gg -sfrag -template fragment -out "%OUT_FOLDER%\!name!.wxs" 
	)

cd /D "%script_location%"

if exist %RESULT_FILES% (
del /Q %RESULT_FILES%
)
mkdir %RESULT_FILES%

rem *************************************************************
rem                  Remove Directory tag
rem *************************************************************
echo removing Directory tag
for %%G in ("%OUT_FOLDER%\*.wxs") do (
	echo Removing Directory tag for %%G ...
	findstr /v /c:"DirectoryRef" /c:"Directory Id" /c:"</Directory>" %%G >> %RESULT_FILES%\%%~nG.wxs
	)


rem *************************************************************
rem Replace SourceDir in all wxs files to $(var.node_modules)\filename
rem *************************************************************
echo Replacing SourceDir in all wxs files to %source_dir%\filename
for %%G in ("%RESULT_FILES%\*.wxs") do (
	echo Replacing SourceDir in %%G ...
	for /F "delims=" %%i in (%%G) do (
		set "line=%%i" 
		set "line=!line:SourceDir=%source_dir%\%%~nG!"
		echo !line!
		) >> %RESULT_FILES%\%%~nG.temp
	)
del %RESULT_FILES%\*.wxs
cd /d %RESULT_FILES%
ren *.temp *.wxs
cd /d %script_location%
	
rem *************************************************************
rem Remove dot(.) in all wxs filename
rem *************************************************************
echo Removing dot(.) in all wxs filename
for  %%G in ("%RESULT_FILES%\*.wxs") do (
	set name=%%~nG
	set name=!name:.=!
	ren %%G !name!.wxs
	)
	


if exist %file_list% (
del %file_list%
)

rem *************************************************************
rem List down all processed wxs files to file called file_list.txt
rem *************************************************************
echo Listing down all processed wxs files to file called file_list.txt ...
dir /b /s %RESULT_FILES% > %file_list%
endlocal

pause
