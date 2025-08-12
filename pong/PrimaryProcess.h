#pragma once
#include "ECSSoft.h"

void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);
    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);
    window.width = r.right - r.left;
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));
    GetClientRect(window.hWnd, &r);
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC);
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);

    if (hOldbm)
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm);

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//??? ??????? ??????? ????? ????? ??????????????? ??? ??????????
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ?????? ??????????? bitmap
        }

        SelectObject(hMemDC, hOldbm);// ??????????????? ???????? ??????
    }

    DeleteDC(hMemDC); // ??????? ???????? ??????
}


//из этого нужно сделать класс приложения
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
};
class Window
{
private:
	const char* NameClass = "Window";
	RECT rc;
	HINSTANCE hIns;
	HWND hWnd;
public:
	Window(int Width,int Height, const char* NameWind) 
		{
		rc = {0,0,Width,Height 
		};

		AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpszClassName = NameClass;
		wc.hInstance = hIns;
		wc.lpfnWndProc = &WindowProc;

		auto NameClassId = RegisterClassEx(&wc);

		hWnd = CreateWindowEx(
			NULL,
			MAKEINTATOM(NameClassId),
			NameWind, 
			WS_CAPTION| WS_MINIMIZEBOX | WS_SYSMENU, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT,
			rc.right-rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			hIns,
			NULL
			);

		ShowWindow(hWnd, SW_SHOW);
	};
	~Window() 
	{
		DestroyWindow(hWnd);
	};
};



//система проверки ошибок логер на основе терминала
class Logger
{
public:
	Logger() {};
	~Logger() {};
};
