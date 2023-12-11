#include "common.h"

#define BUFSIZE     50

void CloseOnTransferClient6() {
	int retval;
	
	// ���� & ����
	int i, len;
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���� �׷��׿�"
	};

	// ������ ������ ���
	for (i = 0; i < 3; i++) {
		SOCKET sock = *InitClientSocket6(&retval, NULL);

		len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);
		
		// ������ ����
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		printf("[TCP CLIENT] %d����Ʈ ����\n", retval);
		closesocket(sock);
	}
}