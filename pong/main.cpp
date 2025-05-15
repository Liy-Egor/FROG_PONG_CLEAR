#include "Structures.h"
#include "paint.h"
#include "logick.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне

    InitGame();//здесь инициализируем переменные игры
    //ShowCursor(FALSE);


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();

        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        int sz = 5;
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);

        ProcessInput();//опрос клавиатуры
        ShowRacketAndBall();//рисуем фон, ракетку и шарик
        ShowTexture();
        ShowObjects();
        DrawHealth();
        ProcessPortal();
        CollisionGroup();
        ProcessDash();//рывок
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        //CollisionGroup();
    }

}