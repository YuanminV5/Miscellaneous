rem create folder structure and DIRECTORY ID - FOLDER NAMW mapping
@echo off
setlocal EnableDelayedExpansion
set script_location=C:\Users\weiyu\Desktop\script


set "msmProjects=U:\Schematics\develop\install\installProjects\wixMsmProjects\ProjectCentric\node_modules"

For /R %msmProjects% %%G in (*.bat) do (
	echo Building project %%G
	call %%G
    )
	
endlocal
exit /b 0

