#define WINSOCK_DEPRECATED_NO_WARNINGS

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

static void ServerSocket() {
	// Server Socket ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	int retval;

	// Socket Bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY: ��� ���� ��û IP ��� 
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	// Socket Listen
	retval = listen(listen_sock, SOMAXCONN); // Socket Max Connection
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
	}

	// Socket Accept (��û�� Ŭ���̾�Ʈ ���� ���� ���)
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		printf(
			"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port)
		);

		// Ŭ���̾�Ʈ�� ���� ������ �ޱ�
		while (1) {
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			// Ŭ���̾�Ʈ���� ���� ���� ������ ��� 0 ��
			else if (retval == 0) {
				break;
			}

			buf[retval] = '\0';
			printf("[TCP/%s %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			// �״�� Ŭ���̾�Ʈ�� ������ ������
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		closesocket(client_sock);
		printf(
			"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port)
		);
	}
}

void TCPClient() {
	int retval;

	// WINSOCK INIT
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return;
	}

	ServerSocket();

	// Client Socket ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// TCP/IP Server Socket �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	// CONNECT
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
