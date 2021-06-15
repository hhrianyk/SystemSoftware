/*1->2,3;2->1,3;3->1, 2.*/


#include <windows.h>
#include <conio.h>
 
STARTUPINFO si;
PROCESS_INFORMATION pi;
 
int main(int argc, char* argv[])
{
    CreateProcess(NULL, (LPSTR)"Tema7.3.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    CreateProcess(NULL, (LPSTR)"Tema7.2.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    CreateProcess(NULL, (LPSTR)"Tema7.1.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
}