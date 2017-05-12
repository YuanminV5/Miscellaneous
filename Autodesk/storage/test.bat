@echo off
SET _test=123456789abcdef0

::Extract only the first 5 characters

 SET _result=%_test:~0,5%
 ECHO %_result%          =12345

::Skip 7 characters and then extract the next 5

 SET _result=%_test:~7,5%
 ECHO %_result%          =89abc

::Skip 7 characters and then extract everything else

 SET _result=%_test:~7%
 ECHO %_result%          =89abcdef0

::Extract only the last 7 characters

 SET _result=%_test:~-7%
 ECHO %_result%          =abcdef0

::Extract everything BUT the last 7 characters

 SET _result=%_test:~0,-7%
 ECHO %_result%          =123456789

::Extract between 7 from the front and 5 from the end

 SET _result=%_test:~7,-5%
 ECHO %_result%          =89ab

::Go back 7 from the end then extract 5 towards the end

 SET _result=%_test:~-7,5%
 ECHO %_result%          =abcde

::Extract between 7 from the end and 5 from the end

SET _result=%_test:~-7,-5%
ECHO %_result%        =ab

cd /D C:\Users\weiyu\Desktop
for %%G in (*.*) do (
set attribs = %%~aG
if %attribs:~1% == "1" (
echo "This is symbol link"
) else (
echo %%G
echo "this is not symbol link" 
)
)

SETLOCAL enabledelayedexpansion
set myvar="C:\my music & videos"
@echo !myvar! name outside quotes.

set "myvar2=C:\my music & videos"
@echo !myvar2! name inside quote.
endlocal






pause