/*виклик іменованих каналів*/
#include <windows.h>
#include <stdio.h>

STARTUPINFO si;
PROCESS_INFORMATION pi;

BOOL fConnected;
LPSTR lpszPipeName = (LPSTR)"\\\\.\\pipe\\$MyFirstPipe";
LPSTR lpszPipeName2 = (LPSTR)"\\\\.\\pipe\\$MyFirstPipe2";
LPSTR lpszPipeName3 = (LPSTR)"\\\\.\\pipe\\$MyFirstPipe3";
HANDLE hNamedPipe, hNamedPipe2, hNamedPipe3;

void main(int argc, char* argv[])
{

    CreateProcess(NULL, (LPSTR)"Tema6.1.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    CreateProcess(NULL, (LPSTR)"Tema6.2.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    CreateProcess(NULL, (LPSTR)"Tema6.3.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
     
}