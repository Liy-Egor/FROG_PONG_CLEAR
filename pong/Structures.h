#pragma once

#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>

//using namespace std;
int currenttime = 0;

POINT mouse;

void ShowBitmap( int x, int y, int x1, int y1,int ID);

std::vector<uint8_t> ConvertHBITMAPToRGBA(HBITMAP hBitmap, uint32_t& width, uint32_t& height)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    width = bmp.bmWidth;
    height = bmp.bmHeight;

    std::vector<uint8_t> rgbaData(width * height * 4);

    HDC hdc = GetDC(nullptr);
    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -static_cast<LONG>(height); // top-down
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    if (!GetDIBits(hdc, hBitmap, 0, height, rgbaData.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS))
    {
        OutputDebugStringA("GetDIBits failed\n");
        rgbaData.clear();
    }

    ReleaseDC(nullptr, hdc);

    return rgbaData;
}

struct {
    int x = 0;
    int y = 0;
    int w;
    int h;

} player_view;

struct sprite {
    float x, y, width, height, dx, dy, speed, jump, gravity;
    int id;
    HBITMAP sprite;

    void loadBitmapWithNativeSize(const std::string& filename,int texture_id)
    {
        id = texture_id;
    }

    void show()
    {
        float scale = 2;

        float vx = (x - player_view.x) * scale + window.width / 2;
        float vy = (y - player_view.y) * scale + window.height / 2;

        ShowBitmap( vx, vy, width * scale, height * scale, id);

    }

    void showBack()
    {

        ShowBitmap( 0, 0, window.width, window.height, id);
    }

};

enum class ObjectsTipe
{
    spike,
    healing,
    frog,
    subject,
    wall,
    texture
};

struct Texture // структура платформ
{

    sprite Sprite;
    ObjectsTipe type;

    Texture(float p_x, float p_y, float p_width, float p_height, const std::string& filename, ObjectsTipe objTipe= ObjectsTipe::wall) {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;
        type = objTipe;
        // Назначение текстуры по типу объекта
        switch (type) {
        case ObjectsTipe::texture:
            Sprite.id = 8;  
            break;
        default:
            Sprite.id = 3;  // background_0.bmp
        }


        
        Sprite.loadBitmapWithNativeSize(filename, Sprite.id);
    }

};


struct Objects // структура игровых обьектов
{
    sprite Sprite;
    ObjectsTipe type;
    Objects(float p_x, float p_y, float p_width, float p_height, const std::string& filename, ObjectsTipe objTipe,int customId = -1) {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;
        int texture_id;
        type = objTipe;
        if (customId != -1) {
            Sprite.id = customId;
        }
        else {
            // Назначение текстуры по типу объекта
            switch (type) {
            case ObjectsTipe::spike:
                Sprite.id = 5;  // spike.bmp
                break;
            case ObjectsTipe::healing:
                Sprite.id = 4;  // ball.bmp
                break;
            case ObjectsTipe::subject:
                Sprite.id = 2;  // walls.bmp
                break;
            default:
                Sprite.id = 1;  // background_0.bmp
            }
        }
        Sprite.loadBitmapWithNativeSize(filename,Sprite.id);
        

    }

};

struct portal_
{
    sprite spr;
    int destination;
    portal_(float p_x, float p_y, float p_width, float p_height, int p_destination, const std::string& filename)
    {
        spr.x = p_x * window.width;
        spr.y = p_y * window.height;
        spr.width = p_width * window.width;
        spr.height = p_height * window.height;
        destination = p_destination;
        spr.loadBitmapWithNativeSize(filename,2);
    }
};



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

    character(float p_x, float p_y, float p_width, float p_height, const std::string& filename, int p_health, int p_max_lives, int p_current_lives)
    {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;

        Sprite.loadBitmapWithNativeSize(filename,2);

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


struct Location_
{
    sprite hBack;
    std::vector<portal_>portal;
    std::vector<Texture> locationTexture;
    std::vector<Texture> walls;
    std::vector<Objects> locationObjects;
    std::vector<character*> Persona;

};

Location_ location[5];


class Hero : public character
{
public:
    Hero(float p_x, float p_y, float p_width, float p_height, const std::string& filename, int p_health, int p_max_lives, int p_current_lives, int current_location)
        : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives)
    {

        std::string name = __FUNCTION__; // получение имени класса (пока реализации нету)

        Sprite.speed = 60;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        //location[current_location].Persona.push_back(this);
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
    Wolf(float p_x, float p_y, float p_width, float p_height, const std::string& filename, int p_health, int p_max_lives, int p_current_lives, int current_location)
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
};



Hero* player;
Wolf* wolf;
Wolf* wolf2;


class health_bar {
public:
    sprite health_full, health_empty;

    health_bar() {
        health_full.loadBitmapWithNativeSize("health_full",6);
        health_empty.loadBitmapWithNativeSize("health_empty",7);
    }

    void Show() {

        int h_w = 50;
        int margin = 10;
        int startX = window.width - 50;
        int startY = 10;

        for (int i = 0; i < player->max_lives; i++) {
            if (i < player->current_lives) {
                ShowBitmap( startX - (i * (h_w + margin)), startY, h_w, h_w,6);

            }
            else {
                ShowBitmap( startX - (i * (h_w + margin)), startY, h_w, h_w, 7);

            }
        }
    }

};

health_bar Health_bar;