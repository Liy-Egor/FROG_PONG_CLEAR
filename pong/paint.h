#include "Boss.h"

void loadBitmap(const char* filename, HBITMAP& hbm)
{
    hbm = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // —оздаем контекст пам€ти, совместимый с контекстом отображени€
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// ¬ыбираем изображение bitmap в контекст пам€ти

    if (hOldbm) // ≈сли не было ошибок, продолжаем работу
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // ќпредел€ем размеры изображени€

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//все пиксели черного цвета будут интепретированы как прозрачные
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // –исуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// ¬осстанавливаем контекст пам€ти
    }

    DeleteDC(hMemDC); // ”дал€ем контекст пам€ти
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
        ShowBitmap(window.context, bullet[i].x - bullet[i].rad, bullet[i].y - bullet[i].rad, 2 * bullet[i].rad, 2 * bullet[i].rad, bullet[i].hBitmap, true);// ?????
    }


}