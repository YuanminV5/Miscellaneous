
@echo off
echo. 
set current="%~dp0"

rem Note ACADE_public doesn't contain "Libs"
set "ACADE_public=C:\Users\Public\Documents\Autodesk\Autocad Electrical 2016" 
set "source=%ACADE_public%\Libs"
set lib_name=Libs
set output_script=C:\Users\%USERNAME%\desktop\FRT_symbol_lib_script


if exist %output_script% (
del /Q %output_script%
)

IF not EXIST "%source%" (
	echo could not find symbol library at: %source%
	echo please install symbol library or update value of "ACADE_public" in this the batch file
	GOTO:EOF
	)

echo Predefined symbol library is located at: %source%
echo. 

echo Generated scripts will be located at: %output_script%
echo. 
mkdir %output_script%

echo Create script for each folder in library or put everything in one script?
echo.	
Set /P option="Please choose(1 = one_by_one; 2 = all_in_one; 3 = both):" || (Set option=0)
If /i "%option%"=="1" goto one_by_one
If /i "%option%"=="2" goto all_in_one
If /i "%option%"=="3" goto both
echo.
goto :sub_error
exit /b 0

:one_by_one
setlocal EnableDelayedExpansion
echo You choose to create a script for each folder under symbol library %source%
echo. 
for /D %%G in ("%source%\*") do (
	echo Creating script for folder %%~nG ...
	echo.
	
	cd /D "%%G"
	rem set name=%%~nxG

	for /r %%a in (*.dwg) do ( 
		echo _.OPEN "%%a"
		echo _.CLOSE
		) >> %output_script%\%%~nxG.scr
	)	
echo Scripts generation done. Find them in folder %output_script%
echo.
endlocal
cd /D %current%
exit /b 0

:all_in_one
echo You choose to create a script for each folder under symbol library %source%
echo Creating script ...
echo. 
CD /D %source%
for /r %%a in (*.dwg) do ( (echo _.OPEN "%%a"&& echo _.CLOSE) ) >> %output_script%\batch.scr 
echo Script generation done. Find "batch.scr" in folder %output_script%
cd /D %current%
exit /b 0


:both
echo You want all! It may take some time ...'
echo.
call :one_by_one
echo.
call :all_in_one
cd /D %current%
exit /b 0

:sub_error
echo you choose nothing or input wrong option! Bye~
echo.
if exist %output_script% (
del /Q %output_script%
)
cd /D %current%
exit /b 0
