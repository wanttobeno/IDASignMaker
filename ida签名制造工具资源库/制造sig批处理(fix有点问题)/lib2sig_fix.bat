@echo off&setlocal enabledelayedexpansion
if exist %1.exc for /f "delims=" %%i in (%1.exc) do set "line=%%i"&set "next_line="&call :process_perline !line! !next_line! %1.exc
pause
:process_perline
if %1 == "\n" echo -%1>>%3
goto :eof