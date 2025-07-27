#pragma once
#include "Component.h"
#include "Structures.h"
using namespace ECC;
//Á‰ÂÒÚ¸ ‚Òˇ ÎÓ„ËÍ‡

//‰Îˇ Á‡„ÛÁÍË ÁÌ‡˜ÂÌËÈ
HBITMAP GetBitmap(string BitmapNameFile)
{
    const string fullName = BitmapNameFile + ".bmp";
    HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, fullName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return hBitmap;
}

void LoadTransform(CTransform& CTransform, float arr[])
{
    CTransform.x = arr[0];
    CTransform.y = arr[1];
    CTransform.Dx = arr[2];
    CTransform.Dy = arr[3];
    CTransform.Scale = arr[4];
    CTransform.Width = arr[5];
    CTransform.Height = arr[6];
}

float* GetTransformData()
{ ///‰Ó‡·ÓÚ‡Ú¸
    float arr[1];

    return arr;
}

void CreateCharacter(CTransform& CTransform)
{
    CTransform.x *= window.width;
    CTransform.y *= window.height;
    CTransform.Width *= window.width;
    CTransform.Height *= window.height;
}


//‰Û„‡ˇ ÎÓ„ËÍ‡
void Show(CBitmap& CBitmap, CTransform& Transform)
{
    float vx = (Transform.x - player_view.x) * Transform.Scale + window.width / 2;
    float vy = (Transform.y - player_view.y) * Transform.Scale + window.height / 2;
    float vw = Transform.Width * Transform.Scale;
    float vh = Transform.Height * Transform.Scale;

    bool in = false;

    if (vx + vw >= 0 && vx < window.width &&
        vy + vh >= 0 && vy < window.height)
        in = true;

    if (!in) return;
    ShowBitmap(window.context, vx, vy, vw, vh, CBitmap.HBitMap, false);
} // ÚÛÚ Ì‡‰Ó ÛÍ‡Á‡Ú¸ ËÏÂÌÌÓ Ë„ÓÍ‡

void ShowWindow(CBitmap& CBitmap)
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, CBitmap.HBitMap, false);
}

void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity)
{
    CJump.Jump *= .9;
    Transform.Dx *= .5;
    Transform.Dy = Gravity.Gravity - CJump.Jump;
}

void ProcessSound(CSound& CSound)
{
    PlaySound(TEXT(CSound.SoundNameFile), NULL, SND_FILENAME | SND_ASYNC);
}

void TracerCollide(CCollider& CCollider, CTransform& CTransform, CJump& CJump, int MapLVL)
{
    CCollider.LastTracePlatformNum = -1;
    CCollider.CollXfound = false;
    CCollider.CollYfound = false;

    float Lenght = sqrt(pow(CTransform.Dx, 2) + pow(CTransform.Dy, 2));
    for (float i = 0; i < Lenght; i++)
    {
        if (CCollider.CollXfound && CCollider.CollYfound) return;

        for (int k = 0; k < 4; k++)
        {
            for (int j = 0; j < location[MapLVL].walls.size(); j++)
            {
                float Bbox[] = {
                    CTransform.x + CTransform.Dx * i / Lenght, CTransform.y + CTransform.Dy * i / Lenght,
                    CTransform.x + CTransform.Width + CTransform.Dx * i / Lenght - 1, CTransform.y + CTransform.Dy * i / Lenght,
                    CTransform.x + CTransform.Width + CTransform.Dx * i / Lenght - 1, CTransform.y + CTransform.Height + CTransform.Dy * i / Lenght - 1,
                    CTransform.x + CTransform.Dx * i / Lenght, CTransform.y + CTransform.Height + CTransform.Dy * i / Lenght - 1
                };
                float pixel_x = Bbox[k * 2];
                float pixel_y = Bbox[k * 2 + 1];
               /* SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));*/

                auto walls = location[MapLVL].walls[j].Sprite;
                if ((pixel_x >= walls.x &&
                    pixel_x <= walls.x + walls.width) &&
                    (pixel_y >= walls.y &&
                        pixel_y <= walls.y + walls.height)
                    )
                {
                    float top = pixel_y - walls.y;
                    float down = (walls.y + walls.height) - pixel_y;
                    float left = pixel_x - walls.x;
                    float right = (walls.x + walls.width) - pixel_x;

                    float minX = min(left, right);
                    float minY = min(top, down);
                    CJump.InJump = false;

                    if (minX < minY && !CCollider.CollXfound)
                    {
                        CTransform.Dx = 0;
                        CCollider.CollXfound = true;

                        if (left < right)
                        {
                            CTransform.x = walls.x - CTransform.Width - 1;
                        }
                        else
                        {
                            CTransform.x = walls.x + walls.width + 1;
                        }

                        j++;
                    }

                    if (minX >= minY && !CCollider.CollYfound)
                    {
                        CTransform.Dy = 0;
                        CCollider.CollYfound = true;

                        if (down < top)
                        {
                            CTransform.y = walls.y + walls.height + 1;
                            CJump.Jump = 30;
                        }
                        else
                        {
                            CCollider.LastTracePlatformNum = j;
                            CTransform.y = walls.y - CTransform.Height - 1;
                            CJump.InJumpBot = false;
                        }

                        j++;
                    }

                }
            }
        }
    }
    if (!CCollider.CollXfound) CTransform.x += CTransform.Dx;
    if (!CCollider.CollYfound) CTransform.y += CTransform.Dy;
}

void MovePlayer(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
{
    if (GetAsyncKeyState(VK_LEFT)) 
    {
        CTransform.Dx = -CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        CTransform.Dx = CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_SPACE) && CJump.InJump == false && CJump.InJumpBot == false)
    {
        CJump.Jump = 110;
        CJump.InJumpBot = true;
        CJump.InJump = true;
    }

    TracerCollide(CCollider, CTransform, CJump, MapLVL);
    ProcessGravity(CJump, CTransform, Gravity);
}

void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
{
    TracerCollide(CCollider, CTransform, CJump, MapLVL);
    ProcessGravity(CJump, CTransform, Gravity);

    if (CCollider.LastTracePlatformNum >= 0)
    {
        if (CTransform.x <= location[MapLVL].walls[CCollider.LastTracePlatformNum].Sprite.x)
        {
            CCollider.Direction = 1;
        }

        auto& platform = location[MapLVL].walls[CCollider.LastTracePlatformNum].Sprite; //Á‰ÂÒÚ¸ ÌÛÊÌÓ ÔÓÎÛ˜Ú¸ ÔÓÚ‡ÙÓÏÛ ÒÚÂÌ ÌÛÊÌÓ ·Û‰ÚÂ ÔÓÚÓÏ ÔÓÏÂÌˇÚ¸
        if (CTransform.x + CTransform.Width >= platform.x + platform.width)
        {
            CCollider.Direction = -1;
        }
        CTransform.Dx = CCollider.Direction * CSpeed.SpeedWalk;
    }
}

void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, —Specialization& —Specialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CType—haracter& CType—haracter, CName—haracter& CName—haracter, —Rank& —Rank,
    string TypeDamage, string Status, string Type—haracter, string Gender, string NameChar, string Specialization, int Rank)
{



}


