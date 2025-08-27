#pragma once
#include "D3D11APP.h"

class AppGame
{
public:
	AppGame() {};

	void FrameGo()
	{

		MSG msg;
		BOOL gbool = true;
		Init();
		while (gbool)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				UpdateApp(&msg);
				if (msg.message == WM_QUIT)
				{
					gbool = false;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Render();
		}
	}
private:
	void UpdateApp(MSG* msg);
	void Render();
	void Init();
}App;
