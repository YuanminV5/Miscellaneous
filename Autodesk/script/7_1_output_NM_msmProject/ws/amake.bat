@echo off
setlocal
set PROJECT_ROOT=%~dp0
set PROJECT_ROOT=%PROJECT_ROOT%'
set PROJECT_ROOT=%PROJECT_ROOT:\'=%
set PROJECT_ROOT=%PROJECT_ROOT:'=%

set PROJECT_FILE=%PROJECT_ROOT%\msmProject.wxs
set PROJECT_NAME=ws
set ARCH=%ACPLATFORM%
set TARGET=build
if /i "%1" == "clean" set TARGET=clean
set MSM_BUILD_FILE=%ACE_WixProjects%\wixMsmProjects\_common\MergeModule.build

call WITools CleanEnvVars
call %TBIN%\nant-0.92\NAnt.exe -buildfile:"%MSM_BUILD_FILE%" -D:PROJECT_NAME="%PROJECT_NAME%" -D:ARCH=%ARCH% -D:PROJECT_FILE="%PROJECT_FILE%" -D:OUTDIR="%ACE_OUTPUTMSMDIR%" -D:OBJDIR="%ACE_OUTPUTMSMOBJDIR%" -D:PDBDIR="%ACE_OUTPUTMSMDIR%" %TARGET%  || exit /b 1
endlocal
exit /b 0
