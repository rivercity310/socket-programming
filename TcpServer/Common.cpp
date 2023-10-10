#include "Common.h"

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;

	// 1. ���� �߻��� ���� �Լ��� ���ϰ��� WSAGetLastError() �Լ��� ���� ��ü���� ���� �ڵ带 ���� �� �ִ�.
	// 2. WSAGetLastError()�� �������ִ� ���� �ڵ忡 �����ϴ� ���� �޼����� FormatMessage()�� ���� ���� �� �ִ�.
	// 3. WORD = unsigned short (2 byte), DWORD = unsigned long (4 byte)
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf,
		0,
		NULL
	);

	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);    // exit 0 ��������, 1 ��������
}

// ���� �Լ� ���� ���
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf,
		0,
		NULL
	);

	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errorcode) {
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf,
		0,
		NULL
	);

	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
