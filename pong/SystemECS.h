#pragma once
#include "ArcheType.h"
#include "PrimaryProcess.h"

using namespace ECC;
//çäåñòü âñÿ ëîãèêà


//äëÿ çàãðóçêè çíà÷åíèé
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
    CTransform.Width = arr[2];
    CTransform.Height = arr[3];
}

void CreateObject(CTransform& CTransform)
{
    CTransform.x *= window.width;
    CTransform.y *= window.height;
    CTransform.Width *= window.width;
    CTransform.Height *= window.height;
}

//äðóãàÿ ëîãèêà
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
}

void ShowWindow(CBitmap& CBitmap)
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, CBitmap.HBitMap, false);
}

void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity)
{
    CJump.Jump *= .9;
    Transform.Dx *= .1;
    Transform.Dy = Gravity.Gravity - CJump.Jump;
}

void ProcessSound(CSound& CSound)
{
    PlaySound(TEXT(CSound.SoundNameFile), NULL, SND_FILENAME | SND_ASYNC);
}

void TracerCollide(CTransform& TransformWalls,CCollider& CCollider, CTransform& CTransform, CJump& CJump, int MapLVL)
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
                int j = 0;
                float Bbox[] = {
                    CTransform.x + CTransform.Dx * i / Lenght, CTransform.y + CTransform.Dy * i / Lenght,
                    CTransform.x + CTransform.Width + CTransform.Dx * i / Lenght - 1, CTransform.y + CTransform.Dy * i / Lenght,
                    CTransform.x + CTransform.Width + CTransform.Dx * i / Lenght - 1, CTransform.y + CTransform.Height + CTransform.Dy * i / Lenght - 1,
                    CTransform.x + CTransform.Dx * i / Lenght, CTransform.y + CTransform.Height + CTransform.Dy * i / Lenght - 1
                };
                float pixel_x = Bbox[k * 2];
                float pixel_y = Bbox[k * 2 + 1];
                /*SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));*/

                auto walls = TransformWalls;
                if ((pixel_x >= walls.x &&
                    pixel_x <= walls.x + walls.Width) &&
                    (pixel_y >= walls.y &&
                        pixel_y <= walls.y + walls.Height)
                    )
                {
                    float top = pixel_y - walls.y;
                    float down = (walls.y + walls.Height) - pixel_y;
                    float left = pixel_x - walls.x;
                    float right = (walls.x + walls.Width) - pixel_x;

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
                            CTransform.x = walls.x + walls.Width + 1;
                        }

                        j++;
                    }

                    if (minX >= minY && !CCollider.CollYfound)
                    {
                        CTransform.Dy = 0;
                        CCollider.CollYfound = true;

                        if (down < top)
                        {
                            CTransform.y = walls.y + walls.Height + 1;
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
                else
                {
                    j++;
                }
        }
    }
    if (!CCollider.CollXfound) CTransform.x += CTransform.Dx;
    if (!CCollider.CollYfound) CTransform.y += CTransform.Dy;
}

void MovePlayer(CTransform& TransformWalls,CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
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
        CJump.Jump = 50;
        CJump.InJumpBot = true;
        CJump.InJump = true;
    }

    TracerCollide(TransformWalls,CCollider, CTransform, CJump, MapLVL);
    ProcessGravity(CJump, CTransform, Gravity);
}

void MoveCharacter(CTransform& TransformWalls,CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
{
    TracerCollide(TransformWalls,CCollider, CTransform, CJump, MapLVL);
    ProcessGravity(CJump, CTransform, Gravity);

    //if (CCollider.LastTracePlatformNum >= 0)
    //{
    //    if (CTransform.x <= location[MapLVL].walls[CCollider.LastTracePlatformNum].Sprite.x)
    //    {
    //        CCollider.Direction = 1;
    //    }

    //    auto& platform = location[MapLVL].walls[CCollider.LastTracePlatformNum].Sprite; //çäåñòü íóæíî ïîëó÷òü ïîòàôîðìó ñòåí íóæíî áóäòå ïîòîì ïîìåíÿòü
    //    if (CTransform.x + CTransform.Width >= platform.x + platform.width)
    //    {
    //        CCollider.Direction = -1;
    //    }
    //    CTransform.Dx = CCollider.Direction * CSpeed.SpeedWalk;
    //}
}

void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, ÑSpecialization& ÑSpecialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CTypeÑharacter& CTypeÑharacter, CNameÑharacter& CNameÑharacter, ÑRank& ÑRank,
    string TypeDamage, string Status, string TypeÑharacter, string Gender, string NameChar, string Specialization, int Rank)
{



}

void SwitchLotation()
{
   /* portal_(float p_x, float p_y, float p_width, float p_height, int p_destination, const string & filename)
        : StaticObjects(p_x, p_y, p_width, p_height, filename)
    {
        destination = p_destination;
    }*/

   /* if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        if (destination > player->currentLocation)
        {
            player->currentLocation = destination;
            player->Sprite.x = location[player->currentLocation].walls[0].Sprite.x + location[player->currentLocation].walls[0].Sprite.width;
        }
        else
        {
            player->currentLocation = destination;
            player->Sprite.x = location[player->currentLocation].walls[1].Sprite.x - player->Sprite.width - Sprite.width - 60;
        }
    }*/
}

void Heal()
{
   /* if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        location[player->currentLocation].healingFlask.erase(location[player->currentLocation].healingFlask.begin() + i);
        player->current_lives++;
    }*/
}

void Spikes()
{
   /* static int lastDamageTime = 0;
    bool spikeCollision = false;
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        spikeCollision = true;

    }
    if (spikeCollision && currenttime > lastDamageTime + 1000) {
        player->current_lives--;
        lastDamageTime = currenttime;
        player->Sprite.jump = 60;
        player->Sprite.x += 20;
        player->inJump = true;
    }*/
}
