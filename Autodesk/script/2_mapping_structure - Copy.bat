@echo off
echo.
echo Creating folder structure and DIRECTORY ID - FOLDER NAME mapping
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script

set THIRDPARTY=U:\Schematics\3rdParty
set NODE_MODULES=%THIRDPARTY%\node_modules

rem folder for step3 output
set STEP3_OUTPUT=%script_location%\3_output_final
rem %dir_fragment% obsolete, use %dir_fragment_2%
set dir_fragment=%script_location%\2_output_all_dirfragment.txt
set resolve_conflict=%script_location%\2_output_tmp_resolve_conflict.txt
set tmp_mapping=%script_location%\2_output_tmp_mapping.tmp

rem %ID_Name_mapping% for step3 to add Directory to wix files in folder 1_output_ResultWixFiles
set ID_Name_mapping=%script_location%\2_output_ID_Name_mapping.txt

rem %dir_fragment_2% record DirectoryID, Directory Name, ParentDirectoryID for step6 6_CreateDIRFragment to create directory fragments
set dir_fragment_2=%script_location%\2_output_for_step6_dirfragment.txt
set STEP6_OUTPUT_FOLDER=%script_location%\6_output_NM_dirfragment
set source_dir=$(env.ACEROOT)\3rdParty\node_modules

rem a count that increases by one when record one folder
set /a unique_count=0
set subroot=dummy

if exist %STEP3_OUTPUT% (
del /Q %STEP3_OUTPUT%
)
mkdir %STEP3_OUTPUT%

if exist %STEP6_OUTPUT_FOLDER% (
del /Q %STEP6_OUTPUT_FOLDER%
)
mkdir %STEP6_OUTPUT_FOLDER%

if exist %dir_fragment% (
del %dir_fragment%
)

if exist %dir_fragment_2% (
del %dir_fragment_2%
)

if exist %resolve_conflict% (
del %resolve_conflict%
)

if exist %tmp_mapping% (
del %tmp_mapping%
)

if exist %ID_Name_mapping% (
del %ID_Name_mapping%
)

rem create new record file
echo #### > %resolve_conflict%

rem *************************************************************
rem       Recursive function call to create dir_fragment
rem *************************************************************
echo Creating dir_fragment...
rem add a header for formatting purpose
rem echo ^<?xml version="1.0" encoding="utf-8"?^> > %dir_fragment% 

call :recursive %NODE_MODULES% 3rdParty ProjectCentricServer 
rem delete temporary file
REM if exist %resolve_conflict% (
	REM del %resolve_conflict%
	REM )
rem *************************************************************
rem       Format the created direcotry fragment
rem *************************************************************
rem echo Formatting dir_fragment...
rem set xml_location=%script_location%\xmlstarlet-1.6.1
rem call %xml_location%\xml.exe format -t %dir_fragment% > %script_location%\temp2.txt
rem move %script_location%\temp2.txt %dir_fragment%

rem *************************************************************
rem Replace node_modules with %source_dir% in mapping file
rem *************************************************************
echo Replacing node_modules with $(var.node_modules) in mapping file
for /F "delims=" %%i in (%tmp_mapping%) do (
	set "line=%%i" 
	set "line=!line:&node_modules=&%source_dir%!"
	echo !line!
	) >> %ID_Name_mapping%
) 


REM if exist %tmp_mapping% (
	REM del %tmp_mapping%
	REM )
	
endlocal
echo script done
exit /b 0
pause



rem param1: folder to check, FULL PATH
rem param2: parent folder, NAME ONLY
rem param3: parent wix directory ID
:recursive
setlocal EnableDelayedExpansion
set /a unique_count+=1
set folder=%1
rem bare_folder is folder path starting with node_modules
call set bare_folder=%%folder:%THIRDPARTY%\=%%
set folder_name=%~nx1


rem remove dot(.), hyphen(-), white space( )
set DirectoryID=%folder_name%
set DirectoryID=!DirectoryID:.=!
set DirectoryID=!DirectoryID:-=!
set DirectoryID=!DirectoryID: =!
rem replace all node_modules to NM
set DirectoryID=!DirectoryID:node_modules=NM!

rem truncate ID to three characters
if "!DirectoryID!" NEQ "node_modules" (
	set DirectoryID=!DirectoryID:~0,3!
	)
 
rem actually this variable should be called ParentDirectoryID
set parent_folder_name=%3

set dir_destination=%subroot%
if "%parent_folder_name%" EQU "ProjectCentricServer" (
	set dir_destination=ProjectCentricServer
	)

if "%parent_folder_name%" EQU "NM" (
	set dir_destination=node_modules
	)
set dir_destination=!dir_destination:.=!
set dir_destination=!dir_destination:-=_!

set /a count=0
rem if folder name unique, the folder name is used as ID; otherwise ParentID + folder name is used as ID
rem record current folder
>nul find "!DirectoryID!" %resolve_conflict% && (
  set DirectoryID=%parent_folder_name%_%DirectoryID%%unique_count%
)

set DirectoryID=!DirectoryID:.=!
set DirectoryID=!DirectoryID:-=!
set DirectoryID=!DirectoryID: =!
rem replace all node_modules to NM
set DirectoryID=!DirectoryID:node_modules=NM!
		
echo ^<Directory Id="ACE_DIR_NM_!DirectoryID!" Name="%folder_name%"^>>>%dir_fragment%
echo !DirectoryID!>>%resolve_conflict%
rem echo to mapping file
echo ACE_DIR_NM_!DirectoryID!^&%bare_folder%>>%tmp_mapping%
rem record DirectoryID, Directory Name, ParentDirectoryID to DIR_install_frament_2.txt
echo ACE_DIR_NM_!DirectoryID!^&!folder_name!$ACE_DIR_NM_%3*!dir_destination!>>%dir_fragment_2%

set ParentDirectoryID=!DirectoryID!

rem loop all subfolders in current folder
for /F "delims=" %%G in ('dir /b /a:d %folder%') do (

	set dir_destination=!subroot!
	
    if "%ParentDirectoryID%" == "NM" (
		set dir_destination=node_modules
		set subroot=%%G
		
		set subfolder=%%G
		set subfolder=!subfolder:.=!
		set subfolder=!subfolder:-=_!
		
		rem create folder structure for step 6 create directory fragments
		set des=%STEP6_OUTPUT_FOLDER%\!subfolder!
		if exist !des! (
			del /Q !des!
			)
		mkdir !des!
		)

	set dir_destination=!dir_destination:.=!
	set dir_destination=!dir_destination:-=_!
	
	set sub_folder=%%G
	for /F "delims=" %%i in ('dir /b /a:d %folder%\!sub_folder!') do (
		set /a count+=1
	) >nul
	
	if !count! GTR 0 (
		call :recursive %folder%\!sub_folder! %folder_name% %ParentDirectoryID%
	) else (
	    set /a unique_count+=1
		set DirectoryID=%%G
		set DirectoryID=!DirectoryID:.=!
		set DirectoryID=!DirectoryID:-=!
		set DirectoryID=!DirectoryID: =!
		rem replace all node_modules to NM
        set DirectoryID=!DirectoryID:node_modules=NM!
		
		rem truncate ID to three characters
        if "!DirectoryID!" NEQ "node_modules" (
	        set DirectoryID=!DirectoryID:~0,3!
	        )
			
		>nul find "!DirectoryID!" %resolve_conflict% && (
		    rem in case content, contact, constrain under same folder
			set DirectoryID=%ParentDirectoryID%_%%G
		    ) 
		set DirectoryID=!DirectoryID:.=!
		set DirectoryID=!DirectoryID:-=!
		set DirectoryID=!DirectoryID: =!
		rem replace all node_modules to NM
		set DirectoryID=!DirectoryID:node_modules=NM!
		
	    echo ^<Directory Id="ACE_DIR_NM_!DirectoryID!" Name="!sub_folder!"^>>>%dir_fragment%
		echo !DirectoryID!>>%resolve_conflict%
		rem echo to mapping file
		echo ACE_DIR_NM_!DirectoryID!^&%bare_folder%\!sub_folder!>>%tmp_mapping%
		echo ^</Directory^>>>%dir_fragment%
		rem record DirectoryID, Directory Name, ParentDirectoryID to DIR_install_frament_2.txt
		echo ACE_DIR_NM_!DirectoryID!^&!sub_folder!$ACE_DIR_NM_%ParentDirectoryID%*!dir_destination!>>%dir_fragment_2%
	)
)

endlocal
echo ^</Directory^> >> %dir_fragment%
exit /b 0

