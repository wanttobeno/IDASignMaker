// SigMaker.cpp : 定义控制台应用程序的入口点。
//


#include"SigMaker.h"
#include<io.h>

#include "windows.h"

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>
using namespace std;


HANDLE hReadPipeCmd;
HANDLE hWritePipeCmd;
HANDLE hReadPipeShell;
HANDLE hWritePipeShell;
HANDLE hProcessHandle;
char readBuff[4096] = "\x00";
char writeBuff[256] = "\x00";
TCHAR file_path[_MAX_PATH] = { 0 };
vector<string> files_name;
TCHAR vcvarsall_command[_MAX_PATH] = { 0 };
TCHAR vcvarsall_path[_MAX_PATH] = { 0 };
TCHAR vcvarsall_para[_MAX_PATH] = { 0 };
TCHAR file_path_driver[MAX_PATH] = { 0 };

ULONG ps_start = NULL;
ULONG ps_end = NULL;
PCHAR vcvarsall_path_fix = NULL;
PCHAR file_path_fix = NULL;
LPSTR new_build_arg[90] = { 0 };

void fn()
{
	printf("xxxxx\n");
}
int main(int argc, char *argv[])
{

	INT option = 0;


	HANDLE hw = NULL;


	

	
	
		
		SetEvent(&hw);
	while (true)
	{
		setvbuf(stdin, NULL, _IONBF, 0);
		//%comspec% /k ""D:\Program Files(x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"" x86
		_tprintf("*****************************************************************\n"
			     "   Welcome to SigMaker2.0 Author:Mockingjay QQgroup:493622016\n"
			     "*****************************************************************\n"
				 "\r\nPlease choose option [0:build only] [1:make sig only] [2:fix pat only] [3:make sig with no pcf]\n"
				 );
		_tscanf_s("%d", &option, 4);
		switch (option)
		{
		case 0:
			build_only();
			break;
		case 1:
			makesig_only();
			break;
		case 2:
			fixpat_only();
			break;
		case 3:
			makesig_no_pcf();
			break;
		default:
			_tprintf("invalid input,please check it and input again!\n");
			continue;
        }
		memset(file_path, 0, _MAX_PATH);
	}

	system("pause");
	return 0;

}
void build_only()
{


	
	for (int i = 0; i < 90; i++)
	{
		new_build_arg[i] = (LPSTR)malloc(32);
		memset(new_build_arg[i], 0, 32);
	}
	LPSTR old_build_arg[90] = { "/O1", "/O2", "/Od,", "/Ox", "/Ob0", "/Ob1", "/Ob2", "/Oi",
		"/Os", "/Ot", "/Oy", "/Oy-", "/favor:blend", "/favor:ATOM", "/favor:AMD64", "/favor:INTEL64",
		"/arch:IA32", "/arch:SSE", "/arch:SSE2", "/arch:AVX", "/arch:AVX2", "/EHa", "/EHs", "/EHc",
		"/EHr", "/EH-", "/fp:precise", "/fp:except", "/fp:except-", "/fp:fast", "/fp:strict", "/GA",
		"/Gd", "/Gr", "/Gz", "/Gv", "/Ge", "/GF", "/GF-", "/Gh",
		"/GH", "/sdl", "/sdl-", "/Gm", "/Gm-", "/GR", "/GR-", "/GL",
		"/GS", "/GS-", "/Gs0", "/GT", "/guard:cf", "/guard:cf-", "/MD", "/MDd",
		"/MT", "/MTd", "/LD", "/LDd", "/Gw", "/Gw-", "/GX", "/Gy",
		"/Gy-", "/GZ", "/TC", "/TP", "/homeparams", "/hotpatch", "/Qfast_transcendentals", "/QIfist",
		"/Qimprecise_fwaits", "/Qpar", "/Qpar-report:1", "/Qpar-report:2", "/Qsafe_fp_loads ", "/Qvec-report:1", "/Qvec-report:2", "/RTC1",
		"/RTCs", "/RTCu", "/RTCc", "/Zp1", "/Zp2", "/Zp4", "/Zp8", "/Zp16",
		"/volatile:iso", "/volatile:ms"
	};

	_tprintf(
		
		">>>>You choose build only!<<<<\n");
	setting_init();
	init_pipe(vcvarsall_para);
	cmd_input(file_path_driver);
	memset(file_path_driver, 0, _MAX_PATH);
	strcpy_s(file_path_driver, _MAX_PATH, "cd ");
	strcat_s(file_path_driver, _MAX_PATH, (PCHAR)(file_path));
	cmd_input(file_path_driver);
	cmd_input("mkdir lib");

	get_all_comb(old_build_arg, new_build_arg);

    for (int i = 0; i < 90; i++)
	{
		free(new_build_arg[i]);
	}

	_tprintf(
		
		">>>>build finished!<<<<\n"
		);
}
void setting_init()
{
	_tprintf(
		"*****************************************************************\n"
		"         Tip:\n"
		"         First,find your virtual studio command line\n"
		"         Second,choose an appropriate one\n"
		"         Third,right key view properties\n"
		"         Last,copy the data in the target column under the Shortcut tab\n"
		"*****************************************************************\n");
	while (true)
	{
		setvbuf(stdin, NULL, _IONBF, 0);
		_tprintf("Please enter the data you have just copied:\n"
			"Tip:Right click on the upper left corner of the icon to select edit and click paste\n");
		_tscanf_s("%[^\n]", vcvarsall_command, _MAX_PATH);
		ps_start = (ULONG)strchr(vcvarsall_command, '/');
		if (ps_start)
			strcpy_s(vcvarsall_para, _MAX_PATH, (PCHAR)(ps_start));
		else
		{

			memset(vcvarsall_command, _MAX_PATH, 0);
			_tprintf("invalid input,please check it and input again!\n");
			continue;
		}


		setvbuf(stdin, NULL, _IONBF, 0);
		_tprintf("Please enter the path of src[0] lib[1] pat[2 or 3] file:\n");
		_tscanf_s("%[^\n]", file_path, _MAX_PATH);

		//if (!strstr(file_path, "lua") || !strstr(file_path, "src"))
		//{
		//	memset(vcvarsall_command, _MAX_PATH, 0);
		//	memset(vcvarsall_para, _MAX_PATH, 0);
		//	//memset(vcvarsall_path, _MAX_PATH, 0);
		//	memset(file_path, _MAX_PATH, 0);
		//	/*free(vcvarsall_path_fix);
		//	vcvarsall_path_fix = NULL;*/
		//	ps_start = 0;
		//	//ps_end = 0;
		//	_tprintf("invalid input,please check it and input again!\n");
		//	continue;
		//}


		strncpy_s(file_path_driver, _MAX_PATH, file_path, 2);
		break;
	}
}
void makesig_only()
{
	string name;
	_tprintf(
		
		">>>>You choose makesig only!<<<<\n"
		);
	
	if (strlen(file_path) == 0)
	{
		setting_init();
		init_pipe(vcvarsall_para);
		cmd_input(file_path_driver);
		memset(file_path_driver, 0, _MAX_PATH);
		strcpy_s(file_path_driver, _MAX_PATH, "cd ");
		strcat_s(file_path_driver, _MAX_PATH, (PCHAR)(file_path));
		cmd_input(file_path_driver);

	}
	//strcat_s(file_path_driver, _MAX_PATH, (PCHAR)"\\lib");
	
	cmd_input("md sig");
	cmd_input("md error_lib");
	cmd_input("md error_exc");
	cmd_input("md error_pat");

	getFiles(file_path, files_name,".lib");
	for (size_t i = 0; i < files_name.size(); i++) 
	{

		name = files_name[i];
		process_perfile(name,false);
		
	}
	_tprintf(
		
		">>>>make sig finished!<<<<\n");
}
void fixpat_only()
{
	TCHAR name[MAX_PATH] = {0};
	_tprintf(

		">>>>You choose fixpat only!<<<<\n");
    
	if (strlen(file_path) == 0)
	{
		setvbuf(stdin, NULL, _IONBF, 0);
		_tprintf("Please enter the file name be fixed!\n");
		_tscanf_s("%[^\n]", name, _MAX_PATH);

		setting_init();
		init_pipe(vcvarsall_para);
		cmd_input(file_path_driver);
		memset(file_path_driver, 0, _MAX_PATH);
		strcpy_s(file_path_driver, _MAX_PATH, "cd ");
		strcat_s(file_path_driver, _MAX_PATH, (PCHAR)(file_path));
		cmd_input(file_path_driver);

		fix_pat(name,true);
	}

	_tprintf(
		
		">>>>fix pat finished!<<<<\n");
}
void process_perfile(string name, bool no_pcf)
{
	fstream _file;

	string nm = name;
	string command = "copy ";
	nm = nm + "_objs";
	command = "md " + nm ;
	cmd_input((LPSTR)command.c_str());

	if (no_pcf)
	{
		command = "copy " + name + ".pat " + nm;
		cmd_input((LPSTR)command.c_str());
		command = "cd " + nm;
		cmd_input((LPSTR)command.c_str());
		

	}
	else
	{
		command = "copy " + name + ".lib " + nm;
		cmd_input((LPSTR)command.c_str());
		command = "cd " + nm;
		cmd_input((LPSTR)command.c_str());
		command = "pcf " + name + ".lib " + name + ".pat";
		cmd_input((LPSTR)command.c_str());

	}
	for (int k = 0; k < 2; k++)
	{
		command = "sigmake -n\"" + name + "\" " + name + ".pat " + name + ".sig";
		cmd_input((LPSTR)command.c_str());

		command = file_path;
		command = command + "\\" + nm+"\\"+name + ".sig";

		command = "cd ..";
		cmd_input((LPSTR)command.c_str());
		/*_file.clear();
		_file.open(command, ios::in);*/
	/*	hFind = CreateFile(command.c_str(), GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		*/
		command = file_path;
		command = command + "\\" + nm + "\\" + name + ".sig";

		_file.clear();
		_file.open(command.c_str(), ios::in);
		
		command = "cd " + nm;
		cmd_input((LPSTR)command.c_str());
		_tprintf("_file :%x\n", _file.good());
		if (_file.good())
		{
	
			if (k)
			{

				command = "copy " + name + ".sig " + "..\\sig";
				cmd_input((LPSTR)command.c_str());


	            command = "copy " + name + ".lib " + "..\\error_lib";
				cmd_input((LPSTR)command.c_str());
				command = "copy " + name + ".exc " + "..\\error_exc";
				cmd_input((LPSTR)command.c_str());
				command = "copy " + name + ".pat " + "..\\error_pat";
				cmd_input((LPSTR)command.c_str());
			}
			else
			{

				command = "copy " + name + ".sig " + "..\\sig";
				cmd_input((LPSTR)command.c_str());
			}
			
			_file.close();
			break;
		}
		else if (k)
		{
			
		    command = "copy " + name + ".lib " + "..\\error_lib";
		    cmd_input((LPSTR)command.c_str());
		    command = "copy " + name + ".exc " + "..\\error_exc";
		    cmd_input((LPSTR)command.c_str());
		    command = "copy " + name + ".pat " + "..\\error_pat";
		    cmd_input((LPSTR)command.c_str());
		   

		}
		else
            fix_pat((LPSTR)name.c_str(),false);



	}

	command = "cd ..";
	cmd_input((LPSTR)command.c_str());
	command = "del " + name + "_objs /s /q";
	cmd_input((LPSTR)command.c_str());
	command = "rd " + name + "_objs";
	cmd_input((LPSTR)command.c_str());
}

void fix_pat(LPSTR name, bool fix_only)
{
	int first = 0;
	FILE *in_fp = NULL;
	FILE *out_fp = NULL;
	errno_t in_err = 0;
	errno_t out_err = 0;
	CHAR strLine[_MAX_PATH] = { 0 };
	CHAR str_new_line[_MAX_PATH] = { 0 };
	CHAR command[_MAX_PATH] = { 0 };
	CHAR infile_name[_MAX_PATH] = { 0 };
	CHAR outfile_name[_MAX_PATH] = {0};
	CHAR file_all_path[_MAX_PATH] = { 0 };

	strcat_s(command, _MAX_PATH, "\\"); //"\\" + name + ".exc"; "
	strcat_s(command, _MAX_PATH, name);
	strcat_s(command, _MAX_PATH, ".exc");
	strcat_s(file_all_path, _MAX_PATH, file_path);
	if (fix_only)
	{
		strcat_s(infile_name, _MAX_PATH, file_all_path);
		strcat_s(infile_name, _MAX_PATH, command);
		strcat_s(outfile_name, _MAX_PATH, file_all_path);
		strcat_s(outfile_name, _MAX_PATH, "\\temp.exc");
	
	}
	else
	{
		strcat_s(infile_name, _MAX_PATH, file_all_path);
		strcat_s(infile_name, _MAX_PATH, "\\");
		strcat_s(infile_name, _MAX_PATH, name);
		strcat_s(infile_name, _MAX_PATH, "_objs");
		strcat_s(infile_name, _MAX_PATH, command);

		strcat_s(outfile_name, _MAX_PATH, file_all_path);
		strcat_s(outfile_name, _MAX_PATH, "\\");
		strcat_s(outfile_name, _MAX_PATH, name);
		strcat_s(outfile_name, _MAX_PATH, "_objs");
		strcat_s(outfile_name, _MAX_PATH, "\\temp.exc");

	
	}
	string command1 = "cd ..";
	cmd_input((LPSTR)command1.c_str());

	if ((in_err  = fopen_s(&in_fp, infile_name, "r")) == NULL)
	{
		
		if ((out_err = fopen_s(&out_fp, outfile_name, "w")) != NULL)
		{
			command1 = name;
			command1 = command1 + "_objs";
			command1 = "cd " + command1;
			if (in_fp)
			{
				fclose(in_fp);
			}
			if (out_fp)
			{
				fclose(out_fp);
			}
			nt_assert(0,"create temp exc file  error!\n");
			return;
		}
		command1 = name;
		command1 = command1 + "_objs";
		command1 = "cd " + command1;
		cmd_input((LPSTR)command1.c_str());
		printf("infile_name:%s outfile_name:%s in_err:%x out_err:%x\n", infile_name, outfile_name, in_err, out_err);

		while (!feof(in_fp))
		{
			fgets(strLine, _MAX_PATH, in_fp);
			if (strcmp(strLine,"\n") == 0)
			{
				if (!feof(in_fp))
				{
					fgets(strLine, _MAX_PATH, in_fp);
					if (strcmp(strLine, "\n") != 0 && !strchr(strLine, ';'))
					{
						first++;
						if (first>1)
						{
							fwrite("\n", sizeof(char), 1, out_fp);
							strcat_s(str_new_line, _MAX_PATH, "-");
							strcat_s(str_new_line, _MAX_PATH, strLine);
							fprintf(out_fp, "%s", str_new_line);
							memset(str_new_line, 0, _MAX_PATH);
							memset(strLine, 0, _MAX_PATH);
						}
						else
						{
							strcat_s(str_new_line, _MAX_PATH, "-");
							strcat_s(str_new_line, _MAX_PATH, strLine);
							fprintf(out_fp, "%s", str_new_line);
							memset(str_new_line, 0, _MAX_PATH);
							memset(strLine, 0, _MAX_PATH);
						}
					}
					else
						break;
				}
				else
					break;
			}
			else if (!strchr(strLine, ';'))
			{
				strcat_s(str_new_line, _MAX_PATH, strLine);
				fprintf(out_fp, "%s", str_new_line);
				memset(str_new_line, 0, _MAX_PATH);
				memset(strLine, 0, _MAX_PATH);
			}
			else
				continue;

		}
	
		if (in_fp)
		{
			fclose(in_fp);
		}
		if (out_fp)
		{
			fclose(out_fp);
		}
		memset(command, 0, _MAX_PATH);
		strcat_s(command, _MAX_PATH, "del ");
		strcat_s(command, _MAX_PATH, name);
		strcat_s(command, _MAX_PATH, ".exc");
		cmd_input((LPSTR)command);

		memset(command, 0, _MAX_PATH);
		strcat_s(command, _MAX_PATH, "ren temp.exc ");
		strcat_s(command, _MAX_PATH, name);
		strcat_s(command, _MAX_PATH, ".exc");
        cmd_input((LPSTR)command);

		if (fix_only)
		{
			memset(command, 0, _MAX_PATH);
			strcat_s(command, _MAX_PATH, "del temp.exc");
			cmd_input((LPSTR)command);

			memset(command, 0, _MAX_PATH);
			strcat_s(command, _MAX_PATH, "sigmake -x\"");
			strcat_s(command, _MAX_PATH, name);
			strcat_s(command, _MAX_PATH, "\" ");
			strcat_s(command, _MAX_PATH, name);
			strcat_s(command, _MAX_PATH, ".pat ");
			strcat_s(command, _MAX_PATH, name);
			strcat_s(command, _MAX_PATH, ".sig");
			cmd_input((LPSTR)command);
		
		}
		return;
    }

	command1 = name;
	command1 = command1 + "_objs";
	command1 = "cd " + command1;

	printf("infile_name:%s outfile_name:%s in_err:%x out_err:%x\n", infile_name,outfile_name, in_err, out_err);
	
}
void getFiles(string path, vector<string>& files, string sub)
{
	//文件句柄  
	long   hFile = 0;
	string name;
	int positon = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files,sub);
				else
				{ }
			}
			else
			{
				name = fileinfo.name;
				positon = name.find(sub);
				if (positon != name.npos)
					name.replace(positon, 4, "");
				else
				{
					_findclose(hFile);
					nt_assert(0, "there is a null correct file format in the file directory!\n");
				}
					
			    files.push_back(name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void makesig_no_pcf()
{
	_tprintf(
		"*****************************************************************\n"
		"                 You choose make sig with no pcf!\n"
		"*****************************************************************\n");
	string name;
	if (strlen(file_path) == 0)
	{
		setting_init();
		init_pipe(vcvarsall_para);
		cmd_input(file_path_driver);
		memset(file_path_driver, 0, _MAX_PATH);
		strcpy_s(file_path_driver, _MAX_PATH, "cd ");
		strcat_s(file_path_driver, _MAX_PATH, (PCHAR)(file_path));
		cmd_input(file_path_driver);

	}
	//strcat_s(file_path_driver, _MAX_PATH, (PCHAR)"\\lib");

	cmd_input("md sig");
	cmd_input("md error_lib");
    cmd_input("md error_exc");
	cmd_input("md error_pat");

	getFiles(file_path, files_name,".pat");
	for (size_t i = 0; i < files_name.size(); i++)
	{

		name = files_name[i];
		process_perfile(name,true);

	}
	_tprintf(
		"*****************************************************************\n"
		"                 makesig with no pcf finished!\n"
		"*****************************************************************\n");
}
void build_unit(LPSTR command)
{
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	CHAR temp[1024] = { 0 };
	sprintf_s(temp, 1024, "lib /OUT:lua-%d-%d-%d-%d.lib *.obj", t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	if (!cmd_input(command))
	{
		cmd_input("del *.obj");
		return;
	}
	cmd_input("ren lua.obj lua.o");
	cmd_input("ren luac.obj luac.o");
	cmd_input(temp);
	cmd_input("del *.obj");
	cmd_input("del *.o");
	cmd_input("move *.lib lib");
}
bool cmd_input(LPSTR cmd)
{
	bool result = true;
	strcpy_s(writeBuff, 256, cmd);
	strcat_s(writeBuff, 256, "\r\n");
	shell();
	//printf("%s", readBuff);//很关键
	if (strstr(readBuff, "warning D") || strstr(readBuff, "error D"))
	{
		result = false;
	}
	memset(readBuff, 0, sizeof(readBuff));
	return result;
}
/*从字符串的中间截取n个字符*/
char * str_mid(char *dst, char *src, int n, int m) /*n为长度，m为位置*/
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if (n>len) n = len - m;    /*从第m个到最后*/
	if (m<0) m = 0;    /*从第一个开始*/
	if (m>len) return NULL;
	p += m;
	while (n--)
	{
		
		*(q++) = *(p++);
	}
		
	*(q++) = '\0'; /*有必要吗？很有必要*/
	return dst;
}
char * str_replace(char const * const original,char const * const pattern, char const * const replacement)
{
	size_t const replen = strlen(replacement);
	size_t const patlen = strlen(pattern);
	size_t const orilen = strlen(original);

	size_t patcnt = 0;
	const char * oriptr;
	const char * patloc;

	// find how many times the pattern occurs in the original string  
	for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
	{
		patcnt++;
	}

  {
	  // allocate memory for the new string  
	  size_t const retlen = orilen + patcnt * (replen - patlen);
	  char * const returned = (char *)malloc(sizeof(char) * (retlen + 1));

	  if (returned != NULL)
	  {
		  // copy the original string,   
		  // replacing all the instances of the pattern  
		  char * retptr = returned;
		  for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
		  {
			  size_t const skplen = patloc - oriptr;
			  // copy the section until the occurence of the pattern  
			  strncpy_s(retptr, retlen + 1,oriptr, skplen);
			  retptr += skplen;
			  // copy the replacement   
			  strncpy_s(retptr,retlen + 1, replacement, replen);
			  retptr += replen;
		  }
		  // copy the rest of the string.  
		  strcpy_s(retptr, retlen + 1, oriptr);
	  }
	  return returned;
  }
}


bool execute_package(LPCSTR fileName, LPCSTR args, LPCSTR baseDir, bool wait)
{
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;

	sei.lpFile = fileName;
	sei.lpParameters = args;
	sei.lpDirectory = baseDir;

	if (!ShellExecuteEx(&sei)) {
		return false;
	}

	if (wait) {
		HANDLE hProcess = sei.hProcess;
		if (hProcess != 0) {
			WaitForSingleObject(hProcess, INFINITE);
			CloseHandle(hProcess);
		}
	}

	return true;
}


//%comspec% / k ""D:\Program Files(x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"" x86
bool check_error(LPSTR *check_array, LPSTR comb)
{
	int count = 0;
	LPSTR str = NULL;
	LPSTR position = NULL;
	int offset = 0;
	int offset_size[46] = { 0, 4, 4, 3, 8, 2, 10, 2, 12, 4, 16, 5, 21, 5, 26, 5, 32, 4, 37, 2, 41, 2, 43, 2, 45, 2, 48, 2, 52, 2, 54, 6, 60, 2, 63, 2, 66, 2, 74, 2, 77, 2, 79, 3, 88, 2 };
	for (int i = 0; i < 46; i += 2)
	{

		for (int j = 0; j <offset_size[i + 1]; j++)
		{
			offset = offset_size[i];
			str = check_array[offset + j];
			position = strstr(comb, str);
			if (position)
			{
				count++;
			}
			if (count > 1)
				return true;
			str = "";
			position = NULL;
		}

	}
	return false;
}


LPSTR str_connect(LPSTR *new_comb, int count)
{
	LPSTR result = NULL;
	result = (LPSTR)malloc(1024);
	nt_assert(result, "malloc connect str failed\n");
	memset(result, 0, 1024);
	strcat_s(result, 1024,"cl ");
	for (int i = 0; i < count; i++)
	{
		strcat_s(result,1024, new_comb[i]);
		strcat_s(result,1024, " ");
	}
	strcat_s(result, 1024,"/c /DLUA_BUILD_AS_DLL *.c");
	return result;
}
void nt_assert(PVOID condition,LPSTR info)
{
	if (!condition)
	{
		assert(condition);
		_tprintf("%s\n",info);
		system("pause");
		exit(1);
	}

}
void get_comb(LPSTR *build_arg_array, int array_count, int array_get, LPSTR *new_comb)
{
	LPSTR comb = NULL;

	nt_assert(new_comb,"malloc new_comb faied!");

	for (int i = array_count; i >= array_get; i--)
	{
		strcpy_s(new_comb[array_get - 1], 32, build_arg_array[i - 1]);//取出第n个元素作为组合的第一个元素    
	   
		if (array_get>1)
			get_comb(build_arg_array, i - 1, array_get - 1, new_comb);  //递归，在n-1个元素中取m-1个元素,直到取出最后一个元素    

		comb = str_connect(new_comb, array_get);     //获得一个组合 
		nt_assert(comb, "malloc comb faied!");

		if (!check_error(build_arg_array, comb))
		{
			build_unit(comb);
			/*  命令格式正常 执行命令 前缀加上一些cl.exe和命令考虑%comspec% /k ""D:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"" x86   考虑执行是否成功 考虑效率  考虑执行 
			if (执行成功)
			{
			DEBUG("打印简短的命令输出\n");
			}
			else
			DEBUG("打印简短的错误消息\n");*/
		}
		free(comb);
	
	}
	
}
void get_all_comb(LPSTR *build_arg_array, LPSTR *new_comb)
{

	for (int i = 1; i <= 90; i++)
	{
		for (int j = 0; j < 90; j++)
		    memset(new_comb[j], 0, 32);
        get_comb(build_arg_array, 90, i, new_comb);
	}
		

}

void shell()
{
	//if(initPipeSuccess == FALSE)
	

	DWORD dwByteWritten;

	WriteFile(hWritePipeShell, writeBuff, strlen(writeBuff), &dwByteWritten, 0);//写管道
	
	

	//Sleep(1000);

	unsigned long   BytesRead = 0;
	unsigned long   fp = (unsigned long)readBuff;
	DWORD TotalBytesAvail;
	
	//检查管道中是否有数据
	while (PeekNamedPipe(hReadPipeCmd, readBuff, sizeof(readBuff), &BytesRead, &TotalBytesAvail, NULL))
	{
		if (TotalBytesAvail <= 0)
			Sleep(300);
		else
		{
			
			
			if (BytesRead == TotalBytesAvail)
			{
				
				
				//fp = fp + BytesRead;
				if (strstr(writeBuff, "cl "))
				{
					memset(writeBuff, 0, 256);
					continue;
					/*Sleep(600);
					PeekNamedPipe(hReadPipeCmd, readBuff, sizeof(readBuff), &BytesRead, &TotalBytesAvail, NULL);
					if (TotalBytesAvail <= 0)
						break;
					else
						ReadFile(hReadPipeCmd, (LPVOID)fp, TotalBytesAvail + 100, &BytesRead, NULL);*/
				}
				ReadFile(hReadPipeCmd, (LPVOID)readBuff, TotalBytesAvail + 100, &BytesRead, NULL);
				break;
			}
		}
		
	}
	memset(writeBuff, 0, 256);
}
//创建双管道和创建cmd进程
void init_pipe(LPSTR param)
{
	SECURITY_ATTRIBUTES sa = { 0 };
	STARTUPINFO         si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	//创建管道
	CreatePipe(&hReadPipeCmd, &hWritePipeCmd, &sa, 0);
	CreatePipe(&hReadPipeShell, &hWritePipeShell, &sa, 0);

	GetStartupInfo(&si);
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput = hReadPipeShell;
	si.hStdOutput = si.hStdError = hWritePipeCmd;
	//找到cmd的绝对路径
	char strShellPath[256] = "\x00";
	GetSystemDirectory(strShellPath, 256);
	strcat_s(strShellPath,256, "\\cmd.exe");
	//创建cmd进程
	if (!CreateProcess(strShellPath, param, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		//printf("CreateProcess Error!\n");
		CloseHandle(hWritePipeCmd);
		CloseHandle(hReadPipeShell);
		//initPipeSuccess = FALSE;
		return;
	}
	hProcessHandle = pi.hProcess;
	//initPipeSuccess = TRUE;
}