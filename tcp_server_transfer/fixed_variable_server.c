#include "common.h"

#define BUFSIZE 512

void FixedVariableTcpServer4() {
	int retval;
	SOCKET sock = *InitServerSocket4(&retval);

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;				 // ���� ���� ������
	char buf[BUFSIZE + 1];	 // ���� ���� ������

	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP SERVER] Ŭ���̾�Ʈ ����: IP=%s, PORT=%d\n", addr, ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// 1. ���� ���� ������ �ޱ�
			retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// 2. ���� ���� ������ �ޱ�
			retval = recv(client_sock, buf, len, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
		}

		closesocket(client_sock);
		printf("[TCP SERVER] Ŭ���̾�Ʈ ����: IP=%s, PORT=%d\n", addr, ntohs(clientaddr.sin_port));
	}

	closesocket(sock);
}