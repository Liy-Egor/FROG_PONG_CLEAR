#pragma once
#include "Structures.h"



void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
    GetClientRect(window.hWnd, &r);

}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// �������� ����������� bitmap � �������� ������

    if (hOldbm) // ���� �� ���� ������, ���������� ������
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // ���������� ������� �����������

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//��� ������� ������� ����� ����� ��������������� ��� ����������
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteDC(hMemDC); // ������� �������� ������
}

void ShowRacketAndBall()
{
    location[player->currentLocation].hBack.showBack();
    player->racket.show();


    player->tdx = player->racket.x;
    player->tdy = player->racket.y;
    player->racket.dy = player->gravity - player->jump;
    player->racket.dx = player->tdx - player->racket.x;
    float ddx = player->racket.dx;
    float ddy = player->racket.dy;

    float lenght = sqrt(pow(ddx, 2) + pow(ddy, 2));

    for (float i = 0; i < lenght * 2; i++) {

        float s = i / lenght;
        float pixel_x = player->racket.x + ddx * s;
        float pixel_y = player->racket.y + ddy * s;

        for (int j = 0; j < location[player->currentLocation].walls.size(); j++)
        {
            auto walls = location[player->currentLocation].walls[j].Sprite;

            SetPixel(window.context, pixel_x, pixel_y, RGB(255, 255, 255));

            if ((pixel_x >= walls.x &&
                pixel_x <= walls.x + walls.width) &&
                (pixel_y >= walls.y  &&
                    pixel_y <= walls.y + walls.height))
                
            {

                float col = i;


                auto walls = location[player->currentLocation].walls[j].Sprite;

                float top = pixel_y - walls.y;
                float down = (walls.y + walls.height) - pixel_y;
                float left = pixel_x - walls.x;
                float right = (walls.x + walls.width) - pixel_x;

                float minX = min(left, right);
                float minY = min(top, down);
                player->inJump = false;

                if (minX < minY)
                {

                    if (left < right)
                    {
                        player->racket.x = walls.x -  player->racket.speed; // ???????? ????? ?????

                    }
                    else
                    {
                        player->racket.x = (walls.x + walls.width)  + player->racket.speed;  // ???????? ????? ??????

                    }
                    // dash_allow = false; // ?? ???? ?????? ????? (????? ????????? ?????????? ? ?????????)

                }
                else
                {

                    if (down < top)
                    {
                        player->racket.y = (walls.y  + walls.height) + player->racket.height + 100; // ?????? ????? ?? ???????
                        //player.jump *= .4;
                    }
                    else
                    {

                        player->racket.y = min(walls.y - player->racket.height - player->gravity, player->racket.y); // ???????? ????? ?? ?????????
                        player->inJump = false; // ???? ?????????? ?? ?????? (????? ????????? ?????????? ? ?????????)

                    }

                }

            }

        }

    }

}

void ShowTexture()
{

    for (int i = 0; i < location[player->currentLocation].locationTexture.size();i++) {
        location[player->currentLocation].locationTexture[i].Sprite.show();
    }
    for (int i = 0; i < location[player->currentLocation].walls.size();i++) {
        location[player->currentLocation].walls[i].Sprite.show();
    }
}

void ShowObjects()
{
    for (int i = 0; i < location[player->currentLocation].locationObjects.size();i++) {
        location[player->currentLocation].locationObjects[i].Sprite.show();
    }
}

void DrawHealth()
{
   
    for (int i = 0; i < health.max_lives; i++)
    {
        if (i < health.current_lives)
        {
            health.hHealthFull.showHealth(i, health.health_width);
        }
        else health.hHealthEmpty.showHealth(i, health.health_width);
    }
}