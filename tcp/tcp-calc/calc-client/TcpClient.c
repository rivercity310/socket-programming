#include "common.h"


#define SERVERIP    "127.0.0.1"
#define SERVERPORT  9000
#define CLIENTPORT  50000

#pragma comment(lib, "ws2_32")

struct calc_args {
	int a, b;
};

void TcpClientStart() {
	// WinSock �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("winsock init fail");
	}

	// Socket �ʱ�ȭ
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	struct calc_args calcargs;
	int* tmp = malloc(sizeof(int));
	int i;
	int len = sizeof(calcargs);

	while (1) {
		// ����� �Է¹ޱ�
		puts("[�� ���� �Է��ϸ� ���� ����� �����ݴϴ�]");

		for (i = 0; i < len / sizeof(int); i++) {
			printf("������ %d: ", i + 1);
			if (scanf("%d", tmp) == 0 || tmp == NULL) return;
			if (i == 0) calcargs.a = *tmp;
			else calcargs.b = *tmp;
			printf("�Է��� ������: %d\n", *tmp);
		}

		// send()
		retval = send(sock, (const char*)&calcargs, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// recv()
		retval = recv(sock, (char*)&calcargs, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

		// ���� ������ ���
		printf("[���� ���] %d\n", calcargs.a);
		putchar('\n');
	}

	if (tmp != NULL) free(tmp);

	// Socket ����
	closesocket(sock);

	// WinSock ����
	WSACleanup();
}

