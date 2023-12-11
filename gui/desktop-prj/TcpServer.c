#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERPORT 9000
#define BUFSIZE    512

#pragma comment(lib, "ws2_32")

extern void DisplayText(const char* fmt, ...);
DWORD WINAPI ProcessClient(LPVOID arg);

void DisplayQuit(const char* msg)
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
void DisplayError(const char* msg) {
	LPVOID lpMsgBuf;
	
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL
	);
	
	DisplayText("[%s] %s\r\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ���� ��� ������ �Լ�
DWORD WINAPI ServerMain(LPVOID arg) {
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) DisplayQuit("socket()");

	// bind
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) DisplayQuit("bind()");

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) DisplayQuit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			DisplayError("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(PF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		DisplayText("\r\n[TCP Server] Ŭ���̾�Ʈ ����: %s:%d\r\n", addr, htons(clientaddr.sin_port));

		// ������ ����
		hThread = (HANDLE)_beginthreadex(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);

		if (hThread == NULL) closesocket(client_sock);
		else CloseHandle(hThread);
	}

	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(PF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			DisplayError("recv()");
			break;
		}

		else if (retval == 0) {
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		DisplayText("[TCP/%s:%d] %s\r\n", addr, ntohs(clientaddr.sin_port), buf);

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			DisplayError("send()");
			break;
		}
	}

	closesocket(client_sock);
	DisplayText("[TCP Server] Ŭ���̾�Ʈ ����: %s:%d\r\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

