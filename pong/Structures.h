#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>
using namespace std;
int currenttime = 0;

POINT mouse;

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
    int width_z, height_z;
} window;

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
        ShowBitmap(window.context, x, y, width, height, hBitmap, false);
    }

    void showBack()
    {
        ShowBitmap(window.context, 0, 0, window.width_z, window.height_z, hBitmap, false);
    }

};

enum class ObjectsTipe
{
    spike,
    healing,
    frog
};

struct Texture // структура платформ
{

    sprite Sprite;


    Texture(float p_x, float p_y, float p_width, float p_height, const string& filename) {
        Sprite.x = p_x * window.width_z;
        Sprite.y = p_y * window.height_z;
        Sprite.width = p_width * window.width_z;
        Sprite.height = p_height * window.height_z;
        Sprite.loadBitmapWithNativeSize(filename);;

    }

};


struct Objects // структура игровых обьектов
{
    sprite Sprite;
    ObjectsTipe type;

    Objects(float p_x, float p_y, float p_width, float p_height, const string& filename, ObjectsTipe objTipe) {
        Sprite.x = p_x * window.width_z;
        Sprite.y = p_y * window.height_z;
        Sprite.width = p_width * window.width_z;
        Sprite.height = p_height * window.height_z;
        Sprite.loadBitmapWithNativeSize(filename);
        type = objTipe;

    }

};

struct portal_
{
    sprite spr;
    int destination;
    portal_(float p_x, float p_y, float p_width, float p_height, int p_destination, const string& filename)
    {
        spr.x = p_x * window.width_z;
        spr.y = p_y * window.height_z;
        spr.width = p_width * window.width_z;
        spr.height = p_height * window.height_z;
        destination = p_destination;
        spr.loadBitmapWithNativeSize(filename);
    }
};

struct Location_
{
    sprite hBack;
    vector<portal_>portal;
    vector<Texture> locationTexture;
    vector<Texture> walls;
    vector<Objects> locationObjects;

};

Location_ location[5];

void tracer_collide(auto& Character);




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

void processGravity(auto& spriteName)
{
    spriteName.jump *= .9;
    spriteName.dx *= .5;
    spriteName.dy = spriteName.gravity - spriteName.jump;
}

vector<character*> Persona;

class Hero : public character
{
    public:
        Hero(float p_x, float p_y, float p_width, float p_height, const string& filename, int p_health, int p_max_lives, int p_current_lives) 
            : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives)
        {

            string name = __FUNCTION__; // получение имени класса (пока реализации нету)
            
            Sprite.speed = 60;
            Sprite.dx = 0;
            Sprite.dy = 0;
            Sprite.jump = 0;
            Sprite.gravity = 30;
            Persona.push_back(this);
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
};

class Wolf : public character //структура врагов
{
public:
   
    int direction = 1; // 1 - вправо, -1 - влево
    Wolf(float p_x, float p_y, float p_width, float p_height, const string& filename, int p_health, int p_max_lives, int p_current_lives)
    : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives)
    {
        Sprite.speed = 5;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        Persona.push_back(this);
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
};



Hero* player;
Wolf* wolf;
Wolf* wolf2;


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
        int startX = player->Sprite.x + window.width/2 - 50;
        int startY = player->Sprite.y - window.height/2 -10;

        for (int i = 0; i < player->max_lives; i++) {
            if (i < player->current_lives) {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_full.hBitmap, false);
               
            }
            else {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_empty.hBitmap, false);
               
            }
        }
    }

};

health_bar Health_bar;