md %1_objs
cd %1_objs
for /f %%i in ('link -lib /list %1.lib') do link -lib /extract:%%i %1.lib
for %%i in (*.obj) do upcf %%i
sigmake -n"%1.lib" *.pat %1.sig
pause
if exist %1.exc for %%i in (%1.exc) do find /v ";" %%i > abc.exc 
if exist %1.exc for %%i in (%1.exc) do > abc.exc more +2 "%%i"
copy abc.exc %1.exc
del abc.exc
sigmake -n"%1.lib" *.pat %1.sig
copy %1.sig ..\%1.sig
cd ..
del %1_objs /s /q
rd %1_objs