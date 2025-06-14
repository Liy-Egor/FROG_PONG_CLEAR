 #pragma once
#include "Structures.h"
#include <memory>

void InitGame()
{
    //player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };
    //player = make_shared<character>(40, 5, 3, "health_full.bmp", "health_empty.bmp");
    player = make_shared<character>(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    wolf = make_shared<Wolf>(0.25, 0.25, 0.023, 0.05, "enemy1.bmp");
    
    player->Sprite.speed = 60;
    player->Sprite.dx = 0;
    player->Sprite.dy = 0;
    player->Sprite.jump = 0;
    player->Sprite.gravity = 30;

    wolf->Sprite.speed = 5;
    wolf->Sprite.dx = 0;
    wolf->Sprite.dy = 0;
    wolf->Sprite.jump = 0;
    wolf->Sprite.gravity = 30;
    //-----------------------------location0_______________
    location[0].hBack.loadBitmapWithNativeSize("background_0.bmp");
    //location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.2, 1, "racket.bmp");//������ � ������� 1
    location[0].walls.emplace_back(0, 0, 0.009, 0.99, "walls.bmp");//����� �����
    location[0].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//������ �����
    location[0].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//���
    location[0].walls.emplace_back(0, 0, 0.999, 0.04, "walls.bmp");//�������
    location[0].locationObjects.emplace_back(0.3, 0.955, 0.02, 0.025, "ball.bmp", ObjectsTipe::healing);
    location[0].locationObjects.emplace_back(0.5, 0.955, 0.025, 0.025, "spike.bmp", ObjectsTipe::spike);
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    //location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.05, 0.9, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.7, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.6, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.5, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.3, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.2, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.1, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.1, 0.4, 0.05, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.3, 0.85, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.6, 0.93, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.8, 0.83, 0.15, 0.05, "walls.bmp");
    //-----------------------------------------------------------------------------
    
    //___________________________location1________________
    ////location[1].hBack.loadBitmapWithNativeSize("background_1.bmp");
    ////location[1].portal.emplace_back(0.02, 0.89, 0.021, 0.09, 0, "racket.bmp");//������ � ������� 0
    //location[1].walls.emplace_back(0, 0, 0.02, 0.99, "walls.bmp");//����� �����
    //location[1].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//������ �����
    //location[1].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//���
    //location[1].walls.emplace_back(0, 0, 0.999, 0.02, "walls.bmp");//�������
    //location[1].locationTexture.emplace_back(0.08, 0.9, 0.15, 0.05, "racket_enemy.bmp");
}

void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
}



void ProcessInput()
{


    if (GetAsyncKeyState(VK_LEFT)) {
        player->Sprite.dx = -player->Sprite.speed;
    }

    if (GetAsyncKeyState(VK_RIGHT)) {
        player->Sprite.dx = player->Sprite.speed;
    }

    if (GetAsyncKeyState(VK_SPACE) && player->inJump == false && player->inJumpBot == false)
    {
        player->Sprite.jump = 110;
        player->inJumpBot = true;
        player->inJump = true;
    }
    float s = .9;
    player->Sprite.jump *= s;
    player->Sprite.dx *= .5;
    player->Sprite.dy = player->Sprite.gravity - player->Sprite.jump;

    wolf->Sprite.jump *= s;
    wolf->Sprite.dx *= .5;
    wolf->Sprite.dy = wolf->Sprite.gravity - wolf->Sprite.jump;
   
    vector<shared_ptr<character>> Persona;
    Persona.push_back(player);
    Persona.push_back(wolf);
    for (int i = 0; i < Persona.size(); i++) {

        Persona[i]->tracer_collide();

    }
}

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}


//void ProcessPortal()
//{
//    for (auto& i : location[Sprite->currentLocation].portal)
//    {
//        i.spr.show();
//
//        if (CheckCollision(Sprite->racket.x, Sprite->racket.y, Sprite->racket.width, Sprite->racket.height, i.spr.x, i.spr.y, i.spr.width, i.spr.height))
//        {
//            Sprite->currentLocation = i.destination;
//            if (i.destination % 2 == 0)
//            {
//                Sprite->racket.x = 1500;
//            }
//            else Sprite->racket.x = 100;
//            
//        }
//    }
//}


HANDLE hTimer;
//void TweenDash(int idx)
//{
//    WaitForSingleObject(hTimer, INFINITE);
//
//    if (Sprite->dash_allow == true)
//        if (Sprite->racket.vect_right == true)
//        {
//            Sprite->racket.x += idx;
//        }
//        else
//        {
//            Sprite->racket.x -= idx;
//        }
//
//
//}

//void ProcessDash()
//{
//    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
//    if (isShiftPressed && !Sprite->wasShiftPressed)
//    {
//        Sprite->wasShiftPressed = true;
//
//        hTimer = CreateWaitableTimer(NULL, false, NULL);
//        LARGE_INTEGER dTime;
//        for (int i = 0; i < Sprite->dashDistance; i++)
//        {
//
//            thread t(TweenDash, i);
//            t.detach();
//
//        }
//        dTime.QuadPart = -10000 * 7;
//        SetWaitableTimer(hTimer, &dTime, 7, NULL, NULL, false);
//
//    }
//    else if (!isShiftPressed)
//    {
//        Sprite->wasShiftPressed = false;
//        Sprite->dash_allow = true;
//    }
//
//
//}



void CollisionGroup()
{
    static int lastDamageTime = 0;
    bool spikeCollision = false;
   // player->Sprite.loadBitmapWithNativeSize("racket.bmp");
    for (int i = 0; i < location[player->currentLocation].locationObjects.size(); ++i)
    {
        Objects& obj = location[player->currentLocation].locationObjects[i];
        if (obj.type == ObjectsTipe::spike)
        {
            if (player->Sprite.x + player->Sprite.width >= obj.Sprite.x &&
                player->Sprite.x <= obj.Sprite.x + obj.Sprite.width &&
                player->Sprite.y + player->Sprite.height >= obj.Sprite.y &&
                player->Sprite.y <= obj.Sprite.y + obj.Sprite.height)
            {
                spikeCollision = true;
                break;
            }
        }
        if (obj.type == ObjectsTipe::healing)
        {
            if (player->Sprite.x + player->Sprite.width >= obj.Sprite.x &&
                player->Sprite.x <= obj.Sprite.x + obj.Sprite.width &&
                player->Sprite.y + player->Sprite.height >= obj.Sprite.y &&
                player->Sprite.y <= obj.Sprite.y + obj.Sprite.height)
            {
                location[player->currentLocation].locationObjects.erase(location[player->currentLocation].locationObjects.begin());
                player->current_lives++;
                break;
            }
        }

    }
    if (spikeCollision && currenttime > lastDamageTime + 1000) {
        player->current_lives--;
        lastDamageTime = currenttime;
        player->Sprite.jump = 60;
        player->Sprite.x += 20;
        player->inJump = true;
        //player->Sprite.loadBitmapWithNativeSize("walls.bmp");
        
    }
    /*if (health.current_lives <= 0) {
        MessageBox(window.hWnd, "Game Over!", "Info", MB_OK);
        exit(0);
    }*/
    
}

//void enemyPhysics()
//{
//    for (int i = 0; i < location[player->currentLocation].enemy.size();i++) {
//        location[player->currentLocation].enemy[i].Sprite.y += location[player->currentLocation].enemy[i].gravity;
//        if(CheckCollision())
//    }
//}



