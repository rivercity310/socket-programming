#define _WINSOCK_DEPRECATED_NO_WARNINGS  // ���� ���� API ��� �� ��� ���� 
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����

#include "Common.h"

#define TESTNAME "www.naver.com"

extern void err_display(const char*);

/* ������ �̸� -> IPv4 �ּ� */
bool getIPAddr(const char* name, struct in_addr* addr)
{
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}

	if (ptr->h_addrtype != AF_INET) {
		return false;
	}

	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

/* IPv4 �ּ� -> ������ �̸� */
bool getDomainName(struct in_addr addr, char* name, int namelen) 
{
	struct hostent* ptr = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
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

void DNSTest()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return;
	}

	printf("[��ȯ ��] ������ �̸� = %s\n", TESTNAME);
	
	// ������ �̸� -> IP �ּ�
	struct in_addr addr;
	if (getIPAddr(TESTNAME, &addr)) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr, str, sizeof(str));    // numeric(network) to presentation
		printf("[��ȯ ��] IP �ּ� = %s\n", str);

		// IP �ּ�(Network Byte Address) -> ������ �̸�
		char name[256];
		if (getDomainName(addr, name, sizeof(name))) {
			printf("[�纯ȯ] ������ �̸� = %s\n", name);
		}
	}

	WSACleanup();

}