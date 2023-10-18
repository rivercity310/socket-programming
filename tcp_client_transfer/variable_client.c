#include "common.h"

#define BUFSIZE    50
#define CLIENTPORT 50000

void VariableTcpClient4() {
	int retval;
	SOCKET sock = *InitClientSocket4(&retval, CLIENTPORT);
	
	// ������ ��ſ� ����� ����
	int len, i;
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�"
	};

	for (i = 0; i < 4; i++) {
		len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);
		buf[len++] = '\n';
	
		// ������ ����
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		printf("[TCP CLIENT] %d����Ʈ�� �����߽��ϴ�.\n", retval);
	}

	closesocket(sock);
	return;
}
