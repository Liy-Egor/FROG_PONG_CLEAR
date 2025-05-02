#pragma once
#include "Boss.h"

void InitGame()
{
    player = new player_(0.2, 0.25, 0.012, 0.021, "racket.bmp");
    player->racket.speed = 30;
    //-------------------------------------------------------
    loadBitmap("ball.bmp", ballBitmap);
    //loadFrog();
    //-----------------------------location0_______________
    loadBitmap("background_0.bmp", location[0].hBack);
    location[0].RightPort = 1;
    location[0].LeftPort = -1;
    location[0].locationObjects.emplace_back(0.3, 0.98, 0.012, 0.021, "ball.bmp", "healing");
    location[0].locationObjects.emplace_back(0.5, 0.98, 0.012, 0.021, "spike.bmp", "spike");
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    location[0].portal.emplace_back(0.98, 0.05, 0.021, 0.9, 1, "racket.bmp");
    //___________________________location1________________
    loadBitmap("background_1.bmp", location[1].hBack);
    location[1].RightPort = -1;
    location[1].LeftPort = 0;
    location[1].locationTexture.emplace_back(0.08, 0.9, 0.15, 0.05, "racket_enemy.bmp");
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

float clickTimeOut = 100;
float clickTime = 0;

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}


void ProcessInput()
{
    if (GetAsyncKeyState(VK_LEFT)) player->racket.x -= player->racket.speed;
    if (GetAsyncKeyState(VK_RIGHT)) player->racket.x += player->racket.speed;
    clickTime = timeGetTime();

    if (GetAsyncKeyState(VK_LBUTTON) && clickTime > clickTimeOut)
    {
        sprite b;
        b.speed = 10;
        b.x = player->racket.x;
        b.y = player->racket.y;

        b.dx = mouse.x - b.x;
        b.dy = mouse.y - b.y;
        float dvector = sqrt(b.dx * b.dx + b.dy * b.dy);
        b.dx = b.dx / dvector;
        b.dy = b.dy / dvector;
        b.rad = 20;
        b.hBitmap = ballBitmap;

        game.action = true;

        bullet.push_back(b);

        ProcessSound("bounce1.wav");

        clickTimeOut = clickTime + 1000;


    }


    for (auto& i : location[player->currentLocation].portal)
    {
        i.spr.show();

        if (CheckCollision(player->racket.x, player->racket.y, player->racket.width, player->racket.height, i.spr.x, i.spr.y, i.spr.width, i.spr.height))
        {
            player->currentLocation = i.destination;
            player->racket.x = 0;
        }
    }
}