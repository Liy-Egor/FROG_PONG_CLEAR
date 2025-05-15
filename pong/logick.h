#pragma once
#include "Structures.h"

void InitGame()
{
    player = new player_(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    player->racket.speed = 30;
    //-----------------------------location0_______________
    location[0].hBack.loadBitmapWithNativeSize("background_0.bmp");
    location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.2, 1, "racket.bmp");//портал в локацию 1
   // location[0].walls.emplace_back(0.5, 0, 0.009, 0.99, "walls.bmp");//левая стена
    location[0].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//правая стена
    //location[0].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//пол
    location[0].walls.emplace_back(0, 0, 0.999, 0.04, "walls.bmp");//потолок
    location[0].locationObjects.emplace_back(0.3, 0.98, 0.012, 0.021, "ball.bmp", "healing");
    location[0].locationObjects.emplace_back(0.5, 0.98, 0.025, 0.025, "spike.bmp", "spike");
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    //location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.6, 0.95, 0.15, 0.05, "walls.bmp");
    //___________________________location1________________
    location[1].hBack.loadBitmapWithNativeSize("background_1.bmp");
    location[1].portal.emplace_back(0.02, 0.89, 0.021, 0.09, 0, "racket.bmp");//портал в локацию 0
    location[1].walls.emplace_back(0, 0, 0.02, 0.99, "walls.bmp");//левая стена
    location[1].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//правая стена
    location[1].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//пол
    location[1].walls.emplace_back(0, 0, 0.999, 0.02, "walls.bmp");//потолок
    location[1].locationTexture.emplace_back(0.08, 0.9, 0.15, 0.05, "racket_enemy.bmp");
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

float clickTimeOut = 100;
float clickTime = 0;
void ProcessInput()
{
    
    player->racket.dx = player->racket.x; // и Дельта для вектора движения если игрок стоит на месте
    player->tdx = player->racket.x;
    if (GetAsyncKeyState(VK_LEFT)) 
    {
        player->racket.x -= player->racket.speed;
        player->tdx = player->racket.x - player->racket.speed;
        
    }

    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        player->racket.x += player->racket.speed;
        player->tdx = player->racket.x + player->racket.speed;
    }
    if (GetAsyncKeyState(VK_SPACE) && player->inJump == false)
    {

        player->jump = 100;
        player->inJump = true;
    }
    player->racket.y += player->gravity - player->jump;
    float s = .9;
    player->jump *= s;
    player->racket.y = min(window.height - player->racket.height, player->racket.y);
    
}

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}


void ProcessPortal()
{
    for (auto& i : location[player->currentLocation].portal)
    {
        i.spr.show();

        if (CheckCollision(player->racket.x, player->racket.y, player->racket.width, player->racket.height, i.spr.x, i.spr.y, i.spr.width, i.spr.height))
        {
            player->currentLocation = i.destination;
            if (i.destination % 2 == 0)
            {
                player->racket.x = 1500;
            }
            else player->racket.x = 100;
            
        }
    }
}


HANDLE hTimer;
void TweenDash(int idx)
{
    WaitForSingleObject(hTimer, INFINITE);

    if (player->dash_allow == true)
        if (player->racket.vect_right == true)
        {
            player->racket.x += idx;
        }
        else
        {
            player->racket.x -= idx;
        }


}

void ProcessDash()
{
    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    if (isShiftPressed && !player->wasShiftPressed)
    {
        player->wasShiftPressed = true;

        hTimer = CreateWaitableTimer(NULL, false, NULL);
        LARGE_INTEGER dTime;
        for (int i = 0; i < player->dashDistance; i++)
        {

            thread t(TweenDash, i);
            t.detach();

        }
        dTime.QuadPart = -10000 * 7;
        SetWaitableTimer(hTimer, &dTime, 7, NULL, NULL, false);

    }
    else if (!isShiftPressed)
    {
        player->wasShiftPressed = false;
        player->dash_allow = true;
    }


}



void CollisionGroup()
{
    static int lastDamageTime = 0;
    bool spikeCollision = false;
    player->racket.loadBitmapWithNativeSize("racket.bmp");
    for (int i = 0; i < location[player->currentLocation].locationObjects.size(); ++i)
    {
        Objects& obj = location[player->currentLocation].locationObjects[i];
        if (obj.type == "spike")
        {
            if (player->racket.x + player->racket.width >= obj.Sprite.x &&
                player->racket.x <= obj.Sprite.x + obj.Sprite.width &&
                player->racket.y + player->racket.height >= obj.Sprite.y &&
                player->racket.y <= obj.Sprite.y + obj.Sprite.height)
            {
                spikeCollision = true;
                break;
            }
        }
        if (obj.type == "healing")
        {
            if (player->racket.x + player->racket.width >= obj.Sprite.x &&
                player->racket.x <= obj.Sprite.x + obj.Sprite.width &&
                player->racket.y + player->racket.height >= obj.Sprite.y &&
                player->racket.y <= obj.Sprite.y + obj.Sprite.height)
            {
                location[player->currentLocation].locationObjects.erase(location[player->currentLocation].locationObjects.begin());
                health.current_lives++;
                break;
            }
        }

    }
    if (spikeCollision && currenttime > lastDamageTime + 1000) {
        health.current_lives--;
        lastDamageTime = currenttime;
        player->jump = 200;
        player->racket.x += 20;
        player->inJump = true;
        player->racket.loadBitmapWithNativeSize("walls.bmp");
    }
    /*if (health.current_lives <= 0) {
        MessageBox(window.hWnd, "Game Over!", "Info", MB_OK);
        exit(0);
    }*/
    player->jump *= .3;
}



