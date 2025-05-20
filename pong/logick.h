 #pragma once
#include "Structures.h"

void InitGame()
{
    player = new player_(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    player->racket.speed = 30;
    player->racket.dx = 0;
    player->racket.dy = 0;
    player->racket.jump = 0;
    player->racket.gravity = 30;
    //-----------------------------location0_______________
    location[0].hBack.loadBitmapWithNativeSize("background_0.bmp");
    location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.2, 1, "racket.bmp");//портал в локацию 1
    location[0].walls.emplace_back(0, 0, 0.009, 0.99, "walls.bmp");//левая стена
    location[0].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//правая стена
    location[0].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//пол
    location[0].walls.emplace_back(0, 0, 0.999, 0.04, "walls.bmp");//потолок
    location[0].locationObjects.emplace_back(0.3, 0.955, 0.02, 0.025, "ball.bmp", ObjectsTipe::healing);
    location[0].locationObjects.emplace_back(0.5, 0.955, 0.025, 0.025, "spike.bmp", ObjectsTipe::spike);
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    //location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.3, 0.85, 0.15, 0.05, "walls.bmp");
    location[0].walls.emplace_back(0.6, 0.93, 0.15, 0.05, "walls.bmp");
    //-----------------------------------------------------------------------------
    location[0].enemy.emplace_back(0.6, 0.25, 0.023, 0.032, "walls.bmp", ObjectsTipe::frog);
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


void tracer_collide()
{
    player->colis = false;
    float lenght = sqrt(pow(player->racket.dx, 2) + pow(player->racket.dy, 2));
    for (float i = 0; i < lenght; i++)
    {

        float pixel_x = player->racket.x + player->racket.width/2 + player->racket.dx / lenght * i;
        float pixel_y = player->racket.y + player->racket.dy / lenght * i;

        SetPixel(window.context, pixel_x, pixel_y, RGB(255, 255, 255));

        for (int j = 0; j < location[player->currentLocation].walls.size(); j++)
        {

            auto walls = location[player->currentLocation].walls[j].Sprite;
            if ((pixel_x >= walls.x  &&
                pixel_x <= walls.x + walls.width) &&
                (pixel_y >= walls.y  &&
                pixel_y <= walls.y  + walls.height)
                )
            {
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

                        player->racket.x = pixel_x - player->racket.width;
                        //player->inJump = true;
                        break;
                    }
                    else
                    {
                        player->racket.x = walls.x + walls.width;
                        //player->inJump = true;
                        break;
                    }
                }
                else
                {
                    if (down < top)
                    {
                        //player->racket.y = walls.y + walls.height + player->racket.height;
                        player->racket.y = pixel_y + player->racket.jump;
                        player->racket.jump = 48;
                        //player->inJump = true;
                        break;
                    }
                    else
                    {
                        player->racket.y = walls.y - player->racket.height;
                        //player->racket.jump = 0;
                        //player->inJump = false;
                        break;
                    }
                }
                player->colis = true;
                //return;
            }
           
        }
    }
}

void ProcessInput()
{
    
    if (!player->colis)
    {
        player->racket.x += player->racket.dx;

        player->racket.y += player->racket.dy;

    }

    if (GetAsyncKeyState(VK_LEFT)) {
        player->racket.dx = -player->racket.speed;
    }

    if (GetAsyncKeyState(VK_RIGHT)) {
        player->racket.dx = player->racket.speed;
    }

    if (GetAsyncKeyState(VK_SPACE) && player->inJump == false)
    {
        player->racket.jump = 110;
        player->inJump = true;
    }
    float s = .9;
    player->racket.jump *= s;

    player->racket.dx *= .5;
    player->racket.dy = player->racket.gravity - player->racket.jump;

    tracer_collide();
    
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
        if (obj.type == ObjectsTipe::spike)
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
        if (obj.type == ObjectsTipe::healing)
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
        player->racket.jump = 60;
        player->racket.x += 20;
        player->inJump = true;
        player->racket.loadBitmapWithNativeSize("walls.bmp");
        
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



