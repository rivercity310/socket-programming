#include "common.h"

#define BUFSIZE 50

void FixedVariableTcpClient4() {
	int retval;
	SOCKET sock = *InitClientSocket4(&retval, NULL);

	// ������ ��ſ� ����� ����
	int i, len;
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ �� ���ƿ�",
		"���� �׷��׿�"
	};

	// ������ ������ ���
	for (i = 0; i < 4; i++) {
		len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);

		// ���� ���� ������ ����
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// ���� ���� ������ ����
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		printf("[TCP CLIENT] %d+%d����Ʈ ����\n", (int)sizeof(int), retval);
	}

	closesocket(sock);
}
