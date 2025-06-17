#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
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

    void loadBitmapWithNativeSize(const char* filename)
    {
        hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

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


    Texture(float p_x, float p_y, float p_width, float p_height, const char* filename) {
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

    Objects(float p_x, float p_y, float p_width, float p_height, const char* filename, ObjectsTipe objTipe) {
        Sprite.x = p_x * window.width_z;
        Sprite.y = p_y * window.height_z;
        Sprite.width = p_width * window.width_z;
        Sprite.height = p_height * window.height_z;
        Sprite.loadBitmapWithNativeSize(filename);
        type = objTipe;

    }

};


struct Location_
{
    sprite hBack;
    //vector<portal_>portal;
    vector<Texture> locationTexture;
    vector<Texture> walls;
    vector<Objects> locationObjects;

};

Location_ location[5];

void tracer_collide(auto& Character);

class health_bar {
public:
    sprite health_full, health_empty;
    int health;
    int max_hp;

    health_bar(int health, int max_hp, const char* max_hp_bitmap, const char* max_empty_bitmap) {
        this->max_hp = max_hp;
        this->health = health;
        health_full.loadBitmapWithNativeSize(max_hp_bitmap);
        health_empty.loadBitmapWithNativeSize(max_empty_bitmap);
    }

    void showHealth(int i, int h_w)
    {

        //ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, hBitmap, false);
    }

    void Show(int i, int h_w) {

        int margin = 10;
        int startX = window.width - 50;
        int startY = 10;

        for (int i = 0; i < max_hp; i++) {
            if (i < health) {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_full.hBitmap, false);
            }
            else {
                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_empty.hBitmap, false);
            }
        }
    }


};

//health_bar Health_bar;

class character //структура игрока
{
public:
    sprite Sprite;//игрок
    sprite hHealthFull, hHealthEmpty;
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

    character(float p_x, float p_y, float p_width, float p_height, const char* filename, int p_health, int p_max_lives, int p_current_lives, const char* AliveFilename, const char* DeadFilename)
    {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;
        Sprite.loadBitmapWithNativeSize(filename);
        
        health_width = p_health;
        max_lives = p_max_lives;
        current_lives = p_current_lives;
        hHealthFull.loadBitmapWithNativeSize(AliveFilename);
        hHealthEmpty.loadBitmapWithNativeSize(DeadFilename);
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
        Hero(float p_x, float p_y, float p_width, float p_height, const char* filename, int p_health, int p_max_lives, int p_current_lives, const char* AliveFilename, const char* DeadFilename) 
            : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives, AliveFilename, DeadFilename)
        {
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
    Wolf(float p_x, float p_y, float p_width, float p_height, const char* filename, int p_health, int p_max_lives, int p_current_lives, const char* AliveFilename, const char* DeadFilename)
    : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives, AliveFilename, DeadFilename)
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

struct portal_ 
{
    sprite spr;
    int destination;
    portal_(float p_x, float p_y, float p_width, float p_height, int p_destination, const char* filename)
    {
        spr.x = p_x * window.width;
        spr.y = p_y * window.height;
        spr.width = p_width * window.width;
        spr.height = p_height * window.height;
        destination = p_destination;
        spr.loadBitmapWithNativeSize(filename);
    }
};



Hero* player;
Wolf* wolf;
Wolf* wolf2;



