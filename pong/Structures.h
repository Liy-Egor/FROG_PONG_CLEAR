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
    float x, y, width, height, rad, dx, dy, speed, time, jump, gravity;
    bool vect_right = true, vect_left = false;
    bool colis;
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

    void showHealth(int i, int h_w)
    {
        int margin = 10;
        int startX = window.width - 50;
        int startY = 10;
        ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, hBitmap, false);
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
        this->Sprite.x = p_x * window.width_z;
        this->Sprite.y = p_y * window.height_z;
        this->Sprite.width = p_width * window.width_z;
        this->Sprite.height = p_height * window.height_z;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    }

};


struct Objects // структура игровых обьектов
{
    sprite Sprite;
    ObjectsTipe type;

    Objects(float p_x, float p_y, float p_width, float p_height, const char* filename, ObjectsTipe objTipe) {
        this->Sprite.x = p_x * window.width_z;
        this->Sprite.y = p_y * window.height_z;
        this->Sprite.width = p_width * window.width_z;
        this->Sprite.height = p_height * window.height_z;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->type = objTipe;

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

    character(int p_health, int p_max_lives, int p_current_lives, const char* filename, const char* filename2)
    {
        this->health_width = p_health;
        this->max_lives = p_max_lives;
        this->current_lives = p_current_lives;
        this->hHealthFull.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->hHealthEmpty.hBitmap = (HBITMAP)LoadImageA(NULL, filename2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

    character(float p_x, float p_y, float p_width, float p_height, const char* filename)
    {
        this->Sprite.x = p_x * window.width;
        this->Sprite.y = p_y * window.height;
        this->Sprite.width = p_width * window.width;
        this->Sprite.height = p_height * window.height;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
   
    virtual void move()
    {
        tracer_collide(*this);
    }
    
};

class Wolf : public character //структура врагов
{
public:
   
    int direction = 1; // 1 - вправо, -1 - влево
    Wolf(float p_x, float p_y, float p_width, float p_height, const char* filename):
        character(p_x, p_y, p_width, p_height, filename)
    {
        this->Sprite.x = p_x * window.width;
        this->Sprite.y = p_y * window.height;
        this->Sprite.width = p_width * window.width;
        this->Sprite.height = p_height * window.height;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

    void move()
    {
        tracer_collide(*this);
        if (last_trace_platform_num >= 0)
        {
            if (Sprite.x <= location[currentLocation].walls[last_trace_platform_num].Sprite.x)
            {
                direction = 1;
            }
            else if (Sprite.x + Sprite.width >= location[currentLocation].walls[last_trace_platform_num].Sprite.x + location[currentLocation].walls[last_trace_platform_num].Sprite.width)
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
        this->spr.x = p_x * window.width;
        this->spr.y = p_y * window.height;
        this->spr.width = p_width * window.width;
        this->spr.height = p_height * window.height;
        this->destination = p_destination;
        this->spr.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
};



shared_ptr<character> player;
shared_ptr<Wolf> wolf;
shared_ptr<Wolf> wolf2;

//player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };


