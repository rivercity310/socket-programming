#include "common.h"

#define BUFSIZE    50

void FixedTcp6Client() {
	int retval = NULL;
	SOCKET sock = *InitClientSocket6(&retval, NULL);

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
		if (retval == SOCKET_ERROR) exit(1);
		printf("[TCP CLIENT] %d ����Ʈ ����\n", retval);
	}

	closesocket(sock);
	return;
}