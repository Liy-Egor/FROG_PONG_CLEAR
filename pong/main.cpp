#include "Structures.h"
#include "paint.h"
#include "logick.h"
#include "GameFileSystem.h"

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

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����

    InitGame();//����� �������������� ���������� ����
    ShowCursor(FALSE);
    


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        Serialization DataGame(location[0]);
        if (GetAsyncKeyState(VK_F1)) {
            DataGame.Ser();
        }
        if (GetAsyncKeyState(VK_F2)) {
            DataGame.Deser();
        }

        currenttime = timeGetTime();

        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
        //PrintBitblt(); //�������� ����� � ���� � ������������ ���� �� �������

        Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
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
        for (int i = 0; i < location[player->currentLocation].walls.size(); i++) {
            location[player->currentLocation].walls[i].Sprite.show();
        }
        for (int i = 0; i < location[player->currentLocation].healingFlask.size(); i++) {
            location[player->currentLocation].healingFlask[i].Sprite.show();
            location[player->currentLocation].healingFlask[i].healing(player, i);
        }
        for (int i = 0; i < location[player->currentLocation].spike.size(); i++) {
            location[player->currentLocation].spike[i].Sprite.show();
            location[player->currentLocation].spike[i].damage(player);
        }
        for (int i = 0; i < location[player->currentLocation].portal.size(); i++) {
            location[player->currentLocation].portal[i].Sprite.show();
            location[player->currentLocation].portal[i].Portal(player);
        }


        //ProcessDash();//�����
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);


        float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
        ls = max(ls - .2, 0.1);
        ls = min(ls, 1);

        float cameraHalfWidth = (window.width / 2) / scale;
        float cameraHalfHeight = (window.height / 2) / scale;

        float targetX = player->Sprite.x;
        float targetY = player->Sprite.y;

        targetX = max(0 + cameraHalfWidth,
            min(MapSizeW - cameraHalfWidth, targetX));
        targetY = max(0 + cameraHalfHeight,
            min(MapSizeH - cameraHalfHeight, targetY));

        player_view.x = lerp(player_view.x, targetX, 0.1f);
        player_view.y = lerp(player_view.y, targetY, 0.1f);
    }


}