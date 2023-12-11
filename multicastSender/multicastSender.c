#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIPv4  "235.7.8.9"
#define MULTICASTIPv6  "FF12::1:2:3:4"
#define REMOTEPORT     9000
#define BUFSIZE        512

#pragma comment(lib, "ws2_32")

static DWORD WINAPI MultiCastSenderv4(LPVOID arg);
static DWORD WINAPI MultiCastSenderv6(LPVOID arg);
static void SendMulticastData(SOCKET sock, struct sockaddr* remoteaddr, int addrlen);

void MulticastSender() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	HANDLE hThread[2];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, MultiCastSenderv4, NULL, CREATE_SUSPENDED, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, MultiCastSenderv6, NULL, CREATE_SUSPENDED, NULL);
	
	int val;
	printf("Ip Version: ");
	scanf_s("%d", &val);
	getchar();

	if (val == 4) ResumeThread(hThread[0]);
	else if (val == 6) ResumeThread(hThread[1]);
	else {
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);
	}

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	WSACleanup();
	return 0;
}

static DWORD WINAPI MultiCastSenderv4(LPVOID arg) {
	int retval;

	SOCKET sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) exit(1);

	// ��Ƽĳ��Ʈ TTL ����
	DWORD ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(const char*)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) exit(1);

	// ���� �ּ� ����ü �ʱ�ȭ
	struct sockaddr_in remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	inet_pton(PF_INET, MULTICASTIPv4, &remoteaddr.sin_addr);
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(REMOTEPORT);

	SendMulticastData(sock, &remoteaddr, sizeof(remoteaddr));

	closesocket(sock);
	return 0;
}

static DWORD WINAPI MultiCastSenderv6(LPVOID arg) {
	int retval;

	SOCKET sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) exit(1);

	// ��Ƽĳ��Ʈ TTL ����
	DWORD ttl = 2;
	retval = setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
		(const char*)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) exit(1);

	// ���� �ּ� ����ü �ʱ�ȭ
	struct sockaddr_in6 remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	inet_pton(PF_INET6, MULTICASTIPv6, &remoteaddr.sin6_addr);
	remoteaddr.sin6_family = AF_INET6;
	remoteaddr.sin6_port = htons(REMOTEPORT);

	SendMulticastData(sock, &remoteaddr, sizeof(remoteaddr));

	closesocket(sock);
	return 0;
}

static void SendMulticastData(SOCKET sock, struct sockaddr* remoteaddr, int addrlen) {
	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len, retval;

	// ��Ƽĳ��Ʈ ������ ������
	while (1) {
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		// ������ ������
		retval = sendto(sock, buf, (int)strlen(buf), 0,
			remoteaddr, addrlen);
		if (retval == SOCKET_ERROR) {
			printf("\n ���� ����!!!\n");
			exit(1);
		}

		printf("[UDP] %d����Ʈ�� ���½��ϴ�.\n", retval);
	}
}