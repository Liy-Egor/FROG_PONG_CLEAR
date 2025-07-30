#pragma once
#include "GameFileSystem.h"

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

void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump, int MapLVL)
{
    CCollider.LastTracePlatformNum = -1;
    bool CollXfound = false;
    bool CollYfound = false;

    float Lenght = sqrt(pow(Transform.Dx, 2) + pow(Transform.Dy, 2));
    for (float i = 0; i < Lenght; i++)
    {
        if (CollXfound && CollYfound) return;

        for (int k = 0; k < 4; k++)
        {
            
            for (int j = 0; j < VLocation[Player->GetLocation()].VWall.size(); j++)
            {
                
            
            float Bbox[] = {
                Transform.x + Transform.Dx * i / Lenght, Transform.y + Transform.Dy * i / Lenght,
                Transform.x + Transform.Width + Transform.Dx * i / Lenght - 1, Transform.y + Transform.Dy * i / Lenght,
                Transform.x + Transform.Width + Transform.Dx * i / Lenght - 1, Transform.y + Transform.Height + Transform.Dy * i / Lenght - 1,
                Transform.x + Transform.Dx * i / Lenght, Transform.y + Transform.Height + Transform.Dy * i / Lenght - 1
            };
            float pixel_x = Bbox[k * 2];
            float pixel_y = Bbox[k * 2 + 1];
            SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));

            auto walls = VLocation[Player->GetLocation()].VWall[j].GetPosition();
            if ((pixel_x >= walls->x &&
                pixel_x <= walls->x + walls->Width) &&
                (pixel_y >= walls->y &&
                    pixel_y <= walls->y + walls->Height)
                )
            {
                float top = pixel_y - walls->y;
                float down = (walls->y + walls->Height) - pixel_y;
                float left = pixel_x - walls->x;
                float right = (walls->x + walls->Width) - pixel_x;

                float minX = min(left, right);
                float minY = min(top, down);
                CJump.InJump = false;

                if (minX < minY && !CollXfound)
                {
                    Transform.Dx = 0;
                    CollXfound = true;

                    if (left < right)
                    {
                        Transform.x = walls->x - Transform.Width - 1;
                    }
                    else
                    {
                        Transform.x = walls->x + walls->Width + 1;
                    }

                    j++;
                }

                if (minX >= minY && !CollYfound)
                {
                    Transform.Dy = 0;
                    CollYfound = true;

                    if (down < top)
                    {
                        Transform.y = walls->y + walls->Height + 1;
                        CJump.Jump = 30;
                    }
                    else
                    {
                        CCollider.LastTracePlatformNum = j;
                        Transform.y = walls->y - Transform.Height - 1;
                        CJump.InJumpBot = false;
                    }
                    j++;
                }
            }

           }
        }
    }
    if (!CollXfound) Transform.x += Transform.Dx;
    if (!CollYfound) Transform.y += Transform.Dy;
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

void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
{
    if (GetAsyncKeyState(VK_LEFT)) 
    {
        Transform.Dx = -CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        Transform.Dx = CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_SPACE) && CJump.InJump == false && CJump.InJumpBot == false)
    {
        CJump.Jump = 110;
        CJump.InJumpBot = true;
        CJump.InJump = true;
    }

    float cameraHalfWidth = (window.width / 2) / Transform.Scale;
    float cameraHalfHeight = (window.height / 2) / Transform.Scale;

    float targetX = Transform.x;
    float targetY = Transform.y;

    targetX = max(0 + cameraHalfWidth,
        min(MapSizeW - cameraHalfWidth, targetX));
    targetY = max(0 + cameraHalfHeight,
        min(MapSizeH - cameraHalfHeight, targetY));

    player_view.x = lerp(player_view.x, targetX, 0.1f);
    player_view.y = lerp(player_view.y, targetY, 0.1f);
}

void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL)
{
        CSpeed.SpeedWalk = 14;
        for (int i = 0; i < VLocation.size(); i++)
        {
            if (Player->GetLocation() == i)
            {
                for (int j = 0; j < VLocation[i].VWall.size();j++)
                { 
                    if (CCollider.LastTracePlatformNum >= 0)
                    {
                        auto& platform = *VLocation[i].VWall[CCollider.LastTracePlatformNum].GetPosition();
                        if (CTransform.x <= platform.x)
                        {
                            CCollider.Direction = 1;
                        }
                        if (CTransform.x + CTransform.Width >= platform.x + platform.Width)
                        {
                            CCollider.Direction = -1;
                        }
                    }
                }
            }
                        CTransform.Dx = CCollider.Direction * CSpeed.SpeedWalk;
        }
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

void UpdateView()
{
    for (int i = 0; i < VLocation.size(); i++)
    {
        if (Player->GetLocation() == i)
        {
            ShowWindow(*VLocation[i].GetBitmaps());
            for (ATWall var : VLocation[i].VWall)
            {
                Show(*var.GetBitmaps(), *var.GetPosition());
            }
            for (ATEnemyFrog var : VLocation[i].VEnemyFrog)
            {
                Show(*var.GetBitmaps(), *var.GetPosition());
            }
            for (ATHealFlack var : VLocation[i].VHealFlack)
            {
                Show(*var.GetBitmaps(), *var.GetPosition());
            }
            for (ATPortal var : VLocation[i].VPortal)
            {
                Show(*var.GetBitmaps(), *var.GetPosition());
            }
            for (ATSpike var : VLocation[i].VSpike)
            {
                Show(*var.GetBitmaps(), *var.GetPosition());
            }
            Show(*Player->GetBitmaps(), *Player->GetPosition());
            break;
        }
    }
}

void UpdatePhysics()
{
    Player->Start();
    for (int i = 0; i < VLocation.size(); i++)
    {
        if (Player->GetLocation() == i)
        {
            for (ATEnemyFrog var : VLocation[i].VEnemyFrog)
            {
                var.Start();
            }
        }
    }
}