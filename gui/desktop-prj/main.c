#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define SERVERPORT 9000
#define BUFSIZE    512

// extern
extern DWORD WINAPI ServerMain(LPVOID arg);
extern DWORD WINAPI ProcessClient(LPVOID arg);

// Critical Section
CRITICAL_SECTION cs;

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(const char* fmt, ...);

HINSTANCE hInst;      // �ν��Ͻ� �ڵ�: ���� �Լ����� ���� ���� WinMain�� hInstance ���� ���� ������ ��´�.
HWND hEdit;           // ����Ʈ ��Ʈ��

// �ܼ� ���� ���α׷��� main() �Լ��� ���� ����� �ϴ� GUI ���� ���α׷��� ���� ������
int WINAPI WinMain(
	HINSTANCE hInstance,		// �ν��Ͻ� �ڵ�: ���� ���Ͽ� ���Ե� ���� ���ҽ��� ������ �� ���
	HINSTANCE hPrevInstance,	// 16��Ʈ ������ ���� ����, ����� ��� X
	LPSTR lpCmdLine,			// ���α׷� ���� �� ���޵� ����� �μ��� ��� �ִ� ���ڿ�
	int nCmdShow				// ���α׷��� ������ �� ���� �����츦 ���� ����� ����(�ִ�ȭ, �ּ�ȭ ��)
) {

	hInst = hInstance;
	InitializeCriticalSection(&cs);

	// ������ Ŭ���� ���
	// - Window Class : �������� �پ��� Ư���� �����ϴ� ����ü, ���ϴ� ������ �ʱ�ȭ�� �� RegisterClass() �Լ��� �ü���� ���
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("MyWndClass");
	if (!RegisterClass(&wndclass)) return 1;


	// ������ ����
	// - ������ ����� Window Class�� ������� ���� ������ ����
	// - CreateWindow() �Լ��� �����츦 ������ �� ������ �ڵ�(HWND Ÿ��)�� ����
	// - ������ �ڵ��� �پ��� API �Լ��� ���������ν� �����츦 �����Ӱ� ������ �� ����
	// - ������ ������� ó���� ������ �����Ƿ�, ShowWindow(), UpdateWindow()�� ���ʷ� ȣ���Ͽ� ȭ�鿡 ���̰� ��
	HWND hWnd = CreateWindow(
		_T("MyWndClass"),		// _T ��ũ��: tchar.h ������� ����, �����Ϸ� ������ ���� ANSI �Ǵ� �����ڵ� ���ڿ��� �ڵ� ����, ������ API �Լ��� ���ڿ� ���޽� ���
		_T("WinApp"),
		WS_OVERLAPPEDWINDOW,
		0, 0, 500, 220, 
		NULL, NULL, 
		hInstance, NULL
	);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	_beginthreadex(NULL, 0, ServerMain, NULL, 0, NULL);

	// �޼��� ����
	// - GetMessage() : ���� ���α׷��� �Ҵ�� �޼��� ť���� �޼����� ����
	// - TranslateMessage() : Ű���� �޽����̸� Ư���� ó���� �Ѵ�.
	// - DispatchMessage() : �ش� �޽����� ������ ���ν����� ���� 
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteCriticalSection(&cs);
	return (int)msg.wParam;
}


// ������ ���ν���: ������ �޽����� ó���ϴ� �ٽ� �Լ� 
// - WM_CREATE: �����찡 �����Ǿ��� �� �߻��ϴ� �޽��� ó��
// - WM_SIZE: ũ�Ⱑ ����Ǿ��� �� �߻��ϴ� �޽��� ó��
// - WM_DESTROY: ���� ��ư�� ������ �� �߻��ϴ� �޽��� ó��
// ���� ���α׷��� ó������ ���� �޽����� DefWindowProc() �Լ��� �����Ͽ� ������ �ü���� �˾Ƽ� ó���ϰ� �Ѵ�.
LRESULT CALLBACK WndProc(
	HWND hWnd,		// �޽����� �߻��� �����츦 ��Ÿ���� �ڵ鰪
	UINT uMsg,		// �߻��� �޽����� ��Ÿ���� ������, WM_* ������ ����� ���ǵǾ� �ִ�.
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uMsg) {
	case WM_CREATE:		// WM_CREATE: ���� ������ ������ �� �߻�. ���� �ʱ�ȭ �۾� ���࿡ ����(���� �޸� �Ҵ�, �ڽ� ������ ����, ���� ���� ��) 
		hEdit = CreateWindow(_T("edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_HSCROLL |			// WS_* ���: �Ϲ� ������� ��Ʈ�ѿ� ��� ������ �� �ִ� ���� ��Ÿ��
			WS_VSCROLL | ES_AUTOHSCROLL |					// ES_* ���: ����Ʈ ��Ʈ�ѿ��� ������ �� �ִ� ��Ÿ��
			ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL
		);

		return 0;

	case WM_SIZE:		// WM_SIZE: ���� �������� ũ�Ⱑ ����� ������ �߻�. lParam�� ���� 16��Ʈ�� ���� 16��Ʈ���� ���� Ŭ���̾�Ʈ ������ ���� ���̰� ��� �ִ�.
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_SETFOCUS:	// WM_SETFOCUS: ���� �����찡 Ű���� ��Ŀ���� ���� �� �߻�. SetFocus() �Լ��� ���� ���� �������� Ű���� ��Ŀ���� �ٸ� ������� ��ȯ
		SetFocus(hEdit);
		return 0;

	case WM_DESTROY:	// WM_DESTROY: ���� �����츦 ���� �� �߻�. ���� û�� �۾� ����(���� �޸� ����, ���� �ݱ� ��)
		PostQuitMessage(0);	 // PostQuitMessage(): ���� ���α׷� �޼��� ť�� WM_QUIT �޽����� �������ν� GetMessage() �Լ��� 0�� �����ϰ� �Ѵ�.
		return 0;
	}

	// ���� ���α׷��� ó������ ���� �޽����� DefWindowProc() �Լ��� �����Ͽ� ������ �ü���� �˾Ƽ� ó���ϰ� �Ѵ�.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	
}


// ����Ʈ ��Ʈ�ѿ� ����� �����ϴ� �Լ�
void DisplayText(const char* fmt, ...) {
	// va_list Ÿ�԰� va_start(), va_end()�� ���� ���� �μ��� ������ ���� ǥ�� C ���̺귯�� �Լ����� ����
	// ����Ʈ ��Ʈ�ѿ� ���� ����� ���ڿ��� vsprintf() �Լ��� �̿��Ͽ� cbuf[] �迭�� �̸� ����صд�.
	va_list arg;
	va_start(arg, fmt);
	char cbuf[BUFSIZE * 2];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	EnterCriticalSection(&cs);

	// GetWindowTextLength() �Լ��� ����Ʈ ��Ʈ�ѿ� ��� �ִ� ���ڿ��� ���̸� ����.
	// 132���� cbuf[]�� ����ִ� ���ڿ��� ������ API�� �����ϹǷ� ANSI �Լ� SendMessageA() �Լ��� ����ؾ� �Ѵ�.
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

	LeaveCriticalSection(&cs);
}
