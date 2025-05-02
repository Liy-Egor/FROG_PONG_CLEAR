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
    int score, balls;
    bool action = false;
} game;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

// HBITMAP hBack;// хэндл для фонового изображения????????????????
// секция данных игры  

struct sprite {
    float x, y, width, height, rad, dx, dy, speed, time;
    bool vect_right = true, vect_left = false;
    HBITMAP hBitmap;//хэндл к спрайту шарика 

    void loadBitmapWithNativeSize(const char* filename)
    {
        hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    }

    void show()
    {
        ShowBitmap(window.context, x, y, width, height, hBitmap, false);
    }


};

struct Texture // структура платформ
{

    sprite Sprite;


    Texture(float p_x, float p_y, float p_width, float p_height, const char* filename) {
        this->Sprite.x = p_x * window.width;
        this->Sprite.y = p_y * window.height;
        this->Sprite.width = p_width * window.width;
        this->Sprite.height = p_height * window.height;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    }

};

struct Objects // структура игровых обьектов
{


    sprite Sprite;
    string type;

    Objects(float p_x, float p_y, float p_width, float p_height, const char* filename, const char* objTipe) {
        this->Sprite.x = p_x * window.width;
        this->Sprite.y = p_y * window.height;
        this->Sprite.width = p_width * window.width;
        this->Sprite.height = p_height * window.height;
        this->Sprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->type = objTipe;

    }

};

struct player_ //структура игрока
{
    sprite racket;//игрок
    HBITMAP hHealthFull, hHealthEmpty;
    int health_width;
    int max_lives;
    int current_lives;
    int currentLocation = 0;

    player_(int p_health, int p_max_lives, int p_current_lives, const char* filename, const char* filename2)
    {
        this->health_width = p_health;
        this->max_lives = p_max_lives;
        this->current_lives = p_current_lives;
        this->hHealthFull = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->hHealthEmpty = (HBITMAP)LoadImageA(NULL, filename2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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


struct portal_ {
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

struct Location_ {
    HBITMAP hBack;
    int LeftPort;
    int RightPort;
    vector<portal_>portal;
    vector<Texture> locationTexture;
    vector<Texture> walls;
    vector<Objects> locationObjects;
};
Location_ location[5];

player_* player;

player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };


