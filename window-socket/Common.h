#define _WINSOCK_DEPRECATED_NO_WARNINGS  // ���� ���� API ��� �� ��� ���� 
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����

#include <winsock2.h>     // WinSock ���� ���
#include <ws2tcpip.h>     // WinSock Ȯ�� ���

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(int errorcode);