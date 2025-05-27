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

struct player_ //структура игрока
{
    sprite racket;//игрок
    sprite hHealthFull, hHealthEmpty;
    int health_width;
    int max_lives;
    int current_lives;
    int currentLocation = 0;
    
    
    float maxjump = 10;
    bool inJump = false;
    const int dashDistance = 20;
    bool wasShiftPressed = false;
    bool colis = false;
    bool dash_allow = true;

    player_(int p_health, int p_max_lives, int p_current_lives, const char* filename, const char* filename2)
    {
        this->health_width = p_health;
        this->max_lives = p_max_lives;
        this->current_lives = p_current_lives;
        this->hHealthFull.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->hHealthEmpty.hBitmap = (HBITMAP)LoadImageA(NULL, filename2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

    player_(float p_x, float p_y, float p_width, float p_height, const char* filename)
    {
        this->racket.x = p_x * window.width;
        this->racket.y = p_y * window.height;
        this->racket.width = p_width * window.width;
        this->racket.height = p_height * window.height;
        this->racket.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

};

struct enemy_ //структура врагов
{
    sprite Sprite;
    ObjectsTipe tipe;//тип врага
    float gravity = 30;
    float step = 30;
    bool colis = false;
    
    enemy_(float p_x, float p_y, float p_width, float p_height, const char* filename, ObjectsTipe objtipe)
    {
        this->Sprite.x = p_x * window.width;
        this->Sprite.y = p_y * window.height;
        this->Sprite.width = p_width * window.width;
        this->Sprite.height = p_height * window.height;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->tipe = objtipe;
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

struct Location_
{
    sprite hBack;
    vector<portal_>portal;
    vector<Texture> locationTexture;
    vector<Texture> walls;
    vector<Objects> locationObjects;
    vector<enemy_> enemy;
};

Location_ location[5];

player_* player;
player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };


