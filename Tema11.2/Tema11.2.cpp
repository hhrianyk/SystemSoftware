#pragma optimize ("gsy", on)  //позволим компилятору самому оптимизировать код
#pragma comment(linker,"/MERGE:.rdata=.text")  // объединяем секции
#pragma comment(linker,"/MERGE:.data=.text")  // объединяем секции
#pragma comment(linker, "/SECTION:.text,EWR")
#pragma comment(linker,"/NODEFAULTLIB")
#pragma comment(linker,"/ENTRY:KinderGarten")  //точка входа
#pragma comment(linker,"/FILEALIGN:number")
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#pragma comment(linker,"/INCREMENTAL:NO")


#define _WIN32_WINNT 0x500

#include <windows.h>;
//#include <cctype>                           // для функции tolower
#include <stdbool.h>
LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
WNDCLASS WND; // структура класса окна
bool Dflag = false;
HWND hwnd, hwnd1, hwnd2, hwndButton; // дескриптор окна
TCHAR buff[1024] = { 0 };
#define IDB_Button1 121
int WINAPI KinderGarten()
{

	MSG msg; // структура сообщени


	// Регистрация класса окна
	memset(&WND, 0, sizeof(WNDCLASS));
	//w.lpszMenuName = L"IDC_WINDOWSPROJECT";
	WND.style = CS_HREDRAW | CS_VREDRAW;
	WND.lpfnWndProc = WndProc; // имя оконной функции
	WND.hInstance = GetModuleHandle(NULL); // указатель на строку, содержащую имя меню, применяемого для класса;
	WND.hbrBackground = (HBRUSH)(ANSI_FIXED_FONT);
	WND.lpszClassName = L"My Class";

	RegisterClass(&WND);

	// Создание окна

	hwnd = CreateWindow(L"My Class", L"ALEX",
		WS_OVERLAPPEDWINDOW, 500, 300, 500, 380, NULL, NULL, GetModuleHandle(NULL), NULL);
	SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) | CS_DBLCLKS);
	ShowWindow(hwnd, SW_SHOW); // отображение nCmdShow

	hwnd1 = CreateWindow(L"LISTBOX", L"ListBox1", WS_CHILD | LBS_EXTENDEDSEL | WS_BORDER | WS_VSCROLL, 10, 10, 470, 250, hwnd, NULL, NULL, NULL);
	ShowWindow(hwnd1, SW_SHOW);

	hwnd2 = CreateWindow(L"Edit", L"input", ES_CENTER | WS_CHILD | WS_BORDER | BS_AUTO3STATE, 10, 270, 470, 20, hwnd, NULL, NULL, NULL);
	ShowWindow(hwnd2, SW_SHOW);
	UINT IP = 221;
	hwndButton = CreateWindow(L"Button", L"Enter", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200, 300, 45, 25, hwnd, (HMENU)IDB_Button1, GetModuleHandle(NULL), NULL);
	ShowWindow(hwndButton, SW_SHOW);

	UpdateWindow(hwnd);          // перерисовка



// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LONG WINAPI WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{

	switch (Message)
	{
 
 
	case WM_COMMAND:
	{
		if (wparam == IDB_Button1)
		{
			GetWindowText(hwnd2, buff, 1024);
			int ix = 0;
			while (buff[ix]) { buff[ix] = CharUpper(buff[ix]); ix++; }
			SendMessage(hwnd1, LB_ADDSTRING, 0, (LPARAM)buff);
		}

	}break; break;
	case WM_DESTROY:
		PostQuitMessage(0);

	default: return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}
