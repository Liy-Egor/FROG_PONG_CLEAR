//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
using namespace std;
int currenttime = 0;


void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);
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
        ShowBitmap(window.context, x, y , width , height , hBitmap, false);
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


void loadBitmap(const char* filename, HBITMAP& hbm)
{
    hbm = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

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
        this->hHealthFull= (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->hHealthEmpty = (HBITMAP)LoadImageA(NULL, filename2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }

    player_(float p_x, float p_y, float p_width, float p_height, const char* filename)
    {
        this->racket.x = p_x * window.width;
        this->racket.y = p_y * window.height;
        this->racket.width = p_width * window.width ;
        this->racket.height = p_height * window.height;
        this->racket.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
    
};
player_ *player;

player_ health{40, 5, 3, "health_full.bmp", "health_empty.bmp"};

int currentLocation = 0;
struct Location_ {
    HBITMAP hBack;
    int LeftPort;
    int RightPort;
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

void InitGame()
{
    player = new player_(0.2, 0.25, 0.012, 0.021, "racket.bmp");
    player->racket.speed = 30;
    //-------------------------------------------------------
    loadFrog();
    loadBitmap("ball.bmp", ballBitmap);
    //-----------------------------location0_______________
    loadBitmap("background_0.bmp", location[0].hBack);
    location[0].RightPort = 1;
    location[0].LeftPort = -1;
    location[0].locationObjects.emplace_back(0.5, 0.5, 0.012, 0.021, "ball.bmp", "healing");
    location[0].locationObjects.emplace_back(0.5, 0.98, 0.012, 0.021, "spike.bmp", "spike");
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.4, 0.85, 0.15, 0.05, "racket_enemy.bmp");
    location[0].locationTexture.emplace_back(0.6, 0.95, 0.15, 0.05, "racket_enemy.bmp");
    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls.bmp");
    //___________________________location1________________
    loadBitmap("background_1.bmp", location[1].hBack);
    location[1].RightPort = -1;
    location[1].LeftPort = 0;
    location[1].locationTexture.emplace_back(0.08, 0.9, 0.15, 0.05, "racket_enemy.bmp");//платформа 2
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

float clickTimeOut = 100;
float clickTime = 0;



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

    
    if (player->racket.x < 0)
    {
        if (location[currentLocation].LeftPort >= 0)
        {
            player->racket.x = window.width - player->racket.width;
            currentLocation = location[currentLocation].LeftPort;
        }
        else
        {
            player->racket.x = 0;
        }
    }

    if (player->racket.x > window.width - player->racket.width)
    {
        if (location[currentLocation].RightPort >= 0)
        {
            player->racket.x = 0;
            currentLocation = location[currentLocation].RightPort;
        }
        else
        {
            player->racket.x = window.width - player->racket.width;
        }
    }
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // Создаем контекст памяти, совместимый с контекстом отображения
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// Выбираем изображение bitmap в контекст памяти

    if (hOldbm) // Если не было ошибок, продолжаем работу
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // Определяем размеры изображения

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//все пиксели черного цвета будут интепретированы как прозрачные
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // Рисуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// Восстанавливаем контекст памяти
    }

    DeleteDC(hMemDC); // Удаляем контекст памяти
}





void ShowRacketAndBall()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, location[currentLocation].hBack);//задний фон
    player->racket.show();

    for (int i = 0; i < slots_count; i++)
    {
        if (!frog[i].dead)
        {
            //frog[i].enemy_sprite.show();
            frog[i].processBullet();
            frog[i].showBullet();
        }

    }


    for (int i = 0; i < bullet.size(); i++)
    {
        ShowBitmap(window.context, bullet[i].x - bullet[i].rad, bullet[i].y - bullet[i].rad, 2 * bullet[i].rad, 2 * bullet[i].rad, bullet[i].hBitmap, true);// øàðèê
    }


}

void DrawHealth() {
    int margin = 10;
    int startX = window.width - 50;
    int startY = 10;

    for (int i = 0; i < health.max_lives; i++) {

        HBITMAP currentHealth = (i < health.current_lives) ? health.hHealthFull : health.hHealthEmpty;


        ShowBitmap(
            window.context,
            startX - (i * (health.health_width + margin)),
            startY,
            health.health_width,
            health.health_width,
            currentHealth
        );
    }
}

void ShowTexture()
{
   
    for (int i = 0; i < location[currentLocation].locationTexture.size();i++) {
        location[currentLocation].locationTexture[i].textureSprite.show();
    }
    for (int i = 0; i < location[currentLocation].walls.size();i++) {
        location[currentLocation].walls[i].textureSprite.show();
    }
}

void ShowObjects()
{
    for (int i = 0; i < location[currentLocation].locationObjects.size();i++) {
        location[currentLocation].locationObjects[i].objectsSprite.show();
    }
}

float gravity = 30;
float jump = 0;
float maxjump = 10;
bool inJump = false;


void ProcessHero() //jump
{
    if (GetAsyncKeyState(VK_SPACE) && inJump == false)
    {
        inJump = true; 
        jump += 90;
    }

    player->racket.y += gravity - jump;
    player->racket.y = min(window.height - player->racket.height, player->racket.y);

    jump *= .9;
    jump = max(jump, 0);

}



const int dashDistance = 20;
bool wasShiftPressed = false;
HANDLE hTimer;
bool dash_allow = true;

void TweenDash(int idx)
{
    WaitForSingleObject(hTimer, INFINITE);

    if (dash_allow == true)
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
    if (isShiftPressed && !wasShiftPressed)
    {
        wasShiftPressed = true;

        hTimer = CreateWaitableTimer(NULL, false, NULL);
        LARGE_INTEGER dTime;
        for (int i = 0; i < dashDistance; i++)
        {

            thread t(TweenDash, i); 
            t.detach();

        }
        dTime.QuadPart = -10000 * 7;
        SetWaitableTimer(hTimer, &dTime, 7, NULL, NULL, false);

    }
    else if (!isShiftPressed)
    {
        wasShiftPressed = false;
        dash_allow = true;
    }


}


void Collision(sprite racket_obj, sprite wall_obj)
{

    
    int centr_x = racket_obj.x;
    int centr_y = racket_obj.y + racket_obj.height / 2;

    float r_dx = centr_x - (centr_x + player->racket.width / 2);
    float r_dy = centr_y - centr_y - (player->racket.height / 2);
    int R = sqrt(pow(r_dx, 2) + pow(r_dy, 2));


    for (int i = 0; i <= 360; i++)
    {
       
        int Pixel_x = R * cos(i) + centr_x;
        int Pixel_y = R * sin(i) + centr_y;


       
        if (Pixel_x > wall_obj.x &&
            Pixel_x < (wall_obj.x + wall_obj.width) &&
            Pixel_y > wall_obj.y &&
            Pixel_y < (wall_obj.y + wall_obj.height)
            )
        {
           
            int top = centr_y - wall_obj.y;
            int down = (wall_obj.y + wall_obj.height) - centr_y;
            int left = centr_x - wall_obj.x;
            int right = (wall_obj.x + wall_obj.width) - centr_x;

            int minX = min(left, right);
            int minY = min(top, down);

            if (minX < minY)
            {
                if (left < right)
                {
                    player->racket.x = (wall_obj.x) - player->racket.width + 13;
                }
                else
                {

                    player->racket.x = ((wall_obj.x + wall_obj.width)) + player->racket.width - 13;
                }
                dash_allow = false;
            }
            else
            {
                if (down < top)
                {
                    player->racket.y = ((wall_obj.y + wall_obj.height)) + player->racket.height / 10;
                    jump *= .4;
                }
                else
                {
                    player->racket.y = wall_obj.y - player->racket.height; 
                    inJump = false;
                }
            }
        }
        else 
        {
            if (player->racket.y + player->racket.height == window.height) 
            {
                inJump = false;
            }
        }

    }

}


void CollisionGroup() 
{
    for (int i = 0; i < location[currentLocation].locationTexture.size(); i++) {
        auto platform = location[currentLocation].locationTexture[i].textureSprite;
        Collision(player->racket, platform); 
    }
    for (int i = 0; i < location[currentLocation].walls.size(); i++) {
        auto walls = location[currentLocation].walls[i].textureSprite;
        Collision(player->racket, walls);
    }


    static int lastDamageTime = 0;
    bool spikeCollision = false;
    for (int i = 0; i < location[currentLocation].locationObjects.size(); ++i)
    {
        Objects& obj = location[currentLocation].locationObjects[i];
        if (obj.type == "spike")
        {
            if (player->racket.x + player->racket.width >= obj.objectsSprite.x &&
                player->racket.x <= obj.objectsSprite.x + obj.objectsSprite.width &&
                player->racket.y + player->racket.height >= obj.objectsSprite.y &&
                player->racket.y <= obj.objectsSprite.y + obj.objectsSprite.height)
            {

                spikeCollision = true;
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






void LimitRacket()
{
    player->racket.x = max(player->racket.x, player->racket.width / 2.);//если коодината левого угла ракетки меньше нуля, присвоим ей ноль
    player->racket.x = min(player->racket.x, window.width - player->racket.width / 2.);//аналогично для правого угла
}


void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
    GetClientRect(window.hWnd, &r);

}

void ProcessBall()
{
    if (game.action)
    {

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

        for (int j = 0; j < slots_count; j++)
        {
            for (int i = 0; i < bullet.size(); i++)
            {
                if (!bullet.empty())
                {
                    if (!frog[j].dead)
                    {

                        if (bullet[i].x < frog[j].enemy_sprite.x + frog[j].enemy_sprite.width and
                            bullet[i].y < frog[j].enemy_sprite.y + frog[j].enemy_sprite.height and
                            bullet[i].x > frog[j].enemy_sprite.x and
                            bullet[i].y > frog[j].enemy_sprite.y)
                        {
                            bullet.erase(bullet.begin() + i);

                            frog[j].HPfrog -= 1;
                            if (frog[j].HPfrog < 0)
                            {
                                frog[j].dead = true;
                                frog[j].spawnTime = currenttime;
                            }

                            i--;
                            if (i < 0) break;

                        }
                    }
                }
                else
                {
                    break;
                }
            }

        }


        for (int i = 0; i < bullet.size(); i++)
        {
            if (bullet[i].speed < .1)
            {
                bullet.erase(bullet.begin() + i);
            }
        }
    }
    else
    {
        for (int i = 0; i < bullet.size(); i++)
        {
            if (bullet[i].speed < .1)
            {
                bullet[i].x = player->racket.x;
            }
        }
    }
    if (game.action)
    {
        //std::vector<int> b = (ball[i])
        {
            //    ball[i].time
        }
    }
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне

    InitGame();//здесь инициализируем переменные игры
    //ShowCursor(FALSE);


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();
        
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        int sz = 5;
        Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);

        ShowRacketAndBall();//рисуем фон, ракетку и шарик
        ShowTexture();
        ShowObjects();
        DrawHealth();
        ProcessInput();//опрос клавиатуры
        ProcessDash();//рывок
        ProcessHero();//прыжок 
        ProcessBall();
        //spawnEnemy();
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        
        
        CollisionGroup();
        LimitRacket();//проверяем, чтобы ракетка не убежала за экран
       
    }

}