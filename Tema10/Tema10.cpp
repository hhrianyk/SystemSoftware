/*Написати програму, яка виконує шукає указані
ключі у реєстрі та виводить всі їх значення
згідно з їх типами.*/


#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <iostream>
#include <iostream>
using namespace std;

char keyName[225],buf[1024];

 

void InfoKey(HKEY hkey, char* path)
{
	DWORD   dwValues, dwMaxValueNameLen, dwMaxValueData, dwValueData, dwIndex=0, dwType, dwNameLen;
	LPTSTR lpszVN1, lpszVN2=NULL;
	LONG lReturn;
	HKEY hk;


	if (RegOpenKeyEx(hkey, path, 0, KEY_ALL_ACCESS, &hk))
	{
		printf("Key Open Error \n"); return;
	}
//Визначення необхідних параметрів для читання
	RegQueryInfoKey(hk, NULL, NULL, NULL, NULL, NULL, NULL, &dwValues, // Кількість значень у	ключі
	&dwMaxValueNameLen, // максимальна довжина
	// 	імені значення
	&dwMaxValueData, // максимальна довжина
	// значення
	NULL, NULL);

	dwMaxValueNameLen++;
	dwMaxValueData++;
// виділення пам’яті для збереження рядків
	  lpszVN1 = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwMaxValueNameLen);
     //lpszVN2 = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwMaxValueNameLen);

	// читання	значень	ключа
		do
		{
			dwNameLen = dwMaxValueNameLen;
			dwValueData = dwMaxValueData;
			lReturn = RegEnumValue(hk, dwIndex,	lpszVN1,&dwNameLen, 0, &dwType,	(LPBYTE)lpszVN2,&dwValueData);
			if (lReturn != ERROR_NO_MORE_ITEMS)
			{
				// printf("\t\t%s=%s\n",lpszVN1, lpszVN2);
				printf("\t\t%s\n", lpszVN1);
				dwIndex++;
			}
		} while (lReturn != ERROR_NO_MORE_ITEMS);
 

	// 	Звільняємо пам’ять

	  if (lpszVN1)HeapFree(GetProcessHeap(), 0, lpszVN1);
	 //if (lpszVN2) HeapFree(GetProcessHeap(), 0, lpszVN2);
     RegCloseKey(hk);
 


 }


void print_list_key(HKEY hkey, char* path) {

	char dwValue[255];
	DWORD  dwSize = 0;
	DWORD n; // subkeys
	HKEY tmp;
 
	char subdir[560];
	if (RegOpenKeyEx(hkey, path, 0, KEY_READ, &tmp) == ERROR_SUCCESS) {
	    dwSize = sizeof(dwValue);
		RegQueryInfoKey(tmp, NULL, NULL, NULL, &n, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		for (int i = 0; i < n; i++) {
			dwSize = sizeof(dwValue);
			RegEnumKeyEx(tmp, i, dwValue, &dwSize, NULL, NULL, NULL, NULL);
 
			if (strcmp(dwValue, keyName) == 0)
			{
				sprintf(buf, "%s\\%s", path, dwValue);
				printf("%s\n", buf);
				InfoKey(hkey, buf);
			}
			if(strlen(path)==0)
				print_list_key(hkey, dwValue);
			else {
				sprintf(subdir, "%s\\%s", path,dwValue);
				print_list_key(hkey, subdir);
			}
			
		}

		RegCloseKey(tmp);
	}
}

int main()
{

 
	while (true)
	{
		cout << "\n\nInput Key: ";
		cin >> keyName;

		 
		print_list_key(HKEY_CURRENT_USER, (char*)"");
	}
	return 0;
}

