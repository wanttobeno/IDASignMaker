::@ECHO OFF & CD /D %~DP0 & TITLE IDA API 特征码生成工具
::拷贝 D:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSPDB60.DLL
::到 D:\Program Files\Microsoft Visual Studio\VC98\Bin\MSPDB60.DLL
set VcBinDir=D:\Program Files\Microsoft Visual Studio\VC98\Bin\


set Lib_Name=LIBC
set Lib_file="D:\Program Files\Microsoft Visual Studio\VC98\Lib\LIBC.LIB"

::set Lib_Name=cmstub
::set Lib_file=E:\BaiduYunDownload\Flair\cmstub.lib

if not exist "%~DP0tmp" mkdir "%~DP0tmp"


::1.遍历lib中所有obj的文件
"%VcBinDir%lib.exe" /list %Lib_file% >"%~DP0tmp\%Lib_Name%_obj.txt"

if not exist "%~DP0tmp\%Lib_Name%_obj.txt" goto end

::2.lib.exe提取obj文件出来.
::"%VcBinDir%lib.exe" /extract:build\intel\st_obj\_ctype.obj "D:\Program Files\Microsoft Visual Studio\VC98\Lib\LIBC.LIB" 
for /f "delims==" %%a in ('type "%~DP0tmp\%Lib_Name%_obj.txt"') do  (
  :: 这里必须加""
  if "%%~xa" == ".obj" (
    ::echo Y
    echo %%a
    "%VcBinDir%lib.exe" /extract:%%a %Lib_file% /OUT:"%~DP0tmp\%%~na%%~xa"
  ) else (
    echo N
  )
)

:: 3.使用pcf.exe(特征提取工具),生成.pat文件
::for %%i in (*.obj) do "%~DP0pcf.exe" "%%i"
:: 合并为一个文件
::for %%i in ("%~DP0tmp\*.obj") do "%~DP0pcf.exe" -a "%~DP0%tmp\Lib_Name%.pat"
cd "%~DP0tmp\"
::for %%i in (*.obj) do "%~DP0pcf.exe" -a "%~DP0%Lib_Name%.pat"
::for %%i in (*.obj) do "%~DP0pcf.exe" -s "%%i"
for %%i in (*.obj) do "%~DP0pcf.exe" -s "%%i"


:: 4.生成.sig文件.
if not exist  "%~DP0sig\" mkdir "%~DP0sig\"

"%~DP0bin\sigmake.exe" -n%Lib_file% *.pat "%~DP0sig\%Lib_Name%.sig"


if exist "%~DP0sig\%Lib_Name%.sig" (
  echo "sig文件生成成功"
)else if exist "%~DP0sig\%Lib_Name%..err" (
  echo "EXC (.exc) 文件创建，存在冲突"
)
else (
  echo "sig文件生成失败"
)

:: 5.清空临时文件
::for %%i in (*.obj) do del /q %%i
::for %%i in (*.pat) do del /q %%i

:: 6.把sig文件拷贝到IDA sig的目录，在IDA shift+F5 已经可以识别了

:end
echo Finish!
pause



