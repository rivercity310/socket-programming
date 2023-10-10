#include "Common.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

#pragma comment(lib, "ws2_32")

// ����� ���� ������ ���� �Լ�
static int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}


void TCPClient() {
	int retval;

	// WINSOCK INIT
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return;
	}

	// Client Socket ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// TCP/IP Server Socket �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	// CONNECT : OS�� �ڵ����� Client ���� IP, PORT �Ҵ� (���� ������ bind() �Լ� ���)
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// DATA ���
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		// �Է¹ޱ�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}

		// ������ ����Ű���� NULL ������ ġȯ
		len = sizeof(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}

		// ������ ������ (send(): ���� byte �� ��ȯ)
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP CLIENT] %d ����Ʈ�� ���½��ϴ� \n", retval);

		// ������ �ޱ�
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ� \n", retval);
		printf("[���� ������] %s\n", buf);
	}

	// SOCKET �ݱ�
	closesocket(sock);

	// WINSOCK ����
	WSACleanup();
}
