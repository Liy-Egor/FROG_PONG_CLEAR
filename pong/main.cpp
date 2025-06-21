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

        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
        //PrintBitblt(); //копируем буфер в окно и отрисовывает окно за игроком
        
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        int sz = 5;
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);

        location[player->currentLocation].hBack.showBack();

        for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {

            location[player->currentLocation].Persona[i]->Sprite.show();
            location[player->currentLocation].Persona[i]->move();
        }
        player->Sprite.show();
        player->move();
        Health_bar.Show();


        if (GetAsyncKeyState('A'))
        {
            player_view.x++;
        }

        if (GetAsyncKeyState('D'))
        {
            player_view.x--;
        }

        player_view.x = player->Sprite.x - .5 * window.width/ 3;
        player_view.y = player->Sprite.y-.5*window.height/3;

        //player->Sprite.showHealth()
        //player.h
        
        ShowTexture();
        ShowObjects();
        //DrawHealth();
        ProcessPortal();
        CollisionGroup();
        //ProcessDash();//рывок
    }

}