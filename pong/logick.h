 #pragma once
#include "Structures.h"
#include <memory>

void InitGame()
{
    //Sprite = new player_(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    //player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };
    player = make_shared<character>(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    //player = make_shared<character>(40, 5, 3, "health_full.bmp", "health_empty.bmp");
    wolf = make_shared<Wolf>(0.2, 0.25, 0.023, 0.032, "racket.bmp");
    
    player->Sprite.speed = 30;
    player->Sprite.dx = 0;
    player->Sprite.dy = 0;
    player->Sprite.jump = 0;
    player->Sprite.gravity = 30;
    //-----------------------------location0_______________
    location[0].hBack.loadBitmapWithNativeSize("background_0.bmp");
    //location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.2, 1, "racket.bmp");//портал в локацию 1
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
    //location[0].enemy.emplace_back(0.6, 0.25, 0.023, 0.05, "enemy1.bmp", ObjectsTipe::frog);
    //___________________________location1________________
    //location[1].hBack.loadBitmapWithNativeSize("background_1.bmp");
    //location[1].portal.emplace_back(0.02, 0.89, 0.021, 0.09, 0, "racket.bmp");//портал в локацию 0
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


//void tracer_collide()
//{
//    bool coll_x_found = false;
//    bool coll_y_found = false;
//
//    float lenght = sqrt(pow(Sprite->racket.dx, 2) + pow(Sprite->racket.dy, 2));
//    for (float i = 0; i < lenght; i++)
//    {
//        if (coll_x_found && coll_y_found) return;
//
//        for (int k = 0; k < 4; k++)
//        {
//            //if (coll_x_found|| coll_y_found) break;
//
//
//            for (int j = 0; j < location[Sprite->currentLocation].walls.size(); j++)
//            {
//                float Bbox[] = {
//                    Sprite->racket.x + Sprite->racket.dx * i / lenght, Sprite->racket.y + Sprite->racket.dy * i / lenght,
//                    Sprite->racket.x + Sprite->racket.width + Sprite->racket.dx * i / lenght - 1, Sprite->racket.y + Sprite->racket.dy * i / lenght,
//                    Sprite->racket.x + Sprite->racket.width + Sprite->racket.dx * i / lenght - 1, Sprite->racket.y + Sprite->racket.height + Sprite->racket.dy * i / lenght - 1,
//                    Sprite->racket.x + Sprite->racket.dx * i / lenght, Sprite->racket.y + Sprite->racket.height + Sprite->racket.dy * i / lenght - 1
//                };
//
//                float pixel_x = Bbox[k * 2];
//                float pixel_y = Bbox[k * 2 + 1];
//
//                SetPixel(window.context, pixel_x, pixel_y, RGB(255, 255, 255));
//
//
//
//                auto walls = location[Sprite->currentLocation].walls[j].Sprite;
//                if ((pixel_x >= walls.x &&
//                    pixel_x <= walls.x + walls.width) &&
//                    (pixel_y >= walls.y &&
//                        pixel_y <= walls.y + walls.height)
//                    )
//                {
//                    float top = pixel_y - walls.y;
//                    float down = (walls.y + walls.height) - pixel_y;
//                    float left = pixel_x - walls.x;
//                    float right = (walls.x + walls.width) - pixel_x;
//
//                    float minX = min(left, right);
//                    float minY = min(top, down);
//                    Sprite->inJump = false;
//
//                    if (minX < minY && !coll_x_found)
//                    {
//                        Sprite->racket.dx = 0;
//                        coll_x_found = true;
//                        j++;
//
//                        if (left < right)
//                        {
//                            Sprite->racket.x = walls.x - Sprite->racket.width - 1;
//                        }
//                        else
//                        {
//                            Sprite->racket.x = walls.x + walls.width + 1;
//                        }
//                    }
//
//                    if (minX >= minY && !coll_y_found)
//                    {
//                        Sprite->racket.dy = 0;
//                        coll_y_found = true;
//                        j++;
//
//                        if (down < top)
//                        {
//                            Sprite->racket.y = walls.y + walls.height + 1;
//                            Sprite->racket.jump = 30;
//                        }
//                        else
//                        {
//                            Sprite->racket.y = walls.y - Sprite->racket.height - 1;
//                            Sprite->inJumpBot = false;
//                        }
//                    }
//
//                }
//
//            }
//        }
//
//    }
//
//    if (!coll_x_found) Sprite->racket.x += Sprite->racket.dx;
//    if (!coll_y_found) Sprite->racket.y += Sprite->racket.dy;
//}

//void tracer_enemy()
//{
//    bool coll_x_found = false;
//    bool coll_y_found = false;
//    float lenght = sqrt(pow(Sprite->racket.dx, 2) + pow(Sprite->racket.dy, 2));
//    for (int i = 0; i < location[Sprite->currentLocation].enemy.size();i++)
//    {
//        if (coll_x_found && coll_y_found) return;
//        auto enemy = location[Sprite->currentLocation].enemy[i].Sprite;
//        for (int k = 0; k < 4; k++)
//        {
//            //if (coll_x_found|| coll_y_found) break;
//
//
//            for (int j = 0; j < location[Sprite->currentLocation].walls.size(); j++)
//            {
//                float Bbox[] = {
//                    enemy.x + enemy.dx * i / lenght, enemy.y + enemy.dy * i / lenght,
//                    enemy.x + enemy.width + enemy.dx * i / lenght - 1, enemy.y + enemy.dy * i / lenght,
//                    enemy.x + enemy.width + enemy.dx * i / lenght - 1, enemy.y + enemy.height + enemy.dy * i / lenght - 1,
//                    enemy.x + enemy.dx * i / lenght, enemy.y + enemy.height + enemy.dy * i / lenght - 1
//                };
//
//                float pixel_x = Bbox[k * 2];
//                float pixel_y = Bbox[k * 2 + 1];
//
//                SetPixel(window.context, pixel_x, pixel_y, RGB(255, 255, 255));
//
//
//
//                auto walls = location[Sprite->currentLocation].walls[j].Sprite;
//                if ((pixel_x >= walls.x &&
//                    pixel_x <= walls.x + walls.width) &&
//                    (pixel_y >= walls.y &&
//                        pixel_y <= walls.y + walls.height)
//                    )
//                {
//                    float top = pixel_y - walls.y;
//                    float down = (walls.y + walls.height) - pixel_y;
//                    float left = pixel_x - walls.x;
//                    float right = (walls.x + walls.width) - pixel_x;
//
//                    float minX = min(left, right);
//                    float minY = min(top, down);
//                    location[Sprite->currentLocation].enemy[i].inJump = false;
//
//                    if (minX < minY && !coll_x_found)
//                    {
//                        location[Sprite->currentLocation].enemy[i].Sprite.dx = 0;
//                        coll_x_found = true;
//                        j++;
//
//                        if (left < right)
//                        {
//                            location[Sprite->currentLocation].enemy[i].Sprite.x = walls.x - enemy.width - 1;
//                        }
//                        else
//                        {
//                            location[Sprite->currentLocation].enemy[i].Sprite.x = walls.x + walls.width + 1;
//                        }
//                    }
//
//                    if (minX >= minY && !coll_y_found)
//                    {
//                        location[Sprite->currentLocation].enemy[i].Sprite.dy = 0;
//                        coll_y_found = true;
//                        j++;
//
//                        if (down < top)
//                        {
//                            location[Sprite->currentLocation].enemy[i].Sprite.y = walls.y + walls.height + 1;
//                            location[Sprite->currentLocation].enemy[i].jump = 30;
//                        }
//                        else
//                        {
//                            location[Sprite->currentLocation].enemy[i].Sprite.y = walls.y - enemy.height - 1;
//                            location[Sprite->currentLocation].enemy[i].Sprite.x -= location[Sprite->currentLocation].enemy[i].Sprite.dx;
//                            //player->inJumpBot = false;
//                        }
//                    }
//
//                }
//
//            }
//        }
//        //if (!coll_x_found) location[player->currentLocation].enemy[i].Sprite.x += location[player->currentLocation].enemy[i].Sprite.dx;
//        if (!coll_y_found) location[Sprite->currentLocation].enemy[i].Sprite.y += location[Sprite->currentLocation].enemy[i].Sprite.dy;
//    }
//   
//}


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

//void EnemyColise()
//{
//    for (int q = 0;q < location[Sprite->currentLocation].enemy.size(); q++)
//    {
//        auto enemy = location[Sprite->currentLocation].enemy[q];
//        location[Sprite->currentLocation].enemy[q].colis = false;
//        float lenght = sqrt(pow(enemy.Sprite.dx, 2) + pow(enemy.Sprite.dy, 2));
//        for (float i = 0; i < lenght; i++)
//        {
//            for (float k = 0; k < enemy.Sprite.width; k++) {
//
//                float pixel_x = enemy.Sprite.x + k + enemy.Sprite.dx / lenght * i;
//                float pixel_y = enemy.Sprite.y + enemy.Sprite.dy / lenght * i;
//
//                float px = enemy.Sprite.x + (enemy.Sprite.width / k) + enemy.Sprite.dx / lenght * i;
//                float py = enemy.Sprite.y + enemy.Sprite.dy / lenght * i;
//
//                SetPixel(window.context, pixel_x, pixel_y, RGB(255, 255, 255));
//                /*SetPixel(window.context, px, py, RGB(255, 255, 0));*/
//                for (int j = 0; j < location[Sprite->currentLocation].walls.size(); j++)
//                {
//
//                    auto walls = location[Sprite->currentLocation].walls[j].Sprite;
//
//
//                    if ((pixel_x >= walls.x &&
//                        pixel_x <= walls.x + walls.width) &&
//                        (pixel_y >= walls.y &&
//                            pixel_y <= walls.y + walls.height)
//                        )
//                    {
//                        float top = pixel_y - walls.y;
//                        float down = (walls.y + walls.height) - pixel_y;
//                        float left = pixel_x - walls.x;
//                        float right = (walls.x + walls.width) - pixel_x;
//
//                        float minX = min(left, right);
//                        float minY = min(top, down);
//                        Sprite->inJump = false;
//
//                        if (minX < minY)
//                        {
//                            if (left < right)
//                            {
//
//                                location[Sprite->currentLocation].enemy[q].Sprite.x = pixel_x - enemy.Sprite.width * 1.3;
//                                //player->inJump = true;
//                                break;
//                            }
//                            else
//                            {
//                                location[Sprite->currentLocation].enemy[q].Sprite.x = pixel_x + enemy.Sprite.width / 3;
//                                //player->inJump = true;
//                                break;
//                            }
//                        }
//                        else
//                        {
//                            if (down < top)
//                            {
//                                location[Sprite->currentLocation].enemy[q].Sprite.y = pixel_y + enemy.Sprite.jump;
//                                
//                                break;
//                            }
//                            else
//                            {
//                                location[Sprite->currentLocation].enemy[q].Sprite.y = walls.y - enemy.Sprite.height;
//                                location[Sprite->currentLocation].enemy[q].Sprite.x = +location[Sprite->currentLocation].enemy[q].step;
//                                //player->inJump = false;
//                                break;
//                            }
//                        }
//                        location[Sprite->currentLocation].enemy[q].colis = true;
//                        //return;
//                    }
//
//                }
//            }
//        }
//        
//    }
//
//}
//
//void EnemyPhisics()
//{
//    for (int q = 0;q < location[Sprite->currentLocation].enemy.size(); q++)
//    {
//        location[Sprite->currentLocation].enemy[q].Sprite.dx = location[Sprite->currentLocation].enemy[q].Sprite.x 
//            + location[Sprite->currentLocation].enemy[q].step;
//        location[Sprite->currentLocation].enemy[q].Sprite.dy = location[Sprite->currentLocation].enemy[q].Sprite.y 
//            + location[Sprite->currentLocation].enemy[q].gravity;
//        /*if (!location[player->currentLocation].enemy[q].colis)
//        {*/
//            //location[player->currentLocation].enemy[q].Sprite.y = +location[player->currentLocation].enemy[q].gravity;
//        //}
//        
//    }
//    EnemyColise();
//}

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
    player->Sprite.loadBitmapWithNativeSize("racket.bmp");
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
        player->Sprite.loadBitmapWithNativeSize("walls.bmp");
        
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



