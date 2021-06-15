#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <iostream>
#include <iostream>
 
HANDLE hMailslotIn, hMailslotOutB, hMailslotOutC;
char szBuf[512]; // ����� ��� ������� � �����
DWORD cbWritten; // ʳ������ �����, ���������
//����� Mailslot
LPSTR lpszMailslotNameA = (LPSTR)"\\\\.\\mailslot\\$A";
LPSTR lpszMailslotNameB = (LPSTR)"\\\\.\\mailslot\\$B";
LPSTR lpszMailslotNameC = (LPSTR)"\\\\.\\mailslot\\$C";

BOOL fReturnCode; // ��� ���������� � �������
DWORD cbMessages; // ����� �����������
DWORD cbMsgNumber; // ʳ������ �����������
DWORD cbRead; // ʳ������ ����� �����, ���������

DWORD WINAPI MailBoxMassage(void* param)
{
	while (1)
	{
		// 		���������� ����� Mailslot
		fReturnCode = GetMailslotInfo(hMailslotIn, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			printf("reception error!!! \n");
			system("pause");

		}
		// 	���� �		Mailslot �	�����������, ������ �����
		if (hMailslotIn != 0)
		{
			if (ReadFile(hMailslotIn, szBuf, 512, &cbRead, NULL))
				printf("Received : <%s> \n", szBuf);
			else printf("Error Data Transfer!!!	\n");

		}
	}
}


int main()
{
	 // � ��������	Mailslot, � ���� $A
	hMailslotIn = CreateMailslot(lpszMailslotNameA, 0, MAILSLOT_WAIT_FOREVER, NULL);
	// 	���� ������� ��������	����� � ��������
	if (hMailslotIn == INVALID_HANDLE_VALUE)
	{
		printf("Error MailSlot Creating 'A'!!! \n");
		system("pause");
		return 0;
	}
	Sleep(1000);

	// 	³�������		mailbox	B
	hMailslotOutB = CreateFile(lpszMailslotNameB, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	// 	���� ������� �������	�	����� � ��������
	if (hMailslotOutB == INVALID_HANDLE_VALUE)
	{
		printf("Open  'B' Error!!! \n");
		system("pause");
		return 0;
	}


	/// 	³�������		mailbox	C	
	hMailslotOutC = CreateFile(lpszMailslotNameC, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	// 	���� ������� �������	�	����� � ��������
	if (hMailslotOutC == INVALID_HANDLE_VALUE)
	{
		printf("Open  'C' Error!!! \n");
		system("pause");
		return 0;
	}

	DWORD ThreadId;
	CreateThread(0, 0, MailBoxMassage, 0, 0, &ThreadId);///���������� �������� �����������

	std::cout << "mail box A" << std::endl;
	while (true)//����������� �����������
	{
		std::cin >> szBuf;
/////////////////////////////////////////////////////////////////////////////
		if (!WriteFile(hMailslotOutB, szBuf, strlen(szBuf) + 1,&cbWritten, NULL))  printf("Data transfer error in B! \n");
		else printf("Transferred %d bytes: <%s> \n", cbWritten, szBuf);
/////////////////////////////////////////////////////////////////////////////
		if (!WriteFile(hMailslotOutC, szBuf, strlen(szBuf) + 1,	&cbWritten, NULL))	printf("Data transfer error in C \n");
		else printf("Transferred %d bytes: <%s> \n", cbWritten, szBuf);
	}

	system("pause");
	return 0;
}