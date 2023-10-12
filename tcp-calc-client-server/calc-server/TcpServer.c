#include "common.h"

#define SERVERPORT 9000
#define BUFSIZE    64

#pragma comment(lib, "ws2_32")

void TcpServerStart() {
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

	// Server Socket ����ü �ʱ�ȭ & bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
	}

	SOCKET clientsock;
	struct sockaddr_in clientaddr;
	char buf[BUFSIZE];
	int len = sizeof(clientaddr);
	int i, sum, received;

	while (1) {
		clientsock = accept(sock, (struct sockaddr*)&clientaddr, &len);
		if (clientsock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		puts("[TCP SERVER] Ŭ���̾�Ʈ ����!");
		printf("IP = %s, PORT = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		while (1) {
			sum = 0;

			for (i = 0; i < 2; i++) {
				retval = recv(clientsock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 1) {
					printf("[TCP SERVER] Client�κ��� ���� ����\n");
					break;
				}

				printf("[TCP SERVER] %d ����Ʈ�� �޾ҽ��ϴ�. %s\n", retval, buf);
				received = atoi(buf);
				sum += received;

				sprintf(buf, "%d", received);
				retval = send(clientsock, buf, retval, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				printf("[TCP SERVER] %d ���� Ŭ���̾�Ʈ�� ����\n", received);
			}

			sprintf(buf, "%d", sum);
			retval = send(clientsock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		closesocket(clientsock);
	}

	// Socket Close
	closesocket(sock);

	// WinSock Close
	WSACleanup();
}
