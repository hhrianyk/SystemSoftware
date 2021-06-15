//Ім’я каталогу вводиться з клавіатури.Підкаталоги аналізуються.
//Відстежувати появу файлів з однаковими іменами в різних підкаталогах.

#define _WIN32_WINNT 0x0500
#define _UNICODE
#define UNICODE 
#include <windows.h> 
#include <conio.h> 
#include <stdio.h> 
#include <clocale>
#include <iostream>
#include <io.h>
#include <malloc.h>
#include <stdlib.h>
using namespace std;
char dir[]= "D:\\temp";
int _countCopy = 0;
bool _copy = false;
char *buf;
void _Filename(WCHAR* FileName)
{
	int len = wcslen(FileName),
	  a = len;
	while (true)
	{
		if (FileName[a] == 92 || a < 0) break;
		else a--;
	}
	a++;
	size_t m = wcslen(FileName) * sizeof(char) - a;
	free(buf);
	buf = NULL;
	buf = (char*)malloc(m-4);     //// помилка тут занадто багато памяті виділяється
	 
	for (int i = 0; a < len; a++, i++)
		buf[i] = FileName[a];

	//return buf;
}


bool listdir(char* _dir) {
	_finddata_t fd;
	int rc;
	long hFile;
	char mask[260];
	char subdir[260];
	 
	sprintf(mask, "%s\\*.*", _dir);

	for (rc = (hFile = _findfirst(mask, &fd)); rc != -1; rc = _findnext(hFile, &fd)) {
		if (fd.name[0] != '.') {
			//printf("\n%s\\%s", _dir, fd.name);
			if (fd.attrib & _A_SUBDIR) {
				sprintf(subdir, "%s\\%s", _dir, fd.name);
				listdir(subdir);
			}
		}
		if (strcmp(buf, fd.name) == 0)
		{
			_copy = true;
			_countCopy++;
			printf("\n%s\\%s", _dir, fd.name);
		}
	}
	return _copy;
}

  WCHAR* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	WCHAR* wc = new WCHAR[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}


int wmain(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	buf = (char*)malloc(0);
	buf = (char*)realloc(buf,1);
	cout << "input dir :";
	cin >> dir;
	cout << endl;
	 
	WCHAR* D = GetWC(dir);
 
	DWORD nBufSize = 32 * 1024;
	FILE_NOTIFY_INFORMATION* pBuffer = (FILE_NOTIFY_INFORMATION*)calloc(1, nBufSize);
	FILE_NOTIFY_INFORMATION* pBufferCurrent;
	WCHAR fname[100];
	HANDLE hDir = CreateFile((LPCWSTR)D, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	DWORD BytesReturned;
	cout <<"Open?"<< endl;

	while (true)
	{
		ReadDirectoryChangesW(hDir, pBuffer, nBufSize, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME| FILE_NOTIFY_CHANGE_DIR_NAME, &BytesReturned, NULL, NULL);
		pBufferCurrent = pBuffer;
		while (pBufferCurrent)
		{
			wcsncpy(fname, pBufferCurrent->FileName, pBufferCurrent->FileNameLength / 2);
			fname[pBufferCurrent->FileNameLength / 2] = 0;
			_copy = false;
			switch (pBufferCurrent->Action)
			{
			 
			case FILE_ACTION_RENAMED_NEW_NAME:	wprintf(L"NEW FILE NAME : % s\n", fname); 
				   _Filename(fname);
				   _countCopy = 0;
				   listdir(dir);
				if (_countCopy>1)wprintf(L"\nA file with this name already exists: : %s ; copy: %d\n", fname, --_countCopy);
				break;
 
			case FILE_ACTION_ADDED: wprintf(L"New FILE  : % s \n", fname);
				_Filename(fname);
				_countCopy = 0;
				listdir(dir);
				if (_countCopy > 1)wprintf(L"\nA file with this name already exists: : %s ; copy: %d\n", fname, --_countCopy);
				break;

	 
				 
			}
			// 	пошук	наступної порції даних у буфері
			if (pBufferCurrent->NextEntryOffset)
				pBufferCurrent = (FILE_NOTIFY_INFORMATION*)(((BYTE*)pBufferCurrent) + pBufferCurrent->NextEntryOffset);
			else pBufferCurrent = NULL;
		}

	}
}
