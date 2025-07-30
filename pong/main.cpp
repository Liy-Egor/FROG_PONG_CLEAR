#include "SystemECS.h"


float lerp(float x1, float x2, float a)
{
    return x1 * (1 - a) + x2 * a;
}

float length(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    InitWindow();//отрисовка окна windows
    InitGame(); //загрузка карт
    ShowCursor(FALSE);
   
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        
        UpdateGame(); //обработка физики,отрисовка картинок, проверка ивентов


        Ellipse(window.context, mouse.x - 5, mouse.y - 5, mouse.x + 5, mouse.y + 5);
    }
}
 
        //что это???!?!?
        //float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
        //ls = max(ls - .2, 0.1);
        //ls = min(ls, 1);