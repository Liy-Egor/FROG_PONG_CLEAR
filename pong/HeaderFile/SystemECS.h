#pragma once
#include "GameFileSystem.h"

using namespace ECC;

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

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}

//void Show(CBitmap& CBitmap, CTransform& Transform)
//{
//    float vx = (Transform.x - player_view.x) * Transform.Scale + window.width / 2;
//    float vy = (Transform.y - player_view.y) * Transform.Scale + window.height / 2;
//    float vw = Transform.Width * Transform.Scale;
//    float vh = Transform.Height * Transform.Scale;
//
//    bool in = false;
//
//    if (vx + vw >= 0 && vx < window.width &&
//        vy + vh >= 0 && vy < window.height)
//        in = true;
//
//    if (!in) return;
//    ShowBitmap(window.context, vx, vy, vw, vh, CBitmap.HBitMap, false);
//}
//
//void ShowAnimation(CBitmap& CBitmap, CTransform& Transform,int num)
//{
//    float vx = (Transform.x - player_view.x) * Transform.Scale + window.width / 2;
//    float vy = (Transform.y - player_view.y) * Transform.Scale + window.height / 2;
//    float vw = Transform.Width * Transform.Scale;
//    float vh = Transform.Height * Transform.Scale;
//
//    bool in = false;
//    static int lastDamageTime = 0;
//    if (vx + vw >= 0 && vx < window.width &&
//        vy + vh >= 0 && vy < window.height)
//        in = true;
//
//    if (!in) return;
//        HBITMAP hbm, hOldbm;
//        HDC hMemDC;
//        BITMAP bm;
//        hMemDC = CreateCompatibleDC(window.context);
//        hOldbm = (HBITMAP)SelectObject(hMemDC, CBitmap.HBitMap);
//        GetObject(CBitmap.HBitMap, sizeof(BITMAP), (LPSTR)&bm);
//        if (hOldbm)
//        {
//            StretchBlt(window.context, vx, vy, vw/4, vh, hMemDC, num, 0, 137, bm.bmHeight, SRCCOPY);
//            SelectObject(hMemDC, hOldbm);
//        }
//        DeleteDC(hMemDC); // ??????? ???????? ??????
//}
//
//void ShowWindow(CBitmap& CBitmap)
//{
//    ShowBitmap(window.context, 0, 0, window.width, window.height, CBitmap.HBitMap, false);
//}

void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump)
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

void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity)
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

void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity)
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
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, СSpecialization& СSpecialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CTypeСharacter& CTypeСharacter, CNameСharacter& CNameСharacter, СRank& СRank,
    string TypeDamage, string Status, string Typeаharacter, string Gender, string NameChar, string Specialization, int Rank)
{



}

void SwitchLotation(CPortalPаth& PortalPаth, CTransform& Transform)
{
    if (CheckCollision(Player->GetPosition()->x, Player->GetPosition()->y, Player->GetPosition()->Width, Player->GetPosition()->Height, Transform.x, Transform.y, Transform.Width, Transform.Height))
    {
        if (VLocation.size() >= PortalPаth.Pаth)
        {
            Player->SetLocation(PortalPаth.Pаth);
            Player->GetPosition()->x = 2473;
            Player->GetPosition()->y = 3639;
            MapSizeW = VLocation[PortalPаth.Pаth].GetPosition()->Width;
            MapSizeH = VLocation[PortalPаth.Pаth].GetPosition()->Height;
        }
    }
}

bool HealEvent(CTransform& Transform, CHealth& CHealth)
{
    CHealth.Health = 30;
    if (CheckCollision(Player->GetPosition()->x, Player->GetPosition()->y, Player->GetPosition()->Width, Player->GetPosition()->Height, Transform.x, Transform.y, Transform.Width, Transform.Height))
    {
        if (Player->GetHealth()->Health < Player->GetHealth()->MaxHealth)
        {
            if (Player->GetHealth()->Health + CHealth.Health > Player->GetHealth()->MaxHealth)
            {
                Player->GetHealth()->Health += Player->GetHealth()->MaxHealth - (Player->GetHealth()->Health + CHealth.Health);
                return true;
            }
            else if (Player->GetHealth()->Health == Player->GetHealth()->MaxHealth)
            {
                return false;
            }
            else
            {
                Player->GetHealth()->Health += CHealth.Health;
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void SpikeEvent(CTransform& Transform,CDamage& Damage)
{   
    static int lastDamageTime = 0;
    bool spikeCollision = false;
    if (CheckCollision(Player->GetPosition()->x, Player->GetPosition()->y, Player->GetPosition()->Width, Player->GetPosition()->Height, Transform.x, Transform.y, Transform.Width, Transform.Height))
    {
        spikeCollision = true;
    }
    if (spikeCollision && currenttime > lastDamageTime + 1000) {
        Damage.Damage = 30;
        if (Player->GetHealth()->Health > Damage.Damage)
        {
        Player->GetHealth()->Health -= Damage.Damage;
        lastDamageTime = currenttime;
        Player->GetJump()->Jump = 60;
        Player->GetPosition()->x += 20;
        Player->GetJump()->InJump = true;
        }
        else
        {
           Player->Destroy();
           exit(0); // ааааааа аа ааааааааа аааааа ааа аааа ааа аааааа аааааа аааааааа
        }
    }
}

void HealthBar()
{
    SetTextColor(window.context, RGB(50, 205, 50));
    //SetBkMode(window.context, TRANSPARENT); //аааааааааааа
    auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "CALIBRI"); //ааааа
    auto hTmp = (HFONT)SelectObject(window.context, hFont);

    char txt[32];
    _itoa_s(Player->GetHealth()->Health, txt, 10); // аа аааа ааааааааа аааааа
    TextOutA(window.context, window.width - 400, window.height - 1000, (LPCSTR)txt, strlen(txt));// аааа ааааа аааааа
}

void AppGame::Init()
{	
	LoadSVGDataMap(MAPS"LVLDemoDay0");
	LoadSVGDataMap(MAPS"LVLDemoDay1");

	MapSizeW = VLocation[0].GetPosition()->Width;
	MapSizeH = VLocation[0].GetPosition()->Height;
}

void AppGame::Render()
{
	float sin_ = sin(Timer.TimePeak()) / 1.0f + 0.5f;
	d3dx.RenderClearBuffer(sin_ /2, 0.2f, 1.0f);

	for (int i = 0; i < VLocation.size(); i++)
	{
		if (Player->GetLocation() == i)
		{

			d3dx.DrawObject(
				VLocation[Player->GetLocation()].GetPosition()->x,
				VLocation[Player->GetLocation()].GetPosition()->y,
				1,
				VLocation[Player->GetLocation()].GetPosition()->Width,
				VLocation[Player->GetLocation()].GetPosition()->Height,
				0,
				VLocation[Player->GetLocation()].GetRender()->TypeRender,
				VLocation[Player->GetLocation()].GetTexture()->Texture
			);

			for (ATWall var : VLocation[i].VWall)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture
				);
			}
			for (ATEnemy var : VLocation[i].VEnemy)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture
				);
				var.Start();
			}
			for (ATHealFlack var : VLocation[i].VHealFlack)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					TypeObject::BOX2DTEX,
					var.GetTexture()->Texture
				);
				if (var.GoEvent())
				{
					var.Destroy();
				}
			}
			for (ATPortal var : VLocation[i].VPortal)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture
				);
				var.GoEvent();
			}
			for (ATSpike var : VLocation[i].VSpike)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture
				);
				var.GoEvent();
			}

			d3dx.DrawObject(
				Player->GetPosition()->x, Player->GetPosition()->y, 1,
				Player->GetPosition()->Width, Player->GetPosition()->Height,
				0,
				Player->GetRender()->TypeRender,
				Player->GetTexture()->Texture
			);

			Player->Start();
			HealthBar();
			break;
		}
	}
	d3dx.SetCameraTarget(Player->GetPosition()->x, Player->GetPosition()->y);

	d3dx.Present(true);
}

void AppGame::UpdateApp(MSG* msg)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		msg->message = WM_QUIT;
	}
}

