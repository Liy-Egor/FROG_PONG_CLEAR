#pragma once
#include "Structures.h"



void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);
    window.width = r.right - r.left;
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));
    GetClientRect(window.hWnd, &r);

}


void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC);
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);

    if (hOldbm)
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm);

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//??? ??????? ??????? ????? ????? ??????????????? ??? ??????????
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ?????? ??????????? bitmap
        }

        SelectObject(hMemDC, hOldbm);// ??????????????? ???????? ??????
    }

    DeleteDC(hMemDC); // ??????? ???????? ??????
}

//новая фича создание, читалка файлов формата Svg и преобразование ее данных в игровой уровень 
//создание карт можно делать в редакторе Adobe Photoshop (*2023 версия на которой я проверял)


string StrReplace(string* str, string namestr)
{
	return str->replace(str->find(namestr), namestr.length(), "");
}

void LoadSVGDataMap(const string NameFileSVG)
{
	ifstream file;

	vector <vector<string>> dS{
		{},
		{},
		{},
		{},
		{},
		{}
	}; // здесть хронятся основные данные имя объекта, x, y

	vector <vector<string>> bufferData{ {},{} }; // здесть хронятся width, height объекта

	string optionArr[]{ "id=", "x=", "y=" ,"width=", "height=","xlink:href=" };

	file.open(NameFileSVG + ".svg");


	//запуск чтения файла
	if (file.is_open()) {
		string str = "";
		while (!file.eof()) {
			file >> str;

			for (int i = 0; i < 6; i++) {
				if (!str.find(optionArr[i])) {
					StrReplace(&str, optionArr[i]);
					StrReplace(&str, "\"");
					StrReplace(&str, "\"");

					//исключения
					if (!str.find("walls_f") || !str.find("background") || !str.find("image")) {
						if (!str.find("walls_f")) {
							dS[i].push_back("empty");
							dS[5].push_back("error image");
						}

						dS[1].push_back("0");
						dS[2].push_back("0");
					}

					//сортировка данных
					if (str.find("walls_f")) {
						if (str.find("data:")) {
							dS[i].push_back(str);
						}
						else {
							dS[5].push_back("error image");
						}
					}
				}
			}
		}

		//присвоение и пересборка массива новыми данными ширины и высоты объектов для тех объектов где эти данные записаны в шаблон #image
		for (int i = 0; i < dS[0].size(); i++) {
			if (dS[5][i] != "error image") {
				for (int j = 0; j < dS[0].size(); j++) {
					if (dS[5][i] == "#" + dS[0][j] + "/>") {
						bufferData[0].push_back(dS[3][j]);
						bufferData[1].push_back(dS[4][j]);
					}
				}
			}
			else {
				bufferData[0].push_back("standardData");
				bufferData[1].push_back("standardData");
			}
		}
		int IDWidth = 0;
		int IDHeight = 0;
		for (int i = 0; i < bufferData[0].size(); i++) {
			if (bufferData[0][i] == "standardData") {
				bufferData[0][i] = dS[3][IDWidth++];
				bufferData[1][i] = dS[4][IDHeight++];
			}
		}
	}


	//загрузка уровня на основе собранных данных
	for (int i = 0; i < dS[0].size(); i++) {
		//интрепритация стоковых данных в числовые значения типа 0.1 от разрешения экрана пользователя
		float x = stof(dS[1][i]) / window.width;
		float y = stof(dS[2][i]) / window.height;
		float width = stof(bufferData[0][i]) / window.width;
		float height = stof(bufferData[1][i]) / window.height;
		string nameObject = dS[0][i];


		//создание объектов пока что только для уровня 0
		if (!nameObject.find("walls")) {
			location[0].walls.emplace_back(x, y, width, height, "walls");
		}
		else if (!nameObject.find("enemy")) {
			wolf = new Wolf(x, y, width, height, "walls", 40, 5, 3, 0);
		}
		else if (!nameObject.find("racket")) {
			player = new Hero(x, y, width, height, "racket", 40, 5, 3, 0);
		}
		else if (!nameObject.find("background")) {
			location[0].hBack.loadBitmapWithNativeSize(nameObject);
		}
		else if (!nameObject.find("portal")) {
			location[0].portal.emplace_back(x, y, width, height, 1, "racket"); // пока реализациия работает на переход на 1 уровень
		}
		else if (!nameObject.find("heal"))
		{
			location[0].healingFlask.emplace_back(0.3, 0.955, 0.02, 0.025, "ball");
		}
		else if (!nameObject.find("spike"))
		{
			location[0].spike.emplace_back(0.5, 0.955, 0.025, 0.025, "spike");
		}


	}
	file.close();
}