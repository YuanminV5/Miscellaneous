
@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script
set final=%script_location%\3_output_final
set beautiful=%script_location%\4_output_AfterFormatting
set file_list_for_step5=%script_location%\4_output_for_step5_creat_include.txt
set STEP5_OUTPUT_FOLDER=%script_location%\5_output_wix_include_dir
if exist %beautiful% (
del /Q "%beautiful%"
)
mkdir %beautiful%

if exist %STEP5_OUTPUT_FOLDER% (
del /Q "%STEP5_OUTPUT_FOLDER%"
)
mkdir %STEP5_OUTPUT_FOLDER%

if exist %file_list_for_step5% (
del "%file_list_for_step5%"
)

set xml_location=%script_location%\xmlstarlet-1.6.1

for  %%G in ("%final%\*.wxs") do (
	echo Beautifying %%G...
	set name=%%~nG
	call %xml_location%\xml.exe format -t %%G > %beautiful%\!name!.wxs
	)

rem *************************************************************
rem List down all beautified wxs files for step 5
rem *************************************************************
echo Listing down all processed wxs files to file called file_list.txt ...
dir /b /s %beautiful% > %file_list_for_step5%
pause