#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#pragma comment(lib, "ws2_32")

static bool getDomainName(struct in_addr addr, char* name, int namelen) {
	HOSTENT* ptr = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_display("getDomainName()");
		return false;
	}

	if (ptr->h_addrtype != AF_INET) {
		return false;
	}

	strncpy(name, ptr->h_name, namelen);
	return true;
}

static SOCKET* TcpServer() {
	// Server Socket ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	int retval;

	// Socket Struct Init & Socket Bind
	SOCKADDR_IN serveraddr;
	//memset(&serveraddr, 0, sizeof(serveraddr));
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY(0.0.0.0) : ��� ���� ��û IP ���
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
	}

	// Socket Listen : SOMAXCONN -> OS Socket Max Connection
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
	}

	// Socket Accept (��û�� Ŭ���̾�Ʈ ���� ���� ���)
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept() : Blocking Socket -> Client ���ӱ��� ���
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char name[256];
		getDomainName(clientaddr.sin_addr, name, sizeof(name));
		printf(
			"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d, DNS = %s\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port),
			name
		);

		// Ŭ���̾�Ʈ�� ���� ������ �ޱ�
		while (1) {
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			// Ŭ���̾�Ʈ���� ���� ������ ��� retval = 0
			else if (retval == 0) {
				break;
			}

			buf[retval] = '\0';
			printf(
				"[TCP/%s %d %s] %s\n",
				inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port),
				name,
				buf
			);

			// �״�� Ŭ���̾�Ʈ�� ���� (����)
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		closesocket(client_sock);
		printf(
			"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d, DNS = %s\n",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port),
			name
		);
	}
	
	return &listen_sock;
}

int main() {
	// WINSOCK �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 0;
	}

	SOCKET* server_sock = TcpServer();
	
	// Socket Close
	closesocket(*server_sock);

	// WINSOCK ����
	WSACleanup();
}