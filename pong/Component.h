#pragma once
#include "Structures.h"

namespace ECC
{
//Base Components
struct CTransform
{
    int x, y, Width, Height, Dx = 0, Dy = 0, Scale = 2;
};

struct CBitmap
{
    HBITMAP HBitMap;
};

struct CGravity
{
    int Gravity = 30;
};

struct CSound
{
   const char* SoundNameFile;
};

struct CCollider
{
   bool CollXfound = false;
   bool CollYfound = false;
   int LastTracePlatformNum = -1;
   int Direction = 1;
};

//—haracter Components
struct CHealth
{
    int Health = 100, MaxHealth = 100;
};

struct CDamage
{
    int Damage = 10;
    string TypeDamage;
};

struct CDefense
{
    int Defense = 100;
    float ResistElements = 0., ResistPhysics = 0., ResistDistantDamage = 0. ;
};

struct CSpeed
{
    int SpeedAttack = 10, SpeedWalk = 15;
};

struct —Rank
{
    int Rank = 0;
};

struct CGender
{
    string Gender;
};

struct CType—haracter
{
    string Type—haracter;
};

struct CStatusBehavior
{
    string Status;
};

struct —Specialization
{
    string Specialization;
};

struct CName—haracter
{
    string NameChar;
};

struct CJump
{
    int Jump = 0;
    bool InJumpBot = false;
    bool InJump = false;
};

};