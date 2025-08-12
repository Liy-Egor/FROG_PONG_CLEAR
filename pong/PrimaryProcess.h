#pragma once
#include "ECSSoft.h"
#include "D3D11APP.h"


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

class Window
{
private:
	const char* ClassName = "Window";
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg,WPARAM wParam,LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			DestroyWindow(hWnd);
		}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
public:
	Window() {
		RECT rect{ 0,0,600,600 };

		WNDCLASSEX wc = { 0 }; // Структура содержит сведения о окне
		wc.cbSize = sizeof(wc);// Размер этой структуры в байтах
		wc.style = CS_OWNDC;// Этот элемент может быть любым сочетанием стилей классов
		wc.lpfnWndProc = WindowProc;// Указатель на процедуру окна
		wc.cbClsExtra = 0;// Количество дополнительных байтов, выделяемых после структуры класса окна
		wc.cbWndExtra = 0;// Количество дополнительных байтов, выделяемых после экземпляра окна
		wc.hInstance = hIns;// Дескриптор экземпляра, содержащего процедуру окна для класса
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;// Дескриптор курсора класса. Этот элемент должен быть дескриптором ресурса курсора
		wc.hbrBackground = nullptr;// Дескриптор фоновой кисти класса. Этот элемент может быть дескриптором кисти, используемой для рисования фона
		wc.lpszMenuName = nullptr;// Указатель на строку символов, указывает имя ресурса меню класса, так как имя отображается в файле ресурса
		wc.lpszClassName = ClassName;// Является строкой, он указывает имя класса окна. Имя класса может быть любым именем, зарегистрированным в RegisterClass
		wc.hIconSm = nullptr;
		RegisterClassEx(&wc);// Регистрирует класс окна для последующего использования в вызовах функции CreateWindow получется как значение ATOM

		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
		hWnd = CreateWindow(
			ClassName,
			"FrogGame",
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			hIns,
			this
		);
	};
	~Window() {};

	HWND GethWnd() {return hWnd;}
	

private:
	HINSTANCE hIns;
	HWND hWnd;

};