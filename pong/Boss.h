#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
#include "paint.h"
#include "logick.h"

using namespace std;
int currenttime = 0;


//void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);
POINT mouse;

struct {
    int score, balls;
    bool action = false;
} game;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

HBITMAP hBack;// хэндл для фонового изображения
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


HBITMAP ballBitmap;
HBITMAP frogHbm;
float frogWidth;
float frogHeight;
int oldtime = 0;


const int slots_count = 6;

void loadFrog()
{
    frogHbm = (HBITMAP)LoadImageA(NULL, "frog.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    BITMAP bm;
    GetObject(frogHbm, (int)sizeof bm, &bm);
    frogWidth = bm.bmWidth;
    frogHeight = bm.bmHeight;
}
//void spawnEnemy()
//{
//    for (int i = 0;i < slots_count;i++)
//    {
//        if (frog[i].dead)
//        {
//
//            if (currenttime > frog[i].spawnTime + 1000)
//            {
//                sprite f;
//
//                f.x = (window.width / slots_count) * i;
//                f.y = 0;
//                f.width = frogWidth;
//                f.height = frogHeight;
//                f.hBitmap = frogHbm;
//
//                enemy e;
//                e.enemy_sprite = f;
//                e.dead = false;
//                e.HPfrog = rand() % 3 + 1;
//                frog[i] = e;
//
//
//            }
//            break;
//
//        }
//    }
//
//
//}
struct Texture // структура платформ
{

    sprite textureSprite;


    Texture(float p_x, float p_y, float p_width, float p_height, const char* filename) {
        this->textureSprite.x = p_x * window.width;
        this->textureSprite.y = p_y * window.height;
        this->textureSprite.width = p_width * window.width;
        this->textureSprite.height = p_height * window.height;
        this->textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    }

};

struct Objects // структура игровых обьектов
{


    sprite objectsSprite;
    string type;

    Objects(float p_x, float p_y, float p_width, float p_height, const char* filename, const char* objTipe) {
        this->objectsSprite.x = p_x * window.width;
        this->objectsSprite.y = p_y * window.height;
        this->objectsSprite.width = p_width * window.width;
        this->objectsSprite.height = p_height * window.height;
        this->objectsSprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

player_* player;

player_ health{ 40, 5, 3, "health_full.bmp", "health_empty.bmp" };

int currentLocation = 0;

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

struct enemy {

    sprite enemy_sprite;
    vector<sprite> bullet;
    int bullettime = 0;
    bool dead = true;
    int spawnTime = 0;
    int HPfrog = 3;           //rand()% 3-5

    void showBullet()
    {
        for (int i = 0; i < bullet.size(); i++)
        {
            ShowBitmap(window.context, bullet[i].x - bullet[i].rad, bullet[i].y - bullet[i].rad, 2 * bullet[i].rad, 2 * bullet[i].rad, bullet[i].hBitmap);
        }
    }

    void processBullet()
    {
        if (currenttime > bullettime + 5000)
        {
            //for (int i = 0; i < bullet.size(); i++)
            {
                sprite b;
                b.height = 40;
                b.width = 40;
                b.speed = rand() % (20 - 1) + 4;
                b.x = enemy_sprite.x;
                b.y = enemy_sprite.y;
                b.dx = player->racket.x - b.x;
                b.dy = player->racket.y - b.y;

                float dvector = sqrt(b.dx * b.dx + b.dy * b.dy);
                b.dx = b.dx / dvector;
                b.dy = b.dy / dvector;
                b.rad = 20;
                b.hBitmap = ballBitmap;

                //game.action = true;

                bullet.push_back(b);
                bullettime = currenttime;
            }
        }

        for (int i = 0; i < bullet.size(); i++)
        {
            float margin = 0;

            if ((bullet[i].x > window.width - margin) || (bullet[i].x < margin) ||
                (bullet[i].y < margin))
            {
                bullet[i].speed = 0;
            }

            bullet[i].x += bullet[i].dx * bullet[i].speed;
            bullet[i].y += bullet[i].dy * bullet[i].speed;
        }

        for (int i = 0; i < bullet.size(); i++)
        {
            if (bullet[i].speed < .1)
            {
                bullet.erase(bullet.begin() + i);
            }
        }
    }
};
enemy frog[slots_count];
sprite ball;
vector<sprite> bullet;

