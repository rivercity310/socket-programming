#include "common.h"

#define BUFSIZE 50

void FixedTcpClient6() {
	int retval = NULL;
	SOCKET sock = *InitClientSocket6(&retval, 50000);

	// ���� & ����
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���� �׷��׿�"
	};

	for (int i = 0; i < 3; i++) {
		memset(buf, '#', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		
		printf("[TCP CLIENT] %d ����Ʈ ����\n", retval);
	}

	closesocket(sock);
	return;
}