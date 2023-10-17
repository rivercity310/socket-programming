#include "common.h"

#define BUFSIZE 512
#define EOR     '\n'  /* \n�� EOR�� ���� */


/* ���� ���� �Լ� recv()���� �����͸� �ѹ��� �а� ��û���� 1����Ʈ�� ���� */
int _recv_ahead(SOCKET s, char* p) {
	__declspec(thread) static int nbytes = 0;
	__declspec(thread) static char buf[BUFSIZE * 2];
	__declspec(thread) static char* ptr;

	if (nbytes == 0 || nbytes == SOCKET_ERROR) {
		nbytes = recv(s, buf, sizeof(buf), 0);
		if (nbytes == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (nbytes == 0) {
			return 0;
		}
		ptr = buf;
	}

	--nbytes;
	*p = *ptr++;
	return 1;
}

/* ����� ���� ������ ���� �Լ� */
int recvline(SOCKET s) {

}

void VariableTcpServer4() {
	int retval = NULL;
	SOCKET sock = InitServerSocket4(&retval);
	
	// TODO..

	closesocket(sock);
	return;
}