#pragma once
//#include "Structures.h"
//#include <memory>
//#include "GameFileSystem.h"
//#include "ArcheType.h"
//
//
//void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
//{
//    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
//}
//
//void tracer_collide(auto& Character)
//{
//    Character.last_trace_platform_num = -1;
//    sprite& Sprite = Character.Sprite;
//
//    bool coll_x_found = false;
//    bool coll_y_found = false;
//
//    float lenght = sqrt(pow(Sprite.dx, 2) + pow(Sprite.dy, 2));
//    for (float i = 0; i < lenght; i++)
//    {
//        if (coll_x_found && coll_y_found) return;
//
//        for (int k = 0; k < 4; k++)
//        {
//            for (int j = 0; j < location[Character.currentLocation].walls.size(); j++)
//            {
//                float Bbox[] = {
//                    Sprite.x + Sprite.dx * i / lenght, Sprite.y + Sprite.dy * i / lenght,
//                    Sprite.x + Sprite.width + Sprite.dx * i / lenght - 1, Sprite.y + Sprite.dy * i / lenght,
//                    Sprite.x + Sprite.width + Sprite.dx * i / lenght - 1, Sprite.y + Sprite.height + Sprite.dy * i / lenght - 1,
//                    Sprite.x + Sprite.dx * i / lenght, Sprite.y + Sprite.height + Sprite.dy * i / lenght - 1
//                };
//
//                float pixel_x = Bbox[k * 2];
//                float pixel_y = Bbox[k * 2 + 1];
//
//                SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));
//
//
//
//                auto walls = location[Character.currentLocation].walls[j].Sprite;
//                if ((pixel_x >= walls.x &&
//                    pixel_x <= walls.x + walls.width) &&
//                    (pixel_y >= walls.y &&
//                        pixel_y <= walls.y + walls.height)
//                    )
//                {
//                    float top = pixel_y - walls.y;
//                    float down = (walls.y + walls.height) - pixel_y;
//                    float left = pixel_x - walls.x;
//                    float right = (walls.x + walls.width) - pixel_x;
//
//                    float minX = min(left, right);
//                    float minY = min(top, down);
//                    Character.inJump = false;
//
//                    if (minX < minY && !coll_x_found)
//                    {
//                        Sprite.dx = 0;
//                        coll_x_found = true;
//
//                        if (left < right)
//                        {
//                            Sprite.x = walls.x - Sprite.width - 1;
//                        }
//                        else
//                        {
//                            Sprite.x = walls.x + walls.width + 1;
//                        }
//
//                        j++;
//                    }
//
//                    if (minX >= minY && !coll_y_found)
//                    {
//                        Sprite.dy = 0;
//                        coll_y_found = true;
//
//                        if (down < top)
//                        {
//                            Sprite.y = walls.y + walls.height + 1;
//                            Sprite.jump = 30;
//                        }
//                        else
//                        {
//                            Character.last_trace_platform_num = j;
//                            Sprite.y = walls.y - Sprite.height - 1;
//                            Character.inJumpBot = false;
//                        }
//
//                        j++;
//                    }
//
//                }
//
//            }
//        }
//
//    }
//
//    if (!coll_x_found) Sprite.x += Sprite.dx;
//    if (!coll_y_found) Sprite.y += Sprite.dy;
//}
//
