#pragma once
#include "Structures.h"



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

//void PrintBitblt()
//{
//
//    //BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//???????? ????? ? ????
//
//    if (player->Sprite.x <= window.width / 2)
//    {
//        if (player->Sprite.y <= window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, 0, 0, SRCCOPY); // ???? ???? ?????? ???? ??????
//        }
//        else if (player->Sprite.y <= window.height_z - window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, 0, player->Sprite.y - window.height / 2, SRCCOPY);
//        }
//        else
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, 0, window.height_z - window.height, SRCCOPY);
//
//        }
//    }
//
//    else if (player->Sprite.x >= window.width / 2)
//    {
//
//        if (player->Sprite.y <= window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, player->Sprite.x - window.width / 2, 0, SRCCOPY); // ???? ???? ?????? ???? ??????
//        }
//        else if (player->Sprite.y <= window.height_z - window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, player->Sprite.x - window.width / 2, player->Sprite.y - window.height / 2, SRCCOPY);
//        }
//        else
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, player->Sprite.x - window.width / 2, window.height_z - window.height, SRCCOPY);
//
//        }
//
//    }
//    else
//    {
//
//        if (player->Sprite.y <= window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, window.width_z, 0, SRCCOPY); // ???? ???? ?????? ???? ??????
//        }
//        else if (player->Sprite.y <= window.height_z - window.height / 2)
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, window.width_z, player->Sprite.y - window.height / 2, SRCCOPY);
//        }
//        else
//        {
//            BitBlt(window.device_context, 0, 0, window.width_z, window.height_z, window.context, window.width_z, window.height_z - window.height, SRCCOPY);
//
//        }
//
//    }
//}




void ShowStaticObjects()
{
    for (int i = 0; i < location[player->currentLocation].walls.size();i++) {
        location[player->currentLocation].walls[i].Sprite.show();
    }
    for (int i = 0; i < location[player->currentLocation].locationObjects.size();i++) {
        location[player->currentLocation].locationObjects[i].Sprite.show();
    }
}


