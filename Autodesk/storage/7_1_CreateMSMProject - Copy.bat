
@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script
set wix_files=d:\CodeSite\ACE\E-Branches\B\AcadE\Install\installFragments\Acade\ProjectCentric
set msm_folder=d:\CodeSite\ACE\E-Branches\B\AcadE\Install\installProjects\wixMsmProjects\ProjectCentric
set uuidgen="C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\x64\uuidgen.exe"
set uuid=%script_location%\7_1_output_uuid.txt
rem %STEP7_1_OUTPUT_FOLDER% for step 7_2 to put files
set STEP7_1_OUTPUT_FOLDER=%script_location%\7_1_output_NM_msmProject

set msmProject=%script_location%\msmProject.txt
if not exist %msm_folder% (
	mkdir %msm_folder%
	)

if exist %msmProject% (
	del %msmProject%
	)
	
if exist %uuid% (
	del %uuid%
	)

if exist %STEP7_1_OUTPUT_FOLDER% (
del /Q %STEP7_1_OUTPUT_FOLDER%
)
mkdir %STEP7_2_OUTPUT_FOLDER%

rem currently we have 13 folders to process, each needs two GUIDs
call %uuidgen% /c /n26>%uuid%

rem create folder at destination 
for /R %wix_files% %%G in ("*.wxs") do (

	set path=%%~dpG
	set path=!path:%wix_files%=%msm_folder%!
	set path=!path!

	cd /d !path!
	set folder_name=%%~nG
	rem set folder_name=!folder_name:-=_!
	echo create wix msm project !folder_name!
	if not exist !folder_name! (
		mkdir !folder_name!
		)

	echo !path!!folder_name!^&!folder_name!>>%msmProject%  
	if not exist %%~nG\amake.bat (
		rem call :CreateAMake %%~nG
		)
	
	if not exist %%~nG\msmProject.wxi (
		rem call :CreateMSMProjectWXI %%~nG
		)

	if not exist %%~nG\msmProject.wxs (
		rem call :CreateMSMProjectWXS %%~nG
		)	

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