#pragma once
#include "Structures.h"





//void CreateMap()
//{
//    RECT z;
//    GetClientRect(window.hWnd, &z);
//    window.device_context = GetDC(window.hWnd);
//    z.left = 0;
//    z.right = window.width * 3;
//    z.bottom = window.height * 2;
//    z.top = 0;
//
//    window.width_z = z.right - z.left;
//    window.height_z = z.bottom - z.top;
//
//    window.context = CreateCompatibleDC(window.device_context);
//    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width_z, window.height_z));
//    GetClientRect(window.hWnd, &z);
//
//}

void ShowBitmap(int x, int y, int width, int height, int textureID)
{
    // Преобразование в NDC (Normalized Device Coordinates)
    float screenWidth = static_cast<float>(window.width);
    float screenHeight = static_cast<float>(window.height);

    float left = (x * 2.0f / screenWidth) - 1.0f;
    float right = ((x + width) * 2.0f / screenWidth) - 1.0f;
    float top = 1.0f - (y * 2.0f / screenHeight);
    float bottom = 1.0f - ((y + height) * 2.0f / screenHeight);

    ConstBuf::global[0] = XMFLOAT4(left, top, right, bottom);
    ConstBuf::Update(5, ConstBuf::global);
    ConstBuf::ConstToVertex(5);
    context->PSSetShaderResources(0, 1, &Textures::Texture[textureID].TextureResView);

    ConstBuf::ConstToVertex(4);
    ConstBuf::ConstToPixel(4);
    Camera::Camera();
    Draw::NullDrawer(1);
    

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
