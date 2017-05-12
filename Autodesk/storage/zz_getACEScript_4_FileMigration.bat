
@echo off
echo. 
set current="%~dp0"

set "Migration_Testing=C:\Common Root\File Migration Testing" 
set target_version_1=2007
set target_version_2=2013
set "source=%Migration_Testing%"
set lib_name=Libs
set output_script=C:\Users\%USERNAME%\desktop\FRT_file_migration_script


if exist %output_script% (
del /Q %output_script%
)

IF not EXIST "%source%" (
	echo could not find test file folder at: %source%
	echo please copy test file folder to local or update value of "Migration_Testing" in this the batch file
	GOTO:EOF
	)

echo Predefined test file folder is located at: %source%
echo. 

echo Generated scripts will be located at: %output_script%
echo. 
mkdir %output_script%

echo Create script for each folder in test file folder
echo.	
Set /P option="Please choose target version(2007, 2013):" 
If /i "%option%"=="2007" goto normal
If /i "%option%"=="2013" goto normal

echo.
goto :sub_error
exit /b 0

:normal
echo.
setlocal EnableDelayedExpansion
echo You choose to migration file from current version to "%option%"
echo. 
for /D %%G in ("%source%\*") do (
	echo Creating script for folder %%~nG ...
	echo.
	
	cd /D "%%G"

	for /r %%a in (*.dwg) do ( 
		echo open
		echo "%%a"
		echo saveas
		echo %option%
		echo.
		if /i "%option%"=="2007" echo Y
		echo close
		echo open
		echo "%%a"
		echo qsave
		echo close
	    echo open
		echo "%%a"
		echo close
		) >> %output_script%\%%~nxG.scr
	)	
echo Scripts generation done. Find them in folder %output_script%
echo.
endlocal

cd /D %output_script%
echo. 
echo Create FileMigration.scr...
for /r %%a in (*.scr) do ( 
	echo script
	echo "%%a"
	) >> FileMigration.txt
ren FileMigration.txt FileMigration.scr
echo File FileMigration.scr created

cd /D %current%
exit /b 0




:sub_error
echo.
echo you choose nothing or input wrong option! Bye~
echo.
if exist %output_script% (
del /Q %output_script%
)
cd /D %current%
exit /b 0
