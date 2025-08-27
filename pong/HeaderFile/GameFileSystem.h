#pragma once
#include "ArcheType.h"

string StrReplace(string* str, string namestr) {
    return str->replace(str->find(namestr), namestr.length(), "");
}

void LoadSVGDataMap(const string NameFileSVG) {
   
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
                    if (!str.find("walls_f") || !str.find("Back_lvl") || !str.find("image")) {
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
    int IdLocation;
    for (int i = 0; i < dS[0].size(); i++) {
        //интрепритация стоковых данных в числовые значения типа 0.1 от разрешения экрана пользователя
        float x = stof(dS[1][i]) / window.width;
        float y = stof(dS[2][i]) / window.height;
        float width = stof(bufferData[0][i]) / window.width;
        float height = stof(bufferData[1][i]) / window.height;
        string nameObject = dS[0][i];
        float arr[4]{x, y, width, height};
        
        //создание объектов
        if (!nameObject.find("Centr_Forest_Place")) {
            Wall = new ATWall(PLATF"Centr_Forest_Place", arr);
            Wall->SetLocation(IdLocation);
            VLocation[IdLocation].VWall.push_back(*Wall);
        }
		else if (!nameObject.find("CentrL_Forest_Place")) {
			Wall = new ATWall(PLATF"CentrL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("CentrR_Forest_Place")) {
			Wall = new ATWall(PLATF"CentrR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownL_Forest_Place")) {
			Wall = new ATWall(PLATF"DownL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownM_Forest_Place")) {
			Wall = new ATWall(PLATF"DownM_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownR_Forest_Place")) {
			Wall = new ATWall(PLATF"DownR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopL_Forest_Place")) {
			Wall = new ATWall(PLATF"TopL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopM_Forest_Place")) {
			Wall = new ATWall(PLATF"TopM_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopR_Forest_Place")) {
			Wall = new ATWall(PLATF"TopR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
        else if (!nameObject.find("Enemy_static_Test")) {
            Enemy = new ATEnemy(ENEMY"Enemy_static_Test", arr);
            Enemy->SetLocation(IdLocation);
            VLocation[IdLocation].VEnemy.push_back(*Enemy);
        }
        else if (!nameObject.find("Player_static_Test")) {
            if (PlayerPlay == false)
            {
                PlayerPlay = true;
                Player = new ATPlayer(PLAYER"Player_static_Test", arr);
                Player->SetLocation(IdLocation);
            }
        }
        else if (!nameObject.find("Back_lvl")) {
            Location = new ATLocation(BACKG + nameObject, arr);
            IdLocation = VLocation.size();
            VLocation.push_back(*Location);
        }

        //else if (!nameObject.find("portal")) {
        //    StrReplace(&nameObject, "portal");
        //    Portal = new ATPortal("racket", arr, stoi(nameObject));
        //    Portal->SetLocation(IdLocation);
        //    VLocation[IdLocation].VPortal.push_back(*Portal);
        //}
        //else if (!nameObject.find("heal")) {
        //    HealFlack = new ATHealFlack("ball", arr);
        //    HealFlack->SetLocation(IdLocation);
        //    VLocation[IdLocation].VHealFlack.push_back(*HealFlack);
        //}
        //else if (!nameObject.find("spike")) {
        //    Spike = new ATSpike("spike", arr);
        //    Spike->SetLocation(IdLocation);
        //    VLocation[IdLocation].VSpike.push_back(*Spike);
        //}

    }
    file.close();
}
