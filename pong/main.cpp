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
    HBITMAP hBitmap;//хэндл к спрайту шарика 

    void loadBitmapWithNativeSize(const char* filename)
    {
        hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        
    }

    void show()
    {
        ShowBitmap(window.context, x* window.width, y * window.height, width * window.width, height * window.height, hBitmap, false);
    }
};
sprite racket;//ракетка игрока
sprite healing;

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

struct enemy {

    sprite enemy_sprite;
    std::vector<sprite> bullet;
    int bullettime = 0;
    bool dead = true;
    int spawnTime = 0;
    int HPfrog = 3;           //rand()% 3-5

    void showBullet()
    {
        for (int i = 0; i < bullet.size(); i++)
        {
            ShowBitmap(window.context, bullet[i].x - bullet[i].rad, bullet[i].y - bullet[i].rad, 2 * bullet[i].rad, 2 * bullet[i].rad, bullet[i].hBitmap, true);
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
                b.dx = racket.x - b.x;
                b.dy = racket.y - b.y;

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

struct Objects{
   
    sprite textureSprite;

    Objects(float p_x, float p_y, float p_width, float p_height, const char* filename) {
        this->textureSprite.x = p_x;
        this->textureSprite.y = p_y;
        this->textureSprite.width = p_width;
        this->textureSprite.height = p_height;
        this->textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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



struct player_
{

    HBITMAP hHealthFull, hHealthEmpty;
    int health_width = 40;
    int max_lives = 5;
    int current_lives = 3;
};

player_ player;


void InitGame()
{
    player.hHealthFull = (HBITMAP)LoadImageA(NULL, "health_full.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.hHealthEmpty = (HBITMAP)LoadImageA(NULL, "health_empty.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    healing.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //------------------------------------------------------
    racket.width = 30;
    racket.height = 30;
    racket.speed = 30;
    racket.x = window.width / 2.;
    racket.y = window.height - racket.height;

    loadFrog();
    loadBitmap("ball.bmp", ballBitmap);
    

    location[0].hBack = (HBITMAP)LoadImageA(NULL, "background_0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[0].RightPort = 1;
    location[0].LeftPort = -1;

    healing.width = 40;
    healing.height = 40;
    healing.x = 700;
    healing.y = 400;
    location[0].locationObjects.push_back(healing);

    
    location[0].locationTexture.emplace_back(0.8, 0.85, 0.15, 0.05,"racket_enemy.bmp");
    //platform1.textureSprite.loadBitmapWithNativeSize("racket_enemy.bmp");
    //location[0].locationTexture.push_back(platform1);


    location[1].hBack = (HBITMAP)LoadImageA(NULL, "background_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[1].RightPort = -1;
    location[1].LeftPort = 0;

    location[1].locationTexture.emplace_back(0.08, 0.9, 0.15, 0.05, "racket_enemy.bmp");
   // platform2.textureSprite.loadBitmapWithNativeSize("racket_enemy.bmp");
    //location[1].locationTexture.push_back(platform2);
}

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

float clickTimeOut = 100;
float clickTime = 0;

float gravity = 30;

float jump = 0;

float maxjump = 10;

void ProcessHero()
{


    if (GetAsyncKeyState(VK_SPACE) && racket.y > (window.height - racket.height - 1))
        if (GetAsyncKeyState(VK_SPACE) && (racket.y > (window.height - racket.height - 1)))
        {

            jump += 90;
        }

    racket.y += gravity - jump;
    racket.y = min(window.height - racket.height, racket.y);

    //inJump = true;
    jump *= .9;
    //jump = max(jump, 0);
    jump = max(jump, 0);

}
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
        b.hBitmap = ballBitmap;

        game.action = true;

        bullet.push_back(b);

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
    for (int i = 0; i < location[currentLocation].locationTexture.size(); i++) {

        auto platform = location[currentLocation].locationTexture[i].textureSprite;

        if ((racket.x >= location[currentLocation].locationTexture[i].textureSprite.x * window.width &&
            racket.x <= location[currentLocation].locationTexture[i].textureSprite.x * window.width + location[currentLocation].locationTexture[i].textureSprite.width) ||
            (racket.y <= location[currentLocation].locationTexture[i].textureSprite.y * window.height + location[currentLocation].locationTexture[i].textureSprite.height &&
                racket.y + racket.height >= location[currentLocation].locationTexture[i].textureSprite.y * window.height)) {
            if ((racket.x + racket.width >= platform.x * window.width &&
                racket.x <= platform.x * window.width + platform.width * window.width) &&
                (racket.y <= platform.y * window.height + platform.height * window.height &&
                    racket.y + racket.height >= platform.y * window.height)) {

                //racket.y = min(platform.y, racket.y - racket.height);
                racket.y = location[currentLocation].locationTexture[i].textureSprite.y * window.height + location[currentLocation].locationTexture[i].textureSprite.height;

                racket.y = min(platform.y * window.height - racket.height, window.height - racket.height);
                jump *= .9;
                jump = max(jump, 0);
                //inJump = false;
            }
            else {
                racket.y = min(window.height - racket.height, racket.y);
            }
            //racket.y -=gravity;

        }
    }
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
                bullet[i].x = racket.x;
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
        DrawHealth();
        ProcessInput();//опрос клавиатуры
        ProcessDash();//рывок
        ProcessHero();//прыжок 
        ProcessBall();
        //spawnEnemy();
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        
        
        Collision();
        LimitRacket();//проверяем, чтобы ракетка не убежала за экран
       
    }

}