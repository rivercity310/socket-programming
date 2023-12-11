#include "common.h"

#define BUFSIZE 1024

void CloseOnTransferServer6() {
	int retval;
	SOCKET sock = *InitServerSocket6(&retval);
	
	// ������ '�۽�' ���� ũ�� Ȯ��
	int nBufSize = 0, nLen = sizeof(nBufSize);
	if (getsockopt(sock, SOL_SOCKET, 
		SO_SNDBUF, (char*)&nBufSize, &nLen) != SOCKET_ERROR) {
		printf("Send Buffer size: %d\n", nBufSize);
	}

	// ������ '����' ���� ũ�� Ȯ��
	nBufSize = 0; nLen = sizeof(nBufSize);
	if (getsockopt(sock, SOL_SOCKET,
		SO_RCVBUF, (char*)&nBufSize, &nLen) != SOCKET_ERROR) {
		printf("Receive Buffer size: %d\n", nBufSize);
	}

	SOCKET client_sock;
	struct sockaddr_in6 clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("\n[TCP SERVER] Ŭ���̾�Ʈ ����: IP=%s, PORT=%d\n", addr, ntohs(clientaddr.sin6_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin6_port), buf);
		}

		closesocket(client_sock);
		printf("[TCP SERVER] Ŭ���̾�Ʈ ����: IP=%s, PORT=%d\n", addr, ntohs(clientaddr.sin6_port));
	}

	closesocket(sock);
}