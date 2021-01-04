#include "stdafx.h"
#include  <stdio.h>  
#include  <stdlib.h> 
#include <iostream>
#include <array> 
#include   "windows.h "                      
#include   "shellapi.h " 
#include <assert.h>
#include <stdio.h>  
#include<time.h>
#include <vector>

using namespace std;
typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
}CLIENT_ID, *PCLIENT_ID, **PPCLIENT_ID;
typedef struct _LPC_MESSAGE
{
	USHORT                  DataSize;
	USHORT                  MessageSize;
	USHORT                  MessageType;
	USHORT                  DataInfoOffset;
	CLIENT_ID               ClientId;
	ULONG                   MessageId;
	ULONG                   SectionSize;
	//  UCHAR    & nbsp;                 Data[];
}LPC_MESSAGE, *PLPC_MESSAGE;

typedef
DWORD
(CALLBACK * NTACCEPTCONNECTPORT) (
OUT PHANDLE PortHandle,
IN PVOID PortContext OPTIONAL,
IN PLPC_MESSAGE ConnectionRequest,
IN BOOLEAN AcceptConnection,
IN OUT int int1, //    IN OUT PPORT_VIEW ServerView OPTIONAL,
OUT int int2  //OUT PREMOTE_PORT_VIEW ClientView OPTIONAL
);

template <int i> struct PORT_MESSAGEX : LPC_MESSAGE {
	UCHAR Data[i];
};

typedef NTSTATUS(NTAPI* pfnNtCreateThreadEx)
(
OUT PHANDLE hThread,
IN ACCESS_MASK DesiredAccess,
IN PVOID ObjectAttributes,
IN HANDLE ProcessHandle,
IN PVOID lpStartAddress,
IN PVOID lpParameter,
IN ULONG Flags,
IN SIZE_T StackZeroBits,
IN SIZE_T SizeOfStackCommit,
IN SIZE_T SizeOfStackReserve,
OUT PVOID lpBytesBuffer);

typedef
DWORD
(CALLBACK * NTREPLYWAITRECVIVEPORT)(
IN HANDLE               PortHandle,
OUT PHANDLE             ReceivePortHandle OPTIONAL,
IN PLPC_MESSAGE         Reply OPTIONAL,
OUT PLPC_MESSAGE        IncomingRequest);
NTREPLYWAITRECVIVEPORT     NtReplyWaitReceivePort;
NTACCEPTCONNECTPORT        NtAcceptConnectPort;
void shell();
void build_only();
void fixpat_only();
void makesig_only();
void makesig_no_pcf();
void setting_init();
void fix_pat(LPSTR name, bool fix_only);
void process_perfile(string name,bool no_pcf);
void getFiles(string path, vector<string>& files,string sub);
bool cmd_input(LPSTR cmd);
void build_unit(LPSTR command);
void init_pipe(LPSTR param);
void nt_assert(PVOID condition, LPSTR info);
LPSTR str_connect(LPSTR *new_comb, int count);
bool check_error(LPSTR *check_array, LPSTR comb);
void get_all_comb(LPSTR *build_arg_array, LPSTR *new_comb);
char * str_mid(char *dst, char *src, int n, int m); /*n为长度，m为位置*/
char * str_replace(char const * const original, char const * const pattern, char const * const replacement);
bool execute_package(LPCSTR fileName, LPCSTR args, LPCSTR baseDir, bool wait);
void get_comb(LPSTR *build_arg_array, int array_count, int array_get, LPSTR *new_comb);
