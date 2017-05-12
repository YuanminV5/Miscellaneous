@echo off
echo.
echo Setting up VRM development enviroment...
set VRMDir=%~dp0

rem remove trailing slash
set VRMDir=%VRMDir%
set VRMDir=%VRMDir%#
set VRMDir=%VRMDir:\#=%

set generic=%VRMDir%\..\generic
set RCPPSDK=%VRMDir%\..\RCPPSDK
set VCSLIB=%VRMDir%\..\VCSLIB

if not exist %generic% (
echo External component "generic" is not in place
goto :eof
)

if not exist %RCPPSDK% (
echo RCPPSDK is not in place
goto :eof
)

if not exist %VCSLIB% (
echo VCSLIB is not in place
goto :eof
)

echo VRM directory is %VRMDir%
echo.

set VS_x32=%VRMDir%\VS_x32
echo Visual Studio Solution folder created: %VS_x32% 
set VS_x64=%VRMDir%\VS_x64
echo Visual Studio Solution folder created: %VS_x64% 

rem create x32 and x64 solution folder
if not exist %VS_x32% (
mkdir "%VS_x32%"
)

if not exist %VS_x64% (
mkdir "%VS_x64%"
)

rem make x32 solution
echo.
cd %VS_x32%
echo Creating Visual Studio x32 project...
cmake ..

rem copy additional dlls
echo.
echo copy additional dlls...
copy /Y %generic%\Release_Win32\vj_generic.dll %VS_x32%
copy /Y %RCPPSDK%\bin\rcpp4.dll %VS_x32%
copy /Y %RCPPSDK%\bin\rcpp4d.dll %VS_x32%

rem make x64 solution
echo.
cd %VS_x64%
echo Creating Visual Studio x64 project...
cmake -G "Visual Studio 14 2015 Win64" ..

rem copy additional dlls
echo.
echo copy additional dlls...
copy /Y %generic%\Release_x64\vj_generic.dll %VS_x64%
copy /Y %RCPPSDK%\bin\rcpp4x64.dll %VS_x64%
copy /Y %RCPPSDK%\bin\rcpp4x64d.dll %VS_x64%

rem optional: license registration
rem copy ./vrm/src/license/vrm.lic to ./vrm; will be handled by cmake
echo.
echo copy license file...
copy /Y %VRMDir%\src\license\vrm.lic %VRMDir%
rem following command should be run in administrator mode and only needs to be run once
rem regsvr32 ./vrm/src/license/LicProtector.dll
rem optional

echo. 
cd %VRMDir%
echo Enviroment setup done.

echo. 
echo What's next?
echo ****************************************************************************
echo 1. Open VRM solution, build the whole solution
echo 2. Set rms as startup project, set command line argument '--https-port 8443 --http-port 8000"(coz iis occupy 443 and 800, and note, http is disabled), F5 to start
echo 2. In browser, type and enter https://localhost:8443/vrmcockpit/index_nochunk.html
echo ****************************************************************************
exit /b 0

:eof
pause
exit /b 0
