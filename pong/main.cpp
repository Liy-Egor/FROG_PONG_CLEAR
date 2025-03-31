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
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;

HBITMAP hBack;// ����� ��� �������� �����������
// ������ ������ ����  

struct sprite {
    float x, y, width, height, rad, dx, dy, speed, time;
    HBITMAP hBitmap;//����� � ������� ������ 

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
        ShowBitmap(window.context, x, y, width, height, hBitmap, true);
    }
};
sprite racket;//������� ������

enum class obj
{
    loc1,
    loc2
};

struct Objects{
    obj name;
    sprite textureSprite;

};

int currentLocation = 0;
struct Location_ {
    HBITMAP hBack;
    int LeftPort;
    int RightPort;
    vector<Objects> locationTexture;
    
};
Location_ location[5];

sprite platform;

//c����� ����

void loadBitmap(const char* filename, HBITMAP& hbm)
{
    hbm = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}



void InitGame()
{
    racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

    Objects platform1;
    platform1.name = obj::loc1;
    platform1.textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, "racket_enemy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    platform1.textureSprite.x = window.width * 0.8;
    platform1.textureSprite.y = window.height * 0.9;
    platform1.textureSprite.width = 350;
    platform1.textureSprite.height = 50;

    location[0].locationTexture.push_back(platform1);


    location[1].hBack = (HBITMAP)LoadImageA(NULL, "background_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    location[1].RightPort = -1;
    location[1].LeftPort = 0;

    Objects platform2;
    platform2.name = obj::loc2;
    platform2.textureSprite.hBitmap = (HBITMAP)LoadImageA(NULL, "racket_enemy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    platform2.textureSprite.x = window.width / 4;
    platform2.textureSprite.y = window.height * 0.9;
    platform2.textureSprite.width = 350;
    platform2.textureSprite.height = 50;

    location[1].locationTexture.push_back(platform2);
}

void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
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

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// �������� ����������� bitmap � �������� ������

    if (hOldbm) // ���� �� ���� ������, ���������� ������
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // ���������� ������� �����������

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//��� ������� ������� ����� ����� ��������������� ��� ����������
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteDC(hMemDC); // ������� �������� ������
}





void ShowRacketAndBall()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, location[currentLocation].hBack);//������ ���
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);// ������� ������


}

void ShowTexture()
{
    for (const auto& i : location[currentLocation].locationTexture)
    {
        ShowBitmap(window.context, i.textureSprite.x, i.textureSprite.y, i.textureSprite.width, i.textureSprite.height, i.textureSprite.hBitmap);
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
    racket.x = max(racket.x, racket.width / 2.);//���� ��������� ������ ���� ������� ������ ����, �������� �� ����
    racket.x = min(racket.x, window.width - racket.width / 2.);//���������� ��� ������� ����
}


void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
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

void ProcessDash() //�����
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

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����


    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        currenttime = timeGetTime();
        GetCursorPos(&mouse);
        ScreenToClient(window.hWnd, &mouse);
        ShowRacketAndBall();//������ ���, ������� � �����
        ShowTexture();
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
        Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
        ProcessInput();//����� ����������
        ProcessDash();//�����
        ProcessHero();//������ 
        Collision();
        LimitRacket();//���������, ����� ������� �� ������� �� �����
       
        // ProcessRoom();//������������ ������� �� ���� � �������, ��������� ������ � ��������


    }

}