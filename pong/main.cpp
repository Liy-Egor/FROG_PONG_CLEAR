//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
using namespace std;
int currenttime = 0;
void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

POINT mouse;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

HBITMAP hBack;// хэндл для фонового изображения
// секция данных игры  

struct sprite {
    float x, y, width, height, rad, dx, dy, speed, time;
    HBITMAP hBitmap;//хэндл к спрайту шарика 

    void loadBitmapWithNativeSize(const char* filename)
    {
        hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        BITMAP bm;
        GetObject(hBitmap, (int)sizeof bm, &bm);
        width = bm.bmWidth;
        height = bm.bmHeight;
    }

    void show()
    {
        ShowBitmap(window.context, x* window.width, y * window.height, width * window.width, height * window.height, hBitmap, false);
    }
};
sprite racket;//ракетка игрока
sprite healing;



struct Objects{
   
    sprite textureSprite;

    Objects(float p_x, float p_y, float p_width, float p_height) {
        this->textureSprite.x = p_x;
        this->textureSprite.y = p_y;
        textureSprite.width = p_width;
        textureSprite.height = p_height;
        //location[loc].locationTexture.push_back(this);
    }

};

int currentLocation = 0;
struct Location_ {
    HBITMAP hBack;
    int LeftPort;
    int RightPort;
    vector<Objects> locationTexture;
    vector<sprite> locationObjects;
    
};
Location_ location[5];

sprite platform;

struct player_
{

    HBITMAP hHealthFull, hHealthEmpty;
    int health_width = 40;
    int max_lives = 5;
    int current_lives = 3;
};

player_ player;

void loadBitmap(const char* filename, HBITMAP& hbm)
{
    hbm = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}



void InitGame()
{
    player.hHealthFull = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.hHealthEmpty = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    healing.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //------------------------------------------------------
    racket.width = 30;
    racket.height = 30;
    racket.speed = 30;
    racket.x = window.width / 2.;
    racket.y = window.height - racket.height;


    location[0].hBack = (HBITMAP)LoadImageA(NULL, "background_0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[0].RightPort = 1;
    location[0].LeftPort = -1;

    healing.width = 40;
    healing.height = 40;
    healing.x = 700;
    healing.y = 400;
    location[0].locationObjects.push_back(healing);

    
   // platform1.textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, "racket_enemy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Objects platform1(0.8, 0.85, 0.15, 0.05);
    platform1.textureSprite.loadBitmapWithNativeSize("racket_enemy.bmp");
    /*platform1.textureSprite.x =  0.8;
    platform1.textureSprite.y =  0.85;
    platform1.textureSprite.width = 0.15;
    platform1.textureSprite.height = 0.05;*/

    location[0].locationTexture.push_back(platform1);


    location[1].hBack = (HBITMAP)LoadImageA(NULL, "background_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[1].RightPort = -1;
    location[1].LeftPort = 0;

    Objects platform2(0.08, 0.9, 0.15, 0.05);
   
   // platform2.textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, "racket_enemy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    platform2.textureSprite.loadBitmapWithNativeSize("racket_enemy.bmp");
    /*platform2.textureSprite.x =  0.08;
    platform2.textureSprite.y =  0.9;
    platform2.textureSprite.width = 0.15;
    platform2.textureSprite.height = 0.05;*/

    location[1].locationTexture.push_back(platform2);
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

float clickTimeOut = 100;
float clickTime = 0;

void ProcessInput()
{
    if (GetAsyncKeyState(VK_LEFT)) racket.x -= racket.speed;
    if (GetAsyncKeyState(VK_RIGHT)) racket.x += racket.speed;
    clickTime = timeGetTime();

    if (GetAsyncKeyState(VK_LBUTTON) && clickTime > clickTimeOut)
    {
        sprite b;
        b.speed = 10;
        b.x = racket.x;
        b.y = racket.y;

        b.dx = mouse.x - b.x;
        b.dy = mouse.y - b.y;
        float dvector = sqrt(b.dx * b.dx + b.dy * b.dy);
        b.dx = b.dx / dvector;
        b.dy = b.dy / dvector;
        b.rad = 20;
        

       

        ProcessSound("bounce1.wav");

        clickTimeOut = clickTime + 1000;


    }

    if (racket.x < 0)
    {
        if (location[currentLocation].LeftPort >= 0)
        {
            racket.x = window.width - racket.width;
            currentLocation = location[currentLocation].LeftPort;
        }
        else
        {
            racket.x = 0;
        }
    }

    if (racket.x > window.width - racket.width)
    {
        if (location[currentLocation].RightPort >= 0)
        {
            racket.x = 0;
            currentLocation = location[currentLocation].RightPort;
        }
        else
        {
            racket.x = window.width - racket.width;
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
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);// ракетка игрока


}

void DrawHealth() {
    int margin = 10;
    int startX = window.width - 50;
    int startY = 10;

    for (int i = 0; i < player.max_lives; i++) {

        HBITMAP currentHealth = (i < player.current_lives) ? player.hHealthFull : player.hHealthEmpty;


        ShowBitmap(
            window.context,
            startX - (i * (player.health_width + margin)),
            startY,
            player.health_width,
            player.health_width,
            currentHealth
        );
    }
}

void ShowTexture()
{
    /*for (const auto& i : location[currentLocation].locationTexture)
    {
        ShowBitmap(window.context, i.textureSprite.x, i.textureSprite.y, i.textureSprite.width, i.textureSprite.height, i.textureSprite.hBitmap);
    }*/

    for (int i = 0; i < location[currentLocation].locationTexture.size();i++) {
        location[currentLocation].locationTexture[i].textureSprite.show();
    }
}

void ShowObjects()
{
    for (const auto& i : location[currentLocation].locationObjects)
    {

        ShowBitmap(window.context, i.x, i.y, i.width, i.height, i.hBitmap);


    }
}



void Collision()
{
    

    //if (racket.x >= location[currentLocation].locationTexture[currentLocation].textureSprite.x && 
    //    racket.x <= location[currentLocation].locationTexture[currentLocation].textureSprite.x + platform.width && 
    //    racket.y + racket.height <= location[currentLocation].locationTexture[currentLocation].textureSprite.y + platform.height &&
    //    racket.y + racket.height >= location[currentLocation].locationTexture[currentLocation].textureSprite.y)
    //{
    //    //racket.y = min(platform.y, racket.y - racket.height);
    //    racket.y = racket.y - racket.height;
    //}

    
}




void LimitRacket()
{
    racket.x = max(racket.x, racket.width / 2.);//если коодината левого угла ракетки меньше нуля, присвоим ей ноль
    racket.x = min(racket.x, window.width - racket.width / 2.);//аналогично для правого угла
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

float gravity = 30;

float jump = 0;
float maxjump = 20;


void ProcessHero()
{


    if (GetAsyncKeyState(VK_SPACE) && racket.y > (window.height - racket.height - 1))
    {

        jump += 90;
    }

    racket.y += gravity - jump;
    racket.y = min(window.height - racket.height, racket.y);

    jump *= .9;
    //jump = max(jump, 0);

}

const float dashDistance = 150;
float dash = 0;
bool wasShiftPressed = false;

void ProcessDash() //рывок
{
    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    if (isShiftPressed && !wasShiftPressed)
    {
        wasShiftPressed = true;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            racket.x -= dashDistance;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            racket.x += dashDistance;
        }
    }
    else if (!isShiftPressed)
    {
        wasShiftPressed = false;
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();
        
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        ShowRacketAndBall();//рисуем фон, ракетку и шарик
        ShowTexture();
        DrawHealth();
        ProcessInput();//опрос клавиатуры
        ProcessDash();//рывок
        ProcessHero();//прыжок 
        Collision();
        LimitRacket();//проверяем, чтобы ракетка не убежала за экран
       
        // ProcessRoom();//обрабатываем отскоки от стен и каретки, попадание шарика в картетку


    }

}