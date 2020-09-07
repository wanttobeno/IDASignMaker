@ECHO OFF & CD /D %~DP0 & TITLE IDA API 特征码生成工具


for %%a in (%*) do set /a num+=1
if defined num (echo 调用了 %num% 个参数) else (
 echo 没有调用任何参数,请输入lib文件的名字
 goto end
)

if not exist %1_objs md %1_objs

cd %1_objs

echo .
echo  copy lib files to %1_objs

pause

for /f %%i in ('"%~DP0vc_tools\link.exe" -lib /list %1.lib') do "%~DP0vc_tools\link.exe" -lib /extract:%%i %1.lib
for %%i in (*.obj) do "%~DP0pcf.exe" -s %%i
"%~DP0bin\sigmake.exe" -n"vc6%1.lib" *.pat %1.sig
if exist %1.exc for %%i in (%1.exc) do find /v ";" %%i > abc.exc 
if exist %1.exc for %%i in (%1.exc) do > abc.exc more +2 "%%i"
copy abc.exc %1.exc
del abc.exc
"%~DP0bin\sigmake.exe" -n"%1.lib" *.pat %1.sig
copy %1.sig ..\%1.sig
del %1_objs /q *.pat
del %1_objs /q *.obj
cd ..
::del %1_objs /s /q
::rd %1_objs

:end
 echo Finish!
pause