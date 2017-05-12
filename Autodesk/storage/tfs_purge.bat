@echo off
setlocal EnableDelayedExpansion
set "VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC"
set target_folder=%~f0
echo Running tfs power tool in folder %target_folder%...

for /F "delims=" %%G in ('dir /a:d %target_folder%') do (
	echo find directory %%G
	set attribs=%~aG
	echo folder attribs is !attribs!
	if "%attribs:~-1%" == "l" echo This is a symlink/junction
)
endlocal
REM tf workspaces
REM Collection: http://tfs.autodesk.com:8080/tfs/acadcollection
REM Workspace    Owner       Computer   Comment
REM ------------ ----------- ---------- -------------------------------------------
REM SHA8R4JH32   Yuanmin Wei SHA8R4JH32
REM SHA8R4JH32_1 Yuanmin Wei SHA8R4JH32

REM tf workspaces /collection:http://tfs.autodesk.com:8080/tfs/acadcollection
REM Collection: http://tfs.autodesk.com:8080/tfs/acadcollection
REM Workspace    Owner       Computer   Comment
REM ------------ ----------- ---------- -------------------------------------------
REM SHA8R4JH32   Yuanmin Wei SHA8R4JH32
REM SHA8R4JH32_1 Yuanmin Wei SHA8R4JH32

tfpt schorch 