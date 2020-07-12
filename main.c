#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define _WIN32_WINNT 0x0500
#define WINDOW_WIDTH    1280  // ソフト起動後に表示する初期ウインドウ横サイズ
#define WINDOW_HEIGHT   720   // ソフト起動後に表示する初期ウインドウ縦サイズ

#define BUTTON_ID1 0 // Start Button
#define BUTTON_ID2 1 // End Button
#define BUTTON_IDA 2 // Start Button
#define BUTTON_IDB 3 // End Button

RECT recDisplay, recWindow, recClient;

void DoGetActiveWindow(); // DoGetActiveWindow() の宣言

int main() // プログラム実行後に実行される関数？
{
	HWND hWnd = GetConsoleWindow(); // コンソールウインドウ取得
	ShowWindow(hWnd, SW_MINIMIZE);  // コンソールウインドウ最小化？
	ShowWindow(hWnd, SW_HIDE);      // コンソールウインドウ隠す
	DoGetActiveWindow();            // DoGetActiveWindow() の実行
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	RECT rec;
	switch (msg) {  // 投げられたメッセージを発見(?)
	/**case WM_SIZE:
		GetWindowRect(hwnd, &rec);
		HWND hChild = GetWindow(hwnd, GW_CHILD);
		while (hChild) {
			hChild = GetWindow(hChild, GW_HWNDNEXT);
			
			SetWindowPos(hChild, hChild, );
		}
		return 0;**/
	case WM_DESTROY:                                                                      // ウインドウを閉じた場合
		PostQuitMessage(0);                                                               // システムを終わらす
		return 0;
	case WM_COMMAND:                                                                      // コマンドであった場合
		switch (LOWORD(wp)) {                                                             // ???
		case BUTTON_ID1:                                                                  // ボタン１であった場合
			MessageBox(hwnd, TEXT("BUTTON_ID1"), TEXT("Kitty"), MB_OK);                   // OK
			break;
		case BUTTON_ID2:                                                                  // ボタン２であった場合
			DestroyWindow(hwnd);                                                          // システムを停止
			break;
		case BUTTON_IDA:                                                                  // ボタンAであった場合
			MessageBox(hwnd, TEXT("BUTTON_IDA"), TEXT("Kitty"), MB_CANCELTRYCONTINUE);    // キャンセル, 続行 or OK
			break;
		case BUTTON_IDB:                                                                  // ボタンであった場合
			MessageBox(hwnd, TEXT("BUTTON_ID1"), TEXT("Kitty"), MB_HELP);                 // ヘルプ
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

/* アクティブウィンドウのタイトルを表示する */
void DoGetActiveWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	HWND hwnd, hWnd, hDeskWnd;
	WNDCLASS winc;
	MSG msg;

	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, &recDisplay);

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WindowProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, NULL);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("MainWindow");

	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindow(
		TEXT("MainWindow"),
		TEXT("検定対策ソフト"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(recDisplay.right - WINDOW_WIDTH) / 2, //左上x座標
		(recDisplay.bottom - WINDOW_HEIGHT) / 2, //左上y座標
		WINDOW_WIDTH, //幅
		WINDOW_HEIGHT, //高さ
		NULL, NULL,
		hInstance, NULL
	);
	HDC hdc;
	RECT rect;
	int width = WINDOW_WIDTH;
	if (GetWindowRect(hwnd, &rect))
	{
		width = rect.right - rect.left;
	}
	width = (width / 2) - 200;
	hdc = GetDC(hwnd);
	LPTSTR lptStr = _T("検定対策用ソフト");
	TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
	ReleaseDC(hwnd, hdc);
	UpdateWindow(hwnd);

	/*
		ウインドウサイズの変更でボタンの位置がズレてしまう問題あり
		ウインドウサイズの変更メッセージは WindowProc() にて取得可能(メッセージ関数は WM_SIZE )

		また、ソフト起動時にウインドウが最前面に来ない ← 4Pで修正予定
	*/
	CreateWindow(
		TEXT("BUTTON"),
		TEXT("START"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		300, // 基準の高さ(Y座標は前の座標に+60)
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID1,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("A"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDA,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("B"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width + 205,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDB,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("CLOSE"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		420,
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID2,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOW);

	if (hwnd == NULL) return 0;

	while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
	return msg.wParam;
}
