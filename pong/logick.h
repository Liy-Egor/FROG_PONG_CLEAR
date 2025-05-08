#pragma once
#include "Structures.h"

void InitGame()
{
    player = new player_(0.2, 0.25, 0.012, 0.021, "racket.bmp");
    player->racket.speed = 30;
    //-----------------------------location0_______________
    location[0].hBack.loadBitmapWithNativeSize("background_0.bmp");
    location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.09, 1, "racket.bmp");//портал в локацию 1
    location[0].walls.emplace_back(0, 0, 0.02, 0.99, "walls.bmp");//левая стена
    location[0].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls.bmp");//правая стена
    location[0].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls.bmp");//пол
    location[0].walls.emplace_back(0, 0, 0.999, 0.02, "walls.bmp");//потолок
    location[0].locationObjects.emplace_back(0.3, 0.98, 0.012, 0.021, "ball.bmp", "healing");
    location[0].locationObjects.emplace_back(0.5, 0.98, 0.012, 0.021, "spike.bmp", "spike");
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    //___________________________location1________________
    location[1].hBack.loadBitmapWithNativeSize("background_1.bmp");
    location[1].portal.emplace_back(0.02, 0.89, 0.021, 0.09, 0, "racket.bmp");//портал в локацию 0
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

float clickTimeOut = 100;
float clickTime = 0;
void ProcessInput()
{
    
    player->racket.dx = player->racket.x; // и Дельта для вектора движения если игрок стоит на месте
    if (GetAsyncKeyState(VK_LEFT)) player->racket.x -= player->racket.speed, player->racket.dx = player->racket.x - player->racket.speed; // и Дельта для вектора движения в следующем кадре по -x
    if (GetAsyncKeyState(VK_RIGHT)) player->racket.x += player->racket.speed, player->racket.dx = player->racket.x + player->racket.speed; // и Дельта для вектора движения в следующем кадре по +x
    if (GetAsyncKeyState(VK_SPACE) && player->inJump == false)
    {
        player->inJump = true;
        player->jump = 90;
    }
    clickTime = timeGetTime();

    

   
}

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}


void ProcessPortal()
{
    for (auto& i : location[player->currentLocation].portal)
    {
        i.spr.show();

        if (CheckCollision(player->racket.x, player->racket.y, player->racket.width, player->racket.height, i.spr.x, i.spr.y, i.spr.width, i.spr.height))
        {
            player->currentLocation = i.destination;
            if (i.destination % 2 == 0)
            {
                player->racket.x = 1500;
            }
            else player->racket.x = 100;
            
        }
    }
}

void ProcessHero() //jump
{
    player->racket.y += player->gravity - player->jump;
    player->racket.y = min(window.height - player->racket.height, player->racket.y);
    player->jump *= .9;
    player->racket.dy = player->racket.y + player->gravity - player->jump; // Дельта для вектора движения в следующем кадре по y
    player->jump = max(player->jump, 0);
}
HANDLE hTimer;
void TweenDash(int idx)
{
    WaitForSingleObject(hTimer, INFINITE);

    if (player->dash_allow == true)
        if (player->racket.vect_right == true)
        {
            player->racket.x += idx;
        }
        else
        {
            player->racket.x -= idx;
        }


}

void ProcessDash()
{
    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    if (isShiftPressed && !player->wasShiftPressed)
    {
        player->wasShiftPressed = true;

        hTimer = CreateWaitableTimer(NULL, false, NULL);
        LARGE_INTEGER dTime;
        for (int i = 0; i < player->dashDistance; i++)
        {

            thread t(TweenDash, i);
            t.detach();

        }
        dTime.QuadPart = -10000 * 7;
        SetWaitableTimer(hTimer, &dTime, 7, NULL, NULL, false);

    }
    else if (!isShiftPressed)
    {
        player->wasShiftPressed = false;
        player->dash_allow = true;
    }


}

void Collision(sprite racket_obj, sprite wall_obj)
{

    int length = sqrt(pow(racket_obj.dx - racket_obj.x, 2) + pow(racket_obj.dy - racket_obj.y, 2)); //длина по теореме пифагора

    for (int i = 0; i < length; i++)
    {
        //на высокой скорости не работает
        float Pixel_x = i * (racket_obj.dx - racket_obj.x) / length + (racket_obj.x + racket_obj.width / 2);  //отрисовка точек отрисовки по X  "+ racket_obj.width / 2" <- это чтобы луч отрисовывался с середины персонажа
        float Pixel_y = i * (racket_obj.dy - racket_obj.y) / length + (racket_obj.y);  //отрисовка точек отрисовки по Y

        SetPixel(window.context, Pixel_x, Pixel_y, RGB(255, 255, 255)); // визуализация отрисовки луча

        if (Pixel_x > wall_obj.x &&
            Pixel_x < (wall_obj.x + wall_obj.width) &&
            Pixel_y > wall_obj.y &&
            Pixel_y < (wall_obj.y + wall_obj.height)
            )
        {
            //сравнение сторон перса со стеной (но мб надо сравнивать точку трасировки со стенами)
            int top = racket_obj.y - wall_obj.y;
            int down = wall_obj.y + wall_obj.height - racket_obj.y;
            int left = (racket_obj.x + racket_obj.width / 2) - wall_obj.x;
            int right = wall_obj.x + wall_obj.width - (racket_obj.x + racket_obj.width / 2);

            int minX = min(left, right);
            int minY = min(top, down);

            if (minX < minY)
            {
                if (left < right)
                {
                    player->racket.x = wall_obj.x - racket_obj.width; // уснаовка перса слева
                }
                else
                {
                    player->racket.x = wall_obj.x + wall_obj.width + racket_obj.width / 4;  // уснаовка перса справа
                }
                player->dash_allow = false; // не дает делать рывок (нужно перенести переменную в структуру)
            }
            else
            {
                if (down < top)
                {
                    player->racket.y = wall_obj.y + wall_obj.height + racket_obj.height; // отскок перса от полотка
                    player->jump *= .4;
                }
                else
                {
                    player->racket.y = wall_obj.y - racket_obj.height; // уснаовка перса на платформе
                    player->inJump = false; // дает разрешение на прыжок (нужно перенести переменную в структуру)
                }
            }
        }
        else
        {
            if (racket_obj.y + racket_obj.height == window.height) // это если перс на нижней части окна экрана Windows
            {
                player->inJump = false;
            }
        }
    }

}


void CollisionGroup()
{
    for (int i = 0; i < location[player->currentLocation].locationTexture.size(); i++) {
        auto platform = location[player->currentLocation].locationTexture[i].Sprite;
        Collision(player->racket, platform);  // это на будущее, эту функцю можно будет дополнять условиями типа bool для случаев если нужно пройти свковзь стену или наоборот
    }
    for (int i = 0; i < location[player->currentLocation].walls.size(); i++) {
        auto walls = location[player->currentLocation].walls[i].Sprite;
        Collision(player->racket, walls);
    }



    static int lastDamageTime = 0;
    bool spikeCollision = false;
    for (int i = 0; i < location[player->currentLocation].locationObjects.size(); ++i)
    {
        Objects& obj = location[player->currentLocation].locationObjects[i];
        if (obj.type == "spike")
        {
            if (player->racket.x + player->racket.width >= obj.Sprite.x &&
                player->racket.x <= obj.Sprite.x + obj.Sprite.width &&
                player->racket.y + player->racket.height >= obj.Sprite.y &&
                player->racket.y <= obj.Sprite.y + obj.Sprite.height)
            {

                spikeCollision = true;
                break;
            }
        }
        if (obj.type == "healing")
        {
            if (player->racket.x + player->racket.width >= obj.Sprite.x &&
                player->racket.x <= obj.Sprite.x + obj.Sprite.width &&
                player->racket.y + player->racket.height >= obj.Sprite.y &&
                player->racket.y <= obj.Sprite.y + obj.Sprite.height)
            {
                location[player->currentLocation].locationObjects.erase(location[player->currentLocation].locationObjects.begin());
                health.current_lives++;
                break;
            }
        }

    }
    if (spikeCollision && currenttime > lastDamageTime + 1000) {
        health.current_lives--;
        lastDamageTime = currenttime;
    }
    if (health.current_lives <= 0) {
        MessageBox(window.hWnd, "Game Over!", "Info", MB_OK);
        exit(0);
    }
}



