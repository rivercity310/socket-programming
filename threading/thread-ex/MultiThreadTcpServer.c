#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

#pragma comment(lib, "ws2_32")

static DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	
	int retval;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	printf("[TCP SERVER] Current Thread Id: %d\n", GetCurrentThreadId());
	
	// ������ Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			break;
		}
	}

	closesocket(client_sock);
	printf("[TCP SERVER] Ŭ���̾�Ʈ ����(%s:%d)\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

void MultiThreadServerEx() {
	int retval;

	// Winsock �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	// ���� ���� ����
	SOCKET listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) exit(1);

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) exit(1);

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) exit(1);

	// ������ ���
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) break;

		// ������ ����
		hThread = (HANDLE)_beginthreadex(NULL, 0, ProcessClient, (LPVOID)client_sock, CREATE_SUSPENDED, NULL);
		if (hThread == NULL) closesocket(client_sock);
		else {
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}