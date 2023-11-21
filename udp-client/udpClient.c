#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <winsock2.h>

#define SERVERADDR4  "127.0.0.1"
#define SERVERPORT4  9000
#define SERVERADDR6  "::1"
#define SERVERPORT6  9090
#define BUFSIZE    512

#pragma comment(lib, "ws2_32")

static void UDPClient4();
static void UDPClient6();

void UDPClient() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		exit(1);
	}

	int version;
	printf("������ IP Version�� �����ϼ���: ");
	scanf_s("%d", &version);
	getchar();

	if (version == 4) UDPClient4();
	else UDPClient6();

	WSACleanup();
}

static void UDPClient4() {
	// ���� ����
	SOCKET sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) exit(1);

	int retval;

	// ���� �ּ� ����ü �ʱ�ȭ
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	inet_pton(AF_INET, SERVERADDR4, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT4);
	serveraddr.sin_family = PF_INET;

	// ������ ��ſ� ����� ����
	struct sockaddr_in peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}

		// �����̽��� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) {
			break;
		}

		// ������ ����
		retval = sendto(sock, buf, (int)strlen(buf), 0,
			(struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) exit(1);

		printf("[UDP Client] %d����Ʈ ���� �Ϸ�\n", retval);

		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(struct sockaddr*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) exit(1);

		// �۽��� IP �ּ� üũ
		if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) {
			printf("[����] �߸��� �������Դϴ�!\n");
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP Client] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);
	}

	closesocket(sock);
}

static void UDPClient6() {
	SOCKET sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) exit(1);

	int retval;

	// ���� ����ü �ʱ�ȭ
	struct sockaddr_in6 serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	inet_pton(AF_INET6, SERVERADDR6, &serveraddr.sin6_addr);
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_port = htons(SERVERPORT6);

	// ������ ��ſ� ����� ����
	struct sockaddr_in6 peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}

		// �����̽��� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) {
			break;
		}

		// ������ ����
		retval = sendto(sock, buf, (int)strlen(buf), 0,
			(struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) exit(1);

		printf("[UDP Client] %d����Ʈ ���� �Ϸ�\n", retval);

		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(struct sockaddr*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) exit(1);

		// �۽��� IP �ּ� üũ
		if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) {
			printf("[����] �߸��� �������Դϴ�!\n");
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP Client] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);
	}

	closesocket(sock);
}