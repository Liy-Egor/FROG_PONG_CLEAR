#include "Structures.h"
#include "paint.h"
#include "logick.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    CreateMap();
    InitGame();//здесь инициализируем переменные игры
    //ShowCursor(FALSE);
    


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();

        PrintBitblt(); //копируем буфер в окно и отрисовывает окно за игроком

        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        int sz = 5;
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);

        location[player->currentLocation].hBack.showBack();

        for (int i = 0; i < Persona.size(); i++) {

            Persona[i]->Sprite.show();
            Persona[i]->move();
        }
        
        ShowTexture();
        ShowObjects();
        //DrawHealth();
        //ProcessPortal();
        CollisionGroup();
        //ProcessDash();//рывок
    }

}