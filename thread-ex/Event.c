#include <windows.h>
#include <stdio.h>

#define BUFSIZE 10

static HANDLE hWriteEvent; /* ���� �Ϸ� �˸� �̺�Ʈ */
static HANDLE hReadEvent;  /* �б� �Ϸ� �˸� �̺�Ʈ */
static buf[BUFSIZE];

// 1. Thread1�� ���� ���ۿ� ������ ���� �۾��� �Ѵ�.
// 2. Thread2, Thread3�� ���� ���ۿ� ������ �����͸� �д´�.
// 3. Thread2�� Thread3 �� �ϳ��� Thread�� ���� ���ۿ� ������ �����ϴ�.
// 4. ���۸� ���� Thread�� �۾��� ������ Thread1�� �ٽ� ���� �۾��� �� �� �ִ�.

static DWORD WINAPI WriteThread(LPVOID arg) {
	DWORD retval;

	for (int k = 1; k <= 500; k++) {
		// �б� �Ϸ� ���(= �б� Thread�� ��ȣ ���·� ����Ǹ� Wakeup)
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		// ���� ���ۿ� ������ ����
		for (int i = 0; i < BUFSIZE; i++) {
			buf[i] = k;
		}

		// ���� �Ϸ� �˸�
		SetEvent(hWriteEvent);
	}

	return 0;
}

static DWORD WINAPI ReadThread(LPVOID arg) {
	DWORD retval;

	while (1) {
		// ���� �Ϸ� ���(= ���� Thread�� ��ȣ ���·� ����Ǹ� Wakeup)
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		// ���� �����͸� ����ϰ� ���� �ʱ�ȭ
		// ���� �ʱ�ȭ: �����͸� ���� ���� ���¿��� ���� �� 0�� ������ ����� ���� ���� Ȯ���� ����
		printf("Thread %4d:\t", GetCurrentThreadId());
		for (int i = 0; i < BUFSIZE; i++) printf("%3d ", buf[i]);
		printf("\n");
		memset(buf, 0, BUFSIZE);

		// �б� �Ϸ� �˸�
		SetEvent(hReadEvent);
	} 

	return 0;
}

void EventEx() {
	// �̺�Ʈ ����
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hWriteEvent == NULL || hReadEvent == NULL) {
		return 1;
	}

	// Thread 3�� ����
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	// �б� �Ϸ� �˸�(= �б� �̺�Ʈ�� ��ȣ ���·� ����)
	SetEvent(hReadEvent);

	// Thread 3�� ���� ���
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	// �̺�Ʈ ����
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);

	return 0;
}
