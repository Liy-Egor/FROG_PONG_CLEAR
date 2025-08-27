#pragma once
#include "ArcheType.h"

string StrReplace(string* str, string namestr) {
    return str->replace(str->find(namestr), namestr.length(), "");
}

//создание карт можно делать в редакторе Adobe Photoshop любой версии
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
            Wall = new ATWall("Centr_Forest_Place", arr);
            Wall->SetLocation(IdLocation);
            VLocation[IdLocation].VWall.push_back(*Wall);
        }
		else if (!nameObject.find("CentrL_Forest_Place")) {
			Wall = new ATWall("CentrL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("CentrR_Forest_Place")) {
			Wall = new ATWall("CentrR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownL_Forest_Place")) {
			Wall = new ATWall("DownL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownM_Forest_Place")) {
			Wall = new ATWall("DownM_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("DownR_Forest_Place")) {
			Wall = new ATWall("DownR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopL_Forest_Place")) {
			Wall = new ATWall("TopL_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopM_Forest_Place")) {
			Wall = new ATWall("TopM_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
		else if (!nameObject.find("TopR_Forest_Place")) {
			Wall = new ATWall("TopR_Forest_Place", arr);
			Wall->SetLocation(IdLocation);
			VLocation[IdLocation].VWall.push_back(*Wall);
		}
        else if (!nameObject.find("Enemy_static_Test")) {
            Enemy = new ATEnemy("Enemy_static_Test", arr);
            Enemy->SetLocation(IdLocation);
            VLocation[IdLocation].VEnemy.push_back(*Enemy);
        }
        else if (!nameObject.find("Player_static_Test")) {      
            if (PlayerPlay == false)
            {
                PlayerPlay = true;
                Player = new ATPlayer("Player_static_Test", arr);
                Player->SetLocation(IdLocation);
            }
        }
        else if (!nameObject.find("Back_lvl")) {
            Location = new ATLocation(nameObject, arr);
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

//class Serialization {
//public:
//    Serialization(Location_ container) { this->container = container;}
//    
//
//    /// добавить функцию создания сохранения с нуля и загрузка мира с нулевого сохранения
//    /// добавить функцию проверки на наличие изменения данных в файле чтобы создать мир с нуля и перезаписать данные на новые
//
//    void Ser() {
//        ofile.open("SaveData.txt", ios::trunc);
//        ofile.close();
//        ofile.open("SaveData.txt", ofstream::app);
//        if (ofile.is_open())
//        {
//            ///hardcode?!?!?!?!?!?
//            for (int i = 0; i < container.healingFlask.size(); i++) {
//                ofile << "<healingFlask>" << "index=" << i << " ";
//                SortSprite(container.healingFlask[i].Sprite);
//            }
//            for (int i = 0; i < container.spike.size(); i++) {
//                ofile << "<spike>" << "index=" << i << " ";
//                SortSprite(container.spike[i].Sprite);
//            }
//            for (int i = 0; i < container.Persona.size(); i++) {
//                ofile << "<Persona>" << "index=" << i << " ";
//                SortSprite(container.Persona[i]->Sprite);
//            }
//            for (int i = 0; i < container.portal.size(); i++) {
//                ofile << "<portal>" << "index=" << i << " ";
//                SortSprite(container.portal[i].Sprite);
//            }
//            for (int i = 0; i < container.walls.size(); i++) {
//                ofile << "<walls>" << "index=" << i << " ";
//                SortSprite(container.walls[i].Sprite);
//                
//            }
//        }
//        ofile.close();
//    }
//    void Deser() {
//        ifile.open("SaveData.txt");
//        if (ifile.is_open())
//        {
//            string StructureArr[]{"<healingFlask>", "<spike>", "<Persona>", "<portal>", "<walls>", "height", "width", "xpos", "ypos"};
//            string str;
//            while (!ifile.eof()) {
//                ifile >> str;
//                for (int i = 0; i < 9; i++) {
//                        ///определение объекта по его индексу и по экземляру стуктуры
//                        if (!str.find(StructureArr[i]) &&
//                            !str.find(StructureArr[i] + "index=")) {
//                            ReplaceStructure(str, StructureArr[i]);
//                            break;
//                        }
//                        else if (!str.find(StructureArr[i]) &&
//                             !str.find(StructureArr[i] + "==")) {
//                             ReplaceProperty(str);
//                             break;
//                             }
//                }
//            }
//        }
//        ifile.close();
//    }
//private:
//    string MarkerStructure;
//    int IndexStructure;
//    Location_ container;
//    ifstream ifile;
//    ofstream ofile;
//
//    void SortSprite(sprite SprData) {
//        ofile << "height==" << SprData.height << " "
//              << "width==" << SprData.width << " "
//              << "xpos==" << SprData.x << " "
//              << "ypos==" << SprData.y << "\n";
//    }
//    void ReplaceStructure(string str, string StructureArr) {
//        StrReplace(&str, StructureArr + "index=");
//        MarkerStructure = StructureArr;
//        IndexStructure = stoi(str);
//        
//    };
//    void ReplaceProperty(string StructureArr) {
//        float height = NULL;
//        float width = NULL;
//        float xpos = NULL;
//        float ypos = NULL;
//
//        if (!StructureArr.find("height")) {
//            StrReplace(&StructureArr, "height==");
//            height = stof(StructureArr);
//        }
//        else if (!StructureArr.find("width")) {
//            StrReplace(&StructureArr, "width==");
//            width = stof(StructureArr);
//        }
//        else if (!StructureArr.find("xpos")) {
//            StrReplace(&StructureArr, "xpos==");
//            xpos = stof(StructureArr);
//        }
//        else if (!StructureArr.find("ypos")) {
//            StrReplace(&StructureArr, "ypos==");
//            ypos = stof(StructureArr);
//        }
//
//        if (MarkerStructure == "<healingFlask>") {
//            if (container.healingFlask.size() != 0)
//            {
//            SwapProperty(&container.healingFlask[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<spike>") {
//            if (container.spike.size()!=0)
//            {
//            SwapProperty(&container.spike[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<Persona>") {
//            if (container.Persona.size() != 0)
//            {
//            SwapProperty(&container.Persona[IndexStructure]->Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<portal>") {
//            if (container.portal.size() != 0)
//            {
//            SwapProperty(&container.portal[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//        else if (MarkerStructure == "<walls>") {
//            if (container.walls.size() != 0)
//            {
//            SwapProperty(&location[0].walls[IndexStructure].Sprite, height, width, xpos, ypos);
//            }
//        }
//
//        height = NULL;
//        width = NULL;
//        xpos = NULL;
//        ypos = NULL;
//    };
//    void SwapProperty(sprite* Struct, float h, float w, float x, float y) {
//        if (h != NULL) { Struct->height = h; }
//        if (w != NULL) { Struct->width = w; }
//        if (x != NULL) { Struct->x = x; }
//        if (y != NULL) { Struct->y = y; }
//    };
//
//};

/* Serialization DataGame(location[0]);
        if (GetAsyncKeyState(VK_F1)) {
            DataGame.Ser();
        }
        if (GetAsyncKeyState(VK_F2)) {
            DataGame.Deser();
        }*/

