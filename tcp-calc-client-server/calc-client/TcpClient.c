#include "common.h"

#define SERVERIP    "127.0.0.1"
#define SERVERPORT  9000
#define CLIENTPORT  50000
#define BUFSIZE     64

#pragma comment(lib, "ws2_32")

void TcpClientStart() {
	// WinSock �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("winsock init fail");
	}

	// Socket �ʱ�ȭ
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	int retval;
	
	// Client ���� ����ü �ʱ�ȭ & bind()
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(CLIENTPORT);
	clientaddr.sin_addr.s_addr = INADDR_ANY;
	retval = bind(sock, (struct sockaddr*)&clientaddr, sizeof(clientaddr));


	// Server ���� ����ü �ʱ�ȭ & connect() 
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
	}

	// Server�� ������ ���
	char buf[BUFSIZE + 1];
	int len, i;

	while (1) {
		// ����� �Է¹ޱ�
		puts("[�� ���� �Է��ϸ� ���� ����� �����ݴϴ�]");

		for (i = 0; i < 2; i++) {
			printf("������ %d: ", i + 1);
			if (fgets(buf, BUFSIZE, stdin) == NULL) {
				break;
			}

			// ����Ű �� NULL�� ġȯ
			len = strlen(buf);
			if (buf[len - 1] == '\n') {
				buf[len - 1] = '\0';
			}

			// send()
			retval = send(sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// recv()
			retval = recv(sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			printf("[TCP CLIENT] �������� ���� %s\n", buf);
		}

		// recv()
		retval = recv(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

		// ���� ������ ���
		printf("[���� ���] %s\n", buf);
		putchar('\n');
	}

	// Socket ����
	closesocket(sock);

	// WinSock ����
	WSACleanup();
}

