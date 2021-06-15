#pragma optimize ("gsy", on)  //позволим компилятору самому оптимизировать код
#pragma comment(linker,"/MERGE:.rdata=.text")  // объединяем секции
#pragma comment(linker,"/MERGE:.data=.text")  // объединяем секции
#pragma comment(linker, "/SECTION:.text,EWR")
#pragma comment(linker,"/NODEFAULTLIB")
#pragma comment(linker,"/ENTRY:KingAlex")  //точка входа
#pragma comment(linker,"/FILEALIGN:number")
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#pragma comment(linker,"/INCREMENTAL:NO")
#pragma comment (lib, "MSVCRT.lib") 


#define _WIN32_WINNT 0x500

#define _WIN32_WINNT 0x500
#define Limit 10000
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
//#include <conio.h>
HANDLE hJob;
LPTSTR lpszSystemInfo;
TCHAR tchBuff[MAX_PATH + 1];
STARTUPINFO si;
PROCESS_INFORMATION pi;

bool stop = false;



 


void* _memset(void* _Dst, int _Val, size_t  _Size)
{
	UINT* buf = (UINT*)_Dst;
	while (_Size--)
	{
		*buf++ = (UINT)_Val;
	}
	return _Dst;
}

void ZeroMemor1(void* output, int len)
{
	while (len) {
		*(unsigned char*)output = 0;
		//output = (void*)((unsigned	int)output + 1);
		len--;
	}
}


DWORD WINAPI AddProcess2Job(void*)
{
	//WriteConsole("Create process\n");
 

	//memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (CreateProcess(NULL, lpszSystemInfo, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		auto p = pi.hProcess;
		auto t = pi.hThread;
		AssignProcessToJobObject(hJob, p);
		WaitForSingleObject(p, Limit);
		TerminateProcess(p, 0);


	}

	return 0;
}

DWORD WINAPI DEAEMEMORY(void*)
{
	//printf("DEAD MEMOTY ON\n");

	while (true)
	{
		DWORD ThreadId;
		CreateThread(0, 0, AddProcess2Job, 0, 0, &ThreadId);
		Sleep(100);
		if (stop == true) break;
	}

	return 0;
}



int KingAlex()///=int main()
{



	DWORD ThreadId;///для запуску процеса паралельно
	TCHAR tchBuff[MAX_PATH + 1];
	lpszSystemInfo = tchBuff;
	GetSystemDirectory(lpszSystemInfo, MAX_PATH + 1);
	wsprintf(tchBuff, "%s\\notepad.exe", lpszSystemInfo);
	hJob = CreateJobObject(NULL, "Job");//


	bool loop = true;
	int answ = 0;
	CreateThread(0, 0, AddProcess2Job, 0, 0, &ThreadId);
	//printf("1 - create process\n2 - DEAD MEMORY WARNING!!!!\n3 - exit(Close all)\n");
	while (loop)
	{

		//answ = _getch() - '0';
		switch (answ)
		{
		case 1:
			CreateThread(0, 0, AddProcess2Job, 0, 0, &ThreadId);
			break;
		case 2:
			CreateThread(0, 0, DEAEMEMORY, 0, 0, &ThreadId);
			break;
		case 3:
			stop = true;
			TerminateJobObject(hJob, NULL);
			//printf("exit\n");
			loop = false;
			break;
		default:
			break;
		}
	}
	//printf("");
	//system("PAUSE");
	return 0;
}
