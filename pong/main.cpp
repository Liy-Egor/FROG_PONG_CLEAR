#include "Structures.h"
#include "paint.h"
#include "logick.h"


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

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
  
    InitGame();//здесь инициализируем переменные игры
    ShowCursor(FALSE);
    


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();

        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
        //PrintBitblt(); //копируем буфер в окно и отрисовывает окно за игроком
        
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        int sz = 5;
        

        location[player->currentLocation].hBack.showBack();

        for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {

            location[player->currentLocation].Persona[i]->Sprite.show();
            location[player->currentLocation].Persona[i]->move();
        }
        player->Sprite.show();
        player->move();
        Health_bar.Show();
        ShowStaticObjects();
        for (int i = 0; i < location[player->currentLocation].portal.size(); i++) {

            location[player->currentLocation].portal[i].Sprite.show();
            location[player->currentLocation].portal[i].ProcessPortal(player);
        }
        
        //CollisionGroup();
        //ProcessDash();//рывок
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);

        if (GetAsyncKeyState('A'))
        {
            player_view.x++;
        }

        if (GetAsyncKeyState('D'))
        {
            player_view.x--;
        }

        float ls = .2* length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y)/500.;
        ls = max(ls - .2, 0.1);
        ls = min(ls, 1);
        player_view.x = lerp(player_view.x, player->Sprite.x,ls);
        player_view.y = lerp(player_view.y, player->Sprite.y,ls);
    }

}