#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <iostream>
#include <iostream>



HANDLE hMailslotIn, hMailslotOutA, hMailslotOutB;
char szBuf[512]; // Буфер для передач і даних
DWORD cbWritten; // Кількість байтів, переданих
 
LPSTR lpszMailslotNameA = (LPSTR)"\\\\.\\mailslot\\$A";
LPSTR lpszMailslotNameB = (LPSTR)"\\\\.\\mailslot\\$B";
LPSTR lpszMailslotNameC = (LPSTR)"\\\\.\\mailslot\\$C";

BOOL fReturnCode; // Код повернення з функцій
DWORD cbMessages; // Розмір повідомлення
DWORD cbMsgNumber; // Кількість повідомленьу
DWORD cbRead; // Кількість байтів даних, прийнятих

DWORD WINAPI MailBoxMassage(void* param)
{
	while (1)
	{
		// 		Визначення стану Mailslot
		fReturnCode = GetMailslotInfo(hMailslotIn, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			printf("reception error!!! \n");
			system("pause");
		}
		// 	Якщо в		Mailslot є	повідомлення, читаємо перше
		if (hMailslotIn != 0)
		{
			if (ReadFile(hMailslotIn, szBuf, 512, &cbRead, NULL))
				printf("Received : <%s> \n",  szBuf);
			else printf("Error Data Transfer!!!	\n");
		}
	}
}


int main()
{
	// С творення	Mailslot, з ім’ям $C
	hMailslotIn = CreateMailslot(lpszMailslotNameC, 0, MAILSLOT_WAIT_FOREVER, NULL);
	// 	Якщо виникла помилка–	вихід з програми
	if (hMailslotIn == INVALID_HANDLE_VALUE)
	{
		printf("Error MailSlot Creating 'A'!!! \n");
		system("pause");
		return 0;
	}

	Sleep(1000);

	// 	Відкриття		mailbox	A
	hMailslotOutA = CreateFile(lpszMailslotNameA, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	// 	Якщо виникла помилка	–	вихід з програми
	if (hMailslotOutA == INVALID_HANDLE_VALUE)
	{
		printf("Open 'A' Error!!! \n");
		system("pause");
		return 0;
	}


	/// 	Відкриття		mailbox	B	
	hMailslotOutB = CreateFile(lpszMailslotNameB, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	// 	Якщо виникла помилка	–	вихід з програми
	if (hMailslotOutB == INVALID_HANDLE_VALUE)
	{
		printf("Open  'B' Error!!! \n");
		system("pause");
		return 0;
	}

	DWORD ThreadId;
	CreateThread(0, 0, MailBoxMassage, 0, 0, &ThreadId);///паралельно приймати повідомлення


	std::cout << "mail box C" << std::endl;
	while (true) //відправлення повідомлення
	{
		std::cin >> szBuf;
		/////////////////////////////////////////////////////////////////////////////
		if (!WriteFile(hMailslotOutB, szBuf, strlen(szBuf) + 1,&cbWritten, NULL))printf("Data transfer error in B! \n"); 
		else printf("Transferred %d bytes: <%s> \n", cbWritten, szBuf);
		/////////////////////////////////////////////////////////////////////////////
		if (!WriteFile(hMailslotOutA, szBuf, strlen(szBuf) + 1, &cbWritten, NULL))printf("Data transfer error in A \n");
		else printf("Transferred %d bytes: <%s> \n", cbWritten, szBuf);
	}

	system("pause");
	return 0;
}