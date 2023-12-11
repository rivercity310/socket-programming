#include <windows.h>
#include <stdio.h>

#define MAXCNT 10000000

static int count = 0;

static DWORD WINAPI Thread1(LPVOID arg) {
	for (int i = 0; i < MAXCNT; i++) {
		count += 2;
	}

	printf("Thread ID: %d, count: %d\n", GetCurrentThreadId(), count);

	return 0;
}

static DWORD WINAPI Thread2(LPVOID arg) {
	for (int i = 0; i < MAXCNT; i++) {
		count -= 2;
	}
	
	printf("Thread ID: %d, count: %d\n", GetCurrentThreadId(), count);

	return 0;
}

// ���� �ڿ� ���ٽ� ���� �߻�
void CriticalSectionEx() {
	HANDLE hThread[2];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, Thread1, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, Thread2, NULL, 0, NULL);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	printf("count = %d\n", count);
	return 0;
}

// ---

// �Ϲ� ����ȭ ��ü(Ŀ�� �޸� ����)�� �޸�, �Ӱ迵���� ����� �޸𸮿� ����
// �Ϲ������� �� ���μ��� �� �����忡���� ���� ����������, ���� �޸� ���� ���� �ٸ� ���μ����� �Ӱ迵�� ���� ����
static CRITICAL_SECTION cs;

static DWORD WINAPI Thread3(LPVOID arg) {
	EnterCriticalSection(&cs);
	
	for (int i = 0; i < MAXCNT; i++) {
		count += 2;
	}

	LeaveCriticalSection(&cs);
	
	return 0;
}

static DWORD WINAPI Thread4(LPVOID arg) {
	EnterCriticalSection(&cs);
	
	for (int i = 0; i < MAXCNT; i++) {
		count -= 2;
	}

	LeaveCriticalSection(&cs);

	return 0;
}

void CriticalSectionEx2() {
	// �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

	HANDLE hThread[2];

	// Create*() �Լ��� Ŀ�� �޸� ������ ����ȭ ��ü�� �����ϰ�, �̿� ������ �� �ִ� �ڵ��� �����Ѵ�.
	hThread[0] = CreateThread(NULL, 0, Thread3, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, Thread4, NULL, 0, NULL);

	//������ �ΰ� ���� ���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	// �Ӱ� ���� ����
	DeleteCriticalSection(&cs);

	printf("count = %d\n", count);
	return 0;
}
