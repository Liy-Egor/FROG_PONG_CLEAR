#pragma once
#include "Structures.h"

struct ALLCopmonents
{
//Base Components
struct CTransform
{
    int x, y, Width, Height, Dx, Dy, Scale;
};

struct CBitmap
{
    HBITMAP HBitMap;
};

struct CGravity
{
    int Gravity;
};

//—haracter Components
struct CHealth
{
    int Health , MaxHealth;
};

struct CDamage
{
    int Damage;
    string TypeDamage;
};

struct CDefense
{
    int Defense, ResistElements, ResistPhysics, ResistDistantDamage;
};

struct CSpeed
{
    int SpeedAttack, SpeedWalk;
};

struct —Rank
{
    int Rank;
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
    string Name;
};

struct CJump
{
    int Jump;
};
};