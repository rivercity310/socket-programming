#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread3(LPVOID arg) {
	while (1);
	return 0;
}

void SchedulingEx() {
	// �켱���� ��(������ �켱����, �켱���� ����) ���� ���
	printf("Priority: %d ~ %d\n", THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);

	// CPU ���� �˾Ƴ���
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	printf("CPU ����: %d\n", (int)si.dwNumberOfProcessors);

	// CPU ������ŭ ������ ����
	for (int i = 0; i < (int)si.dwNumberOfProcessors; i++) {
		HANDLE hThread = CreateThread(NULL, 0, MyThread3, NULL, 0, NULL);
		if (hThread == NULL) return 1;

		// Main Thread���� �켱 ������ ���δ�.
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		CloseHandle(hThread);
	}

	// Main Thread�� �켱 ������ ���� �����Ѵ�
	// ���(Starvation) ������ �߻��ϴ��� üũ 
	// -> Windows CPU Scheduling������ �����ȸ�� ���� ���� �������� �켱������ �ܰ������� ����ø�
	// -> ��������� Main Thread�� CPU�� ���� ������ ��� �ܼ��� ������ �Ǿ��ٰ�, Main Thread�� ���� ��ȸ�� ������ ���������� �Լ��� ����� �� ����ȴ�.
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
	Sleep(1000);
	printf("Main Thread ����!\n");
	return 0;
}