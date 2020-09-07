@ECHO OFF & CD /D %~DP0 & TITLE IDA API 特征码生成工具
::拷贝 D:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSPDB60.DLL
::到 D:\Program Files\Microsoft Visual Studio\VC98\Bin\MSPDB60.DLL
set VcBinDir=D:\Program Files\Microsoft Visual Studio\VC98\Bin\


set Lib_Name=LIBC
set Lib_file="D:\Program Files\Microsoft Visual Studio\VC98\Lib\LIBC.LIB"

::set Lib_Name=cmstub
::set Lib_file=E:\BaiduYunDownload\Flair\cmstub.lib

for %%a in (%*) do set /a num+=1
if defined num (echo 调用了 %num% 个参数) else (
 echo 没有调用任何参数,请输入lib文件的完整地址
 goto end
)

if not exist %1 (
  ehco "输入的文件不存在"
  goto end
)

:: 直接从lib生成整个pat文件，默认保存在lib下
"%~DP0pcf.exe" -s %1 

if not exist "%~d1%~p1%~n1.pat" (
  echo #### 生成pat文件失败！
  goto end
) else echo ### 生成pat文件成功！

:: 生成.sig文件.
if not exist  "%~DP0sig\" mkdir "%~DP0sig\"

"%~DP0bin\sigmake.exe" -n%1 "%~d1%~p1%~n1.pat" "%~DP0sig\%~n1.sig"

if exist "%~DP0sig\%~n1.sig" (
  echo ### sig文件生成成功
)else if exist "%~DP0sig\%~n1..err" (
  echo #### EXC (.exc) 文件创建，存在冲突
) else  echo #### sig文件生成失败

:: 5.清空临时文件
::for %%i in (*.obj) do del /q %%i
::for %%i in (*.pat) do del /q %%i

:: 6.把sig文件拷贝到IDA sig的目录，在IDA shift+F5 已经可以识别了

goto end

:end
echo Finish!

pause



