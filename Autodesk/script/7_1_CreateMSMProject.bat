
@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script
set wxs_files=%script_location%\4_output_AfterFormatting

set uuidgen="C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\x64\uuidgen.exe"
set uuid=%script_location%\7_1_output_uuid.txt
rem %STEP7_1_OUTPUT_FOLDER% for step 7_2 to put files
set STEP7_1_OUTPUT_FOLDER=%script_location%\7_1_output_NM_msmProject

set msmProject_list=%script_location%\7_1_output_msmProject_list.txt


if exist %msmProject_list% (
	del %msmProject_list%
	)
	
if exist %uuid% (
	del %uuid%
	)

if exist %STEP7_1_OUTPUT_FOLDER% (
RD /S /Q %STEP7_1_OUTPUT_FOLDER%
)
mkdir %STEP7_1_OUTPUT_FOLDER%

rem currently we have 13 folders to process, each needs two GUIDs
call %uuidgen% /c /n26>%uuid%

rem create folder at destination 
for /R %wxs_files% %%G in ("*.wxs") do (

	set wxsfilename=%%~nG
	set msmProjectFolder=%STEP7_1_OUTPUT_FOLDER%\!wxsfilename!
	rem create folder for each wxs file. folder name is same as file name
	echo Creating msm project folder !msmProjectFolder! for %%~nxG ...
	cd /d %STEP7_1_OUTPUT_FOLDER%
	set folder_name=%%~nG

	if not exist !msmProjectFolder! (
		mkdir !msmProjectFolder!
		)

	echo !wxsfilename!>>%msmProject_list%
	)
		
	
	
	
cd /d %script_location%
endlocal
exit /b 0

:CreateAMake project_name
setlocal
echo ^@echo off>> amake.bat
echo setlocal>> amake.bat
echo set PROJECT_ROOT=^%^~dp0>> amake.bat
echo set PROJECT_ROOT=%PROJECT_ROOT%'>> amake.bat
echo set PROJECT_ROOT=%PROJECT_ROOT:\'=%>> amake.bat
echo set PROJECT_ROOT=%PROJECT_ROOT:'=%>> amake.bat
echo.>> amake.bat
echo set PROJECT_FILE=%PROJECT_ROOT%\msmProject.wxs>> amake.bat
echo set PROJECT_NAME=AeData>> amake.bat
echo set ARCH=%ACPLATFORM%>> amake.bat
echo set TARGET=build>> amake.bat
echo if /i "%1" == "clean" set TARGET=clean>> amake.bat
echo set MSM_BUILD_FILE=%ACE_WixProjects%\wixMsmProjects\_common\MergeModule.build>> amake.bat
echo.>> amake.bat
echo call WITools CleanEnvVars>> amake.bat
echo call %TBIN%\nant-0.92\NAnt.exe -buildfile:"%MSM_BUILD_FILE%" -D:PROJECT_NAME="%PROJECT_NAME%" -D:ARCH=%ARCH% -D:PROJECT_FILE="%PROJECT_FILE%" -D:OUTDIR="%ACE_OUTPUTMSMDIR%" -D:OBJDIR="%ACE_OUTPUTMSMOBJDIR%" -D:PDBDIR="%ACE_OUTPUTMSMDIR%" %TARGET%  || exit /b 1>> amake.bat
echo endlocal
echo exit /b 0
endlocal
exit /b 0

:CreateMSMProjectWXI project_name
exit /b 0


:CreateMSMProjectWXS project_name
exit /b 0

:GetGuid
cd /d %uuidgen%
echo %uuidgen%
for /f %%i in ('Uuidgen.Exe /c') do set _GUID=%%i
set %1=%_GUID%
exit /b 0

pause