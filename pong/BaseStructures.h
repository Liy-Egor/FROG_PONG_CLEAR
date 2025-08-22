#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>
#include "vector"
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "DirectXTex.h"



using namespace std;
class Logger
{
public:
	Logger() {};
	~Logger() {};
	void Log(HRESULT hr,const char* msg)
	{
		if (FAILED(hr))
		{
			std::clog << endl << "ERROR - " << msg << endl;
			throw std::runtime_error("0 ");
		}
	}

	void Info(float data, const char* msg)
	{
		std::clog << endl << "INFO - "<< data << "; MSG - " << msg << endl;
	}

}Logg;

int currenttime = 0;
POINT mouse;

struct 
{
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);//сюда сохраним размеры окна которое создаст программа
} window;

struct 
{
    int x = 0;
    int y = 0;
    int w;
    int h;
} player_view;

int MapSizeW = window.width;
int MapSizeH = window.height;

bool PlayerPlay = false;

class ChronoTimer
{
public:
	ChronoTimer() {
		Begin = chrono::steady_clock::now();
	}

	float TimePeak()
	{
		return chrono::duration<float>(chrono::steady_clock::now() - Begin).count();
	}

private:
	chrono::steady_clock::time_point Begin;
}Timer;

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
	Window(int Width, int Height, const char* NameWind)
	{
		rc = { 0,0,Width,Height
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
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
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

	HWND GetHWND()
	{
		return hWnd;
	}
}win(window.width, window.height, "GameFrog");

