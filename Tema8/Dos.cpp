/*Визначити, який логічний диск містить більшевиконуваних файлів формату DOS
_CRT_SECURE_NO_WARNINGS
*/
#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <cstdint>
#include <io.h>
#include <time.h>
#include <iostream>
#include <iostream>
#include <string>
#include <regex>
using namespace std;


char directory[2][256] = { "C:\\","D:\\" };///адреса директорії
long _count[2];
int i = 0;
bool _status[2];///статус кінця виконання підрахування
std::regex DOS("^.[Dd][Oo][Ss]$");

void Report()
{
	printf("Порахунок файлів завершено\n");
	printf("За отриманими даними стверджую, що при аналізу  %s та %s було підраховано кількість .dos файлів.\n", directory[0], directory[1]);
	printf("\t<%s> - %d files\n\t<%s> - %d files\n",directory[0], _count[0],directory[1],_count[1]);
	printf("Отже\n");
	if (_count[0] > _count[1]) printf("Файлів в %s більше ніж %s\n", directory[0], directory[1]);
	else if (_count[0] < _count[1]) printf("Файлів в %s більше ніж %s\n", directory[1], directory[0]);
	else if (_count[0] == _count[1]) printf("В томах %s та %s кількість однакова\n", directory[0], directory[1]);

}


void CountDOS(const char* dir, long& size)
{
	struct _finddata_t c_file;
	intptr_t hFile;
	char buf[256];
	strcpy(buf, dir);
	strcat(buf, "*");
	// Пошук  файл у поточному каталозі
	if ((hFile = _findfirst(buf, &c_file)) == -1L)
		printf("No  files in current directory!\n");
	else
	{

		do {
			// якщо каталог 
			if (c_file.attrib == 16 && (c_file.name[0] != '.')&& (c_file.name[0] != '..'))
			{
				char* newdir = new char[strlen(dir) + strlen(c_file.name) + 2];
				strcpy(newdir, dir);
				strcat(newdir, c_file.name);
				strcat(newdir, "\\");
				CountDOS(newdir, size); //рекурсивний виклик 
				delete newdir;
			}

			if (strcmp(c_file.name, ".") != 0 && strcmp(c_file.name, "..") != 0)//уникнення невідомих файлів 
				if ((regex_match(c_file.name, DOS)) == 0)
					size++;

		 
		} while (_findnext(hFile, &c_file) == 0);
	}

}

DWORD WINAPI Poshuk(void* param)
{
	int a = i++;
	CountDOS(directory[a], _count[a]);
	_status[a] = true;
	return 0;
}


void main()
{
	setlocale(LC_ALL, "RUS");
	_count[0] = 0;
	_count[1] = 0;
 


	DWORD ThreadId;
	CreateThread(0, 0, Poshuk, 0, 0, &ThreadId);///паралельно приймати повідомлення
	CreateThread(0, 0, Poshuk, 0, 0, &ThreadId);///паралельно приймати повідомлення

 
	for (;;)if (_status[0] == true && _status[1] == true) break;///очікування кінця підрахунку
	Report();

	system("pause");

}
