#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>
#include <fstream>
using namespace std;
int currenttime = 0;

POINT mouse;

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

struct {
    int x = 0;
    int y = 0;
    int w;
    int h;

} player_view;

struct float2 {
    float x;
    float y;
};
float scale = 2;
int MapSizeW = window.width;
int MapSizeH = window.height;
struct sprite {
    float x, y, width, height, dx, dy, speed, jump, gravity;
    HBITMAP hBitmap;

    void loadBitmapWithNativeSize(const string& filename)
    {
        const string fullName = filename + ".bmp";
        hBitmap = (HBITMAP)LoadImageA(NULL, fullName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    }

    void show()
    {
        float vx = (x - player_view.x) * scale + window.width / 2;
        float vy = (y - player_view.y) * scale + window.height / 2;
        float vw = width * scale;
        float vh = height * scale;

        bool in = false;

        if (vx + vw >= 0 && vx < window.width &&
            vy + vh >= 0 && vy < window.height)
            in = true;

        if (!in) return;
        ShowBitmap(window.context, vx, vy, vw, vh, hBitmap, false);
    }

    void showBack()
    {
        ShowBitmap(window.context, 0, 0, window.width, window.height, hBitmap, false);
    }
};

class StaticObjects
{
public:

    sprite Sprite;
    StaticObjects(float p_x, float p_y, float p_width, float p_height, const string& filename) {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;
        Sprite.loadBitmapWithNativeSize(filename);;
    }
    bool CheckCollision(float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2)
    {
        return x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2;
    }
};

class Platform : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
};

class HealingFlask : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
    void healing(auto& player, int i);
};

class Spike : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
    void damage(auto& player);
};

class portal_ : public StaticObjects
{
public:
    int destination;
    portal_(float p_x, float p_y, float p_width, float p_height, int p_destination, const string& filename)
        : StaticObjects(p_x, p_y, p_width, p_height, filename)
    {
        destination = p_destination;
    }
    void Portal(auto& player);
};



void tracer_collide(auto& Character);
void processGravity(auto& spriteName)
{
    spriteName.jump *= .9;
    spriteName.dx *= .5;
    spriteName.dy = spriteName.gravity - spriteName.jump;
}


class character //структура игрока
{
public:
    sprite Sprite;//игрок
    int health_width;
    int max_lives;
    int current_lives;
    int currentLocation = 0;
    int last_trace_platform_num = -1;

    float maxjump = 10;
    bool inJump = false;
    const int dashDistance = 20;
    bool inJumpBot = false;
    bool wasShiftPressed = false;
    bool colis = false;
    bool dash_allow = true;

    character(float p_x, float p_y, float p_width, float p_height, const string& filename, int p_health, int p_max_lives, int p_current_lives)
    {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;

        Sprite.loadBitmapWithNativeSize(filename);

        health_width = p_health;
        max_lives = p_max_lives;
        current_lives = p_current_lives;
    }

    virtual void move() = 0;

};


struct Location_
{
    sprite hBack;
    vector<portal_>portal;
    vector<Platform> walls;
    vector<HealingFlask> healingFlask;
    vector<Spike> spike;
    vector<character*> Persona;

}location[5];
class Hero : public character
{
public:
    Hero(float p_x, float p_y, float p_width, float p_height, const string& filename, int p_health, int p_max_lives, int p_current_lives, int current_location)
        : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives)
    {

        string name = __FUNCTION__; // получение имени класса (пока реализации нету)

        Sprite.speed = 15;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        /*location[current_location].Persona.push_back(this);*/
    }



    void move()
    {
        if (GetAsyncKeyState(VK_LEFT)) {
            Sprite.dx = -Sprite.speed;
        }

        if (GetAsyncKeyState(VK_RIGHT)) {
            Sprite.dx = Sprite.speed;
        }

        if (GetAsyncKeyState(VK_SPACE) && inJump == false && inJumpBot == false)
        {
            Sprite.jump = 110;
            inJumpBot = true;
            inJump = true;
        }

        tracer_collide(*this);

        processGravity(Sprite);


    }
}*player;
class Wolf : public character //структура врагов
{
public:

    int direction = 1; // 1 - вправо, -1 - влево
    Wolf(float p_x, float p_y, float p_width, float p_height, const string& filename, int p_health, int p_max_lives, int p_current_lives, int current_location)
        : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives)
    {
        Sprite.speed = 5;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        location[current_location].Persona.push_back(this);
    }

    void move()
    {
        tracer_collide(*this);

        processGravity(Sprite);

        if (last_trace_platform_num >= 0)
        {
            if (Sprite.x <= location[currentLocation].walls[last_trace_platform_num].Sprite.x)
            {
                direction = 1;
            }

            auto& platform = location[currentLocation].walls[last_trace_platform_num].Sprite;
            if (Sprite.x + Sprite.width >= platform.x + platform.width)
            {
                direction = -1;
            }
            Sprite.dx = direction * Sprite.speed;
        }
    }
}*wolf, *wolf2;
class health_bar {
public:
    sprite health_full, health_empty;

    health_bar() {
        health_full.loadBitmapWithNativeSize("health_full");
        health_empty.loadBitmapWithNativeSize("health_empty");
    }

    void Show() {

        int h_w = 50;
        int margin = 10;
        int startX = window.width - h_w - 20;
        int startY = 10;

        for (int i = 0; i < player->max_lives; i++) {
            if (i < player->current_lives) {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_full.hBitmap, false);

            }
            else {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_empty.hBitmap, false);

            }
        }
    }

}Health_bar;


void portal_::Portal(auto& player)
{
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
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
    }
}
void HealingFlask::healing(auto& player, int i)
{
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        location[player->currentLocation].healingFlask.erase(location[player->currentLocation].healingFlask.begin() + i);
        player->current_lives++;
    }
}
void Spike::damage(auto& player)
{
    static int lastDamageTime = 0;
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
    }
}



