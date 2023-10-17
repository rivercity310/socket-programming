#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERPORT 9000

#pragma comment(lib, "ws2_32")

/* IPv6 ���� TCP ���� �ʱ�ȭ �� ��ȯ (Listening ����) */
SOCKET* InitServerSocket6(int* retval);

/* IPv4 ���� TCP ���� �ʱ�ȭ �� ��ȯ (Listening ����) */
SOCKET* InitServerSocket4(int* retval);

/* ���� ó�� �Լ� */
void err_quit(const char* msg);
void err_display(const char* msg);
