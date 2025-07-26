#pragma once
#include "Structures.h"

string StrReplace(string* str, string namestr) {
    return str->replace(str->find(namestr), namestr.length(), "");
}

//����� ���� ��������, ������� ������ ������� Svg � �������������� �� ������ � ������� ������� 
//�������� ���� ����� ������ � ��������� Adobe Photoshop (*2023 ������ �� ������� � ��������)

void LoadSVGDataMap(const string NameFileSVG) {
    ifstream file;

    vector <vector<string>> dS{
        {},
        {},
        {},
        {},
        {},
        {}
    }; // ������ �������� �������� ������ ��� �������, x, y

    vector <vector<string>> bufferData{ {},{} }; // ������ �������� width, height �������

    string optionArr[]{ "id=", "x=", "y=" ,"width=", "height=","xlink:href=" };

    file.open(NameFileSVG + ".svg");


    //������ ������ �����
    if (file.is_open()) {
        string str = "";
        while (!file.eof()) {
            file >> str;

            for (int i = 0; i < 6; i++) {
                if (!str.find(optionArr[i])) {
                    StrReplace(&str, optionArr[i]);
                    StrReplace(&str, "\"");
                    StrReplace(&str, "\"");

                    //����������
                    if (!str.find("walls_f") || !str.find("background") || !str.find("image")) {
                        if (!str.find("walls_f")) {
                            dS[i].push_back("empty");
                            dS[5].push_back("error image");
                        }

                        dS[1].push_back("0");
                        dS[2].push_back("0");
                    }

                    //���������� ������
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

        //���������� � ���������� ������� ������ ������� ������ � ������ �������� ��� ��� �������� ��� ��� ������ �������� � ������ #image
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


    //�������� ������ �� ������ ��������� ������
    for (int i = 0; i < dS[0].size(); i++) {
        //������������� �������� ������ � �������� �������� ���� 0.1 �� ���������� ������ ������������
        float x = stof(dS[1][i]) / window.width;
        float y = stof(dS[2][i]) / window.height;
        float width = stof(bufferData[0][i]) / window.width;
        float height = stof(bufferData[1][i]) / window.height;
        string nameObject = dS[0][i];


        //�������� �������� ���� ��� ������ ��� ������ 0
        if (!nameObject.find("walls")) {
            location[0].walls.emplace_back(x, y, width, height, "walls");
        }
        else if (!nameObject.find("enemy")) {
            wolf = new Wolf(x, y, width, height, "enemy1", 40, 5, 3, 0);
        }
        else if (!nameObject.find("racket")) {
            player = new Hero(x, y, width, height, "racket", 40, 5, 3, 0);
        }
        else if (!nameObject.find("background")) {

            MapSizeW = stoi(bufferData[0][0]);
            MapSizeH = stoi(bufferData[1][0]);
            location[0].hBack.loadBitmapWithNativeSize(nameObject);
        }
        else if (!nameObject.find("portal")) {
            location[0].portal.emplace_back(x, y, width, height, 1, "racket"); // ���� ����������� �������� �� ������� �� 1 �������
        }
        else if (!nameObject.find("heal")) {
            location[0].healingFlask.emplace_back(x, y, width, height, "ball");
        }
        else if (!nameObject.find("spike")) {
            location[0].spike.emplace_back(x, y, width, height, "spike");
        }

    }
    file.close();
}

class Serialization {
public:
    Serialization(Location_ container) { this->container = container;}
    
    /// �������� ������� �������� ���������� � ���� � �������� ���� � �������� ����������
    /// �������� ������� �������� �� ������� ��������� ������ � ����� ����� ������� ��� � ���� � ������������ ������ �� �����

    void Ser() {
        ofile.open("SaveData.txt", ios::trunc);
        ofile.close();
        ofile.open("SaveData.txt", ofstream::app);
        if (ofile.is_open())
        {
            ///hardcode?!?!?!?!?!?
            for (int i = 0; i < container.healingFlask.size(); i++) {
                ofile << "<healingFlask>" << "index=" << i << " ";
                SortSprite(container.healingFlask[i].Sprite);
            }
            for (int i = 0; i < container.spike.size(); i++) {
                ofile << "<spike>" << "index=" << i << " ";
                SortSprite(container.spike[i].Sprite);
            }
            for (int i = 0; i < container.Persona.size(); i++) {
                ofile << "<Persona>" << "index=" << i << " ";
                SortSprite(container.Persona[i]->Sprite);
            }
            for (int i = 0; i < container.portal.size(); i++) {
                ofile << "<portal>" << "index=" << i << " ";
                SortSprite(container.portal[i].Sprite);
            }
            for (int i = 0; i < container.walls.size(); i++) {
                ofile << "<walls>" << "index=" << i << " ";
                SortSprite(container.walls[i].Sprite);
                
            }
        }
        ofile.close();
    }
    void Deser() {
        ifile.open("SaveData.txt");
        if (ifile.is_open())
        {
            string StructureArr[]{"<healingFlask>", "<spike>", "<Persona>", "<portal>", "<walls>", "height", "width", "xpos", "ypos"};
            string str;
            while (!ifile.eof()) {
                ifile >> str;
                for (int i = 0; i < 9; i++) {
                        ///����������� ������� �� ��� ������� � �� ��������� ��������
                        if (!str.find(StructureArr[i]) &&
                            !str.find(StructureArr[i] + "index=")) {
                            ReplaceStructure(str, StructureArr[i]);
                            break;
                        }
                        else if (!str.find(StructureArr[i]) &&
                             !str.find(StructureArr[i] + "==")) {
                             ReplaceProperty(str);
                             break;
                             }
                }
            }
        }
        ifile.close();
    }
private:
    string MarkerStructure;
    int IndexStructure;
    Location_ container;
    ifstream ifile;
    ofstream ofile;

    void SortSprite(sprite SprData) {
        ofile << "height==" << SprData.height << " "
              << "width==" << SprData.width << " "
              << "xpos==" << SprData.x << " "
              << "ypos==" << SprData.y << "\n";
    }
    void ReplaceStructure(string str, string StructureArr) {
        StrReplace(&str, StructureArr + "index=");
        MarkerStructure = StructureArr;
        IndexStructure = stoi(str);
        
    };
    void ReplaceProperty(string StructureArr) {
        float height = NULL;
        float width = NULL;
        float xpos = NULL;
        float ypos = NULL;

        if (!StructureArr.find("height")) {
            StrReplace(&StructureArr, "height==");
            height = stof(StructureArr);
        }
        else if (!StructureArr.find("width")) {
            StrReplace(&StructureArr, "width==");
            width = stof(StructureArr);
        }
        else if (!StructureArr.find("xpos")) {
            StrReplace(&StructureArr, "xpos==");
            xpos = stof(StructureArr);
        }
        else if (!StructureArr.find("ypos")) {
            StrReplace(&StructureArr, "ypos==");
            ypos = stof(StructureArr);
        }

        if (MarkerStructure == "<healingFlask>") {
            if (container.healingFlask.size() != 0)
            {
            SwapProperty(&container.healingFlask[IndexStructure].Sprite, height, width, xpos, ypos);
            }
        }
        else if (MarkerStructure == "<spike>") {
            if (container.spike.size()!=0)
            {
            SwapProperty(&container.spike[IndexStructure].Sprite, height, width, xpos, ypos);
            }
        }
        else if (MarkerStructure == "<Persona>") {
            if (container.Persona.size() != 0)
            {
            SwapProperty(&container.Persona[IndexStructure]->Sprite, height, width, xpos, ypos);
            }
        }
        else if (MarkerStructure == "<portal>") {
            if (container.portal.size() != 0)
            {
            SwapProperty(&container.portal[IndexStructure].Sprite, height, width, xpos, ypos);
            }
        }
        else if (MarkerStructure == "<walls>") {
            if (container.walls.size() != 0)
            {
            SwapProperty(&location[0].walls[IndexStructure].Sprite, height, width, xpos, ypos);
            }
        }

        height = NULL;
        width = NULL;
        xpos = NULL;
        ypos = NULL;
    };
    void SwapProperty(sprite* Struct, float h, float w, float x, float y) {
        if (h != NULL) { Struct->height = h; }
        if (w != NULL) { Struct->width = w; }
        if (x != NULL) { Struct->x = x; }
        if (y != NULL) { Struct->y = y; }
    };

};



