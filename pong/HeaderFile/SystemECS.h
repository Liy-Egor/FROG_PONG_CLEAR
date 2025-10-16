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

void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump)
{
    CCollider.LastTracePlatformNum = -1;
	CCollider.CollXfound = false;
	CCollider.CollYfound = false;

    float Lenght = sqrt(pow(Transform.Dx, 2) + pow(Transform.Dy, 2));
    for (float i = 0; i < Lenght; i++)
    {
        if (CCollider.CollXfound && CCollider.CollYfound) return;

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

                if (minX < minY && !CCollider.CollXfound)
                {
                    Transform.Dx = 0;
                    CCollider.CollXfound = true;

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

                if (minX >= minY && !CCollider.CollYfound)
                {
                    Transform.Dy = 0;
                    CCollider.CollYfound = true;

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
    if (!CCollider.CollXfound) Transform.x += Transform.Dx;
    if (!CCollider.CollYfound) Transform.y += Transform.Dy;
}

void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity)
{
    CJump.Jump *= 0.99;
    Transform.Dx *= .1;
    Transform.Dy = Gravity.Gravity - CJump.Jump;
}

void ProcessSound(CSound& CSound)
{
    PlaySound(TEXT(CSound.SoundNameFile), NULL, SND_FILENAME | SND_ASYNC);
}

void SingleTapInput(int KeyInput, int& ImputTimer)
{
	ImputTimer--;
	if (GetAsyncKeyState(KeyInput))
	{
		ImputTimer = 1;
	}
}



void AttackPlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation, CAnimationTimeLine& TimeLine, CImputTimer& ImputTimer)
{
	if (GetAsyncKeyState(VK_LBUTTON)&& ImputTimer.KeyLButton <= 0)
	{
		StatusAnimation.StatusAnim = StatusAnimate::SWORD;

		if (ImputTimer.OrderBehavior <= 0)
		{
			ImputTimer.OrderBehavior = 100;
			StatusAnimation.PatternAnim = "~playersword360";




		}
		else if (ImputTimer.OrderBehavior > 0 && ImputTimer.OrderBehavior <= 100)
		{
			ImputTimer.OrderBehavior = 300;
			StatusAnimation.PatternAnim = "~playerswordhorizont";




		}
		else if (ImputTimer.OrderBehavior > 100 && ImputTimer.OrderBehavior <= 300)
		{
			ImputTimer.OrderBehavior = 0;
			StatusAnimation.PatternAnim = "~playerswordmega";





		}
	}

	SingleTapInput(VK_LBUTTON, ImputTimer.KeyLButton); /// команда которая не дает зажимать кнопку
}

void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation, CAnimationTimeLine& TimeLine, CImputTimer& ImputTimer)
{		
		/// остановка процесса движения если мы бъем
		if (StatusAnimation.PatternAnim == "~playersword360" || StatusAnimation.PatternAnim == "~playerswordhorizont" || StatusAnimation.PatternAnim == "~playerswordmega")
		{
			if (GetAsyncKeyState(0x41) && !CCollider.CollYfound) /// A
			{
				StatusAnimation.Mirror = -1;
				Transform.Dx = -CSpeed.SpeedWalk;
			}
			else if (GetAsyncKeyState(0x44) && !CCollider.CollYfound) /// D
			{
				StatusAnimation.Mirror = 1;
				Transform.Dx = CSpeed.SpeedWalk;
			}
			
			if (TimeLine.TimeLineIt[0] == 0)
			{
				StatusAnimation.StatusAnim = StatusAnimate::IDLE;
				StatusAnimation.PatternAnim = "no pattern";
			}
		}
		else
		{
		ImputTimer.OrderBehavior--;
		if (GetAsyncKeyState(VK_SPACE) && CCollider.CollYfound && ImputTimer.KeySpace <= 0)
		{
			CJump.InJumpBot = true;
			CJump.InJump = true;
			CJump.Jump = 60;
		}

		SingleTapInput(VK_SPACE, ImputTimer.KeySpace); /// команда которая не дает зажимать кнопку

			/// бежим или идем
		if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x44))
		{
					string temp;
				if (GetAsyncKeyState(0x41)) /// A
					{
						StatusAnimation.Mirror = -1;
						Transform.Dx = -CSpeed.SpeedWalk;
						temp = "~playerwalkL";
					}
				else if (GetAsyncKeyState(0x44)) /// D
					{
						StatusAnimation.Mirror = 1;
						Transform.Dx = CSpeed.SpeedWalk;
						temp = "~playerwalkR";
					}
				if (CCollider.CollYfound)
				{
					if (GetAsyncKeyState(VK_LSHIFT))
					{	/// бег
						CSpeed.SpeedWalk = 12;
						StatusAnimation.StatusAnim = StatusAnimate::RUN;
						StatusAnimation.PatternAnim = "~playerrun";
					}
					else
					{	/// ходьба
						CSpeed.SpeedWalk = 6;
						StatusAnimation.StatusAnim = StatusAnimate::WALK;
						if (StatusAnimation.PatternAnim == "~playerrun" || StatusAnimation.PatternAnim == "~playerwalkwalk")
						{
							StatusAnimation.PatternAnim = "~playerwalkwalk";
						}
						else
						{
							StatusAnimation.PatternAnim = temp;
						}
					}
				}
				else
				{
					StatusAnimation.StatusAnim = StatusAnimate::JUMP;
					StatusAnimation.PatternAnim = "~playerjumpRun";
				}
		}
		else
		{
			/// стоим
			if (CCollider.CollYfound)
			{
				StatusAnimation.StatusAnim = StatusAnimate::IDLE;
				/// стоим
				if (StatusAnimation.PatternAnim == "~playerwalkwalk" ||
					StatusAnimation.PatternAnim == "~playerwalkL" ||
					StatusAnimation.PatternAnim == "~playerwalkR" ||
					StatusAnimation.PatternAnim == "~playerwalkidle" ||
					StatusAnimation.PatternAnim == "~playerjumpidle" ||
					StatusAnimation.PatternAnim == "~playerjumpRun" ||
					StatusAnimation.PatternAnim == "~playerrun"
					)
				{
					StatusAnimation.PatternAnim = "~playerwalkidle";
				}
				else
				{
					StatusAnimation.PatternAnim = "no pattern";
				}
			}
			else
			{
				StatusAnimation.StatusAnim = StatusAnimate::JUMP;
				StatusAnimation.PatternAnim = "~playerjumpidle";
			}
		}
		}
}
void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation)
{
        CSpeed.SpeedWalk = 6;

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
							StatusAnimation.Mirror = 1;
							StatusAnimation.StatusAnim = StatusAnimate::WALK;
							StatusAnimation.PatternAnim = "~enemywalkR";
                            CCollider.Direction = 1;
                        }
                        if (CTransform.x + CTransform.Width >= platform.x + platform.Width)
                        {
							StatusAnimation.Mirror = -1;
							StatusAnimation.StatusAnim = StatusAnimate::WALK;
							StatusAnimation.PatternAnim = "~enemywalkL";
                            CCollider.Direction = -1;
                        }
                    }
                }
            }
                        CTransform.Dx = CCollider.Direction * CSpeed.SpeedWalk;
        }
}

void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, CSpecialization& CSpecialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CTypeCharacter& CTypeCharacter, CNameCharacter& CNameCharacter, CRank& CRank,
    string TypeDamage, string Status, string TypeCharacter, string Gender, string NameChar, string Specialization, int Rank)
{



}

void SwitchLotation(CPortalPath& PortalPath, CTransform& Transform)
{
    if (CheckCollision(Player->GetPosition()->x, Player->GetPosition()->y, Player->GetPosition()->Width, Player->GetPosition()->Height, Transform.x, Transform.y, Transform.Width, Transform.Height))
    {
        if (VLocation.size() >= PortalPath.Path)
        {
            Player->SetLocation(PortalPath.Path);
            Player->GetPosition()->x = 2473;
            Player->GetPosition()->y = 3639;
            MapSizeW = VLocation[PortalPath.Path].GetPosition()->Width;
            MapSizeH = VLocation[PortalPath.Path].GetPosition()->Height;
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

	LoadAnimationFiles(PLAYER"Animation");
	LoadAnimationFiles(ENEMY"Animation");

	LoadPatternAnmation();

	MapSizeW = VLocation[0].GetPosition()->Width;
	MapSizeH = VLocation[0].GetPosition()->Height;
}

void AppGame::Render()
{
	float sin_ = sin(Timer.TimePeak()) / 1.0f + 0.5f;
	d3dx.RenderClearBuffer(0.2f, 0.2f, 1.0f);

	for (int i = 0; i < VLocation.size(); i++)
	{
		if (Player->GetLocation() == i)
		{
			int size = 300;

			d3dx.DrawObject(
				VLocation[Player->GetLocation()].GetPosition()->x,
				VLocation[Player->GetLocation()].GetPosition()->y,
				1,
				VLocation[Player->GetLocation()].GetPosition()->Width,
				VLocation[Player->GetLocation()].GetPosition()->Height,
				0,
				VLocation[Player->GetLocation()].GetRender()->TypeRender,
				VLocation[Player->GetLocation()].GetTexture()->Texture,
				VLocation[Player->GetLocation()].GetNameObj()->Name,
				VLocation[Player->GetLocation()].GetStatusAnimation()->StatusAnim
			);

			for (ATWall var : VLocation[i].VWall)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture,
					var.GetNameObj()->Name,
					var.GetStatusAnimation()->StatusAnim
				);
			}
			for (ATEnemy var : VLocation[i].VEnemy)
			{

				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					ENEMY"Enemy_static_Test",
					var.GetNameObj()->Name,
					StatusAnimate::DEFAULT
				);

				d3dx.SetAnimetionTimeLine(var.GetTimeLine()->TimeLineIt, var.GetTimeLine()->TimeLineName, var.GetStatusAnimation()->Mirror, var.GetStatusAnimation()->PatternAnim);
				d3dx.DrawObject(
					var.GetPosition()->x - size /2, var.GetPosition()->y - size / 1.2, 1,
					var.GetPosition()->Width + size, var.GetPosition()->Height + size,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture,
					var.GetNameObj()->Name,
					var.GetStatusAnimation()->StatusAnim
				);
				var.GetTimeLine()->TimeLineIt = d3dx.GetTimeLineIt();
				var.GetTimeLine()->TimeLineName = d3dx.GetTimeLineName();

				var.Start();
			}
			for (ATHealFlack var : VLocation[i].VHealFlack)
			{
				d3dx.DrawObject(
					var.GetPosition()->x, var.GetPosition()->y, 1,
					var.GetPosition()->Width, var.GetPosition()->Height,
					0,
					var.GetRender()->TypeRender,
					var.GetTexture()->Texture,
					var.GetNameObj()->Name,
					var.GetStatusAnimation()->StatusAnim
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
					var.GetTexture()->Texture,
					var.GetNameObj()->Name,
					var.GetStatusAnimation()->StatusAnim
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
					var.GetTexture()->Texture,
					var.GetNameObj()->Name,
					var.GetStatusAnimation()->StatusAnim
				);
				var.GoEvent();
			}

			d3dx.DrawObject(
				Player->GetPosition()->x, Player->GetPosition()->y, 1,
				Player->GetPosition()->Width, Player->GetPosition()->Height,
				0,
				Player->GetRender()->TypeRender,
				PLAYER"Player_static_Test",
				Player->GetNameObj()->Name,
				StatusAnimate::DEFAULT
			);

			d3dx.SetAnimetionTimeLine(Player->GetTimeLine()->TimeLineIt, Player->GetTimeLine()->TimeLineName, Player->GetStatusAnimation()->Mirror, Player->GetStatusAnimation()->PatternAnim);
			d3dx.DrawObject(
				Player->GetPosition()->x	 - size /2, Player->GetPosition()->y      - size / 1.2, 1,
				Player->GetPosition()->Width + size, Player->GetPosition()->Height + size,
				0,
				Player->GetRender()->TypeRender,
				Player->GetTexture()->Texture,
				Player->GetNameObj()->Name,
				Player->GetStatusAnimation()->StatusAnim
			);
			Player->GetTimeLine()->TimeLineIt = d3dx.GetTimeLineIt();
			Player->GetTimeLine()->TimeLineName = d3dx.GetTimeLineName();

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

