#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>
#include "vector"
#include <fstream>


using namespace std;

int currenttime = 0;
POINT mouse;

struct 
{
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

struct 
{
    int x = 0;
    int y = 0;
    int w;
    int h;
} player_view;

int MapSizeW = window.width;
int MapSizeH = window.height;

bool PlayerPlay = false;