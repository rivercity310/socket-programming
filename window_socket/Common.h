#pragma once

#include <winsock2.h>     // WinSock ���� ���
#include <ws2tcpip.h>     // WinSock Ȯ�� ���

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

void err_quit(const char* msg);
void err_display(const char* msg);