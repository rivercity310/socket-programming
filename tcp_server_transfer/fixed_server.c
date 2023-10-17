#include "common.h"

#define BUFSIZE 50


void FixedTcpServer6() {
	int retval = NULL;
	SOCKET sock = *InitServerSocket6(&retval);
	
	SOCKET clientsock;
	struct sockaddr_in6 clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		addrlen = sizeof(clientaddr);
		clientsock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (clientsock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ���� Ŭ���̾�Ʈ ���� ���
		char addr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("[TCP SERVER] Ŭ���̾�Ʈ ����: IP = %s, PORT = %d\n", addr, ntohs(clientaddr.sin6_port));

		// ������ ���
		while (1) {
			retval = recv(clientsock, buf, BUFSIZE, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin6_port), buf);
		}

		closesocket(clientsock);
	}

	closesocket(sock);
}
