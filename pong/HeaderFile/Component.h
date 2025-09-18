#pragma once
#include"PrimaryProcess.h"
#include"GraphicManager.h"

namespace ECC
{
  
//базовые компоненты
struct CTransform
{
    float x, y, Width, Height, Dx = 0., Dy = 0.;
    int Scale = 2;
};

struct CTexture
{
    string Texture;
};

struct CGravity
{
    float Gravity = 30;
};

struct CSound
{
   const char* SoundNameFile;
};

struct CCollider
{
   int LastTracePlatformNum = -1;
   int Direction = 1;
};

struct CNameObject
{
    const char* Name;
    int Number;
};

struct CTypeRender
{
	TypeObject TypeRender;
};

struct CAnimationTimeLine
{
	vector<int> TimeLineIt;
	vector<string> TimeLineName;
};

struct CStatusAnimation
{
	StatusAnimate StatusAnim = StatusAnimate::DEFAULT;
	int Mirror = 1;
};

//Ñharacter Components
struct CHealth
{
    int Health = 100, MaxHealth = 100;
};

struct CDamage
{
    int Damage = 10;
    const char* TypeDamage;
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

struct CRank
{
    int Rank = 0;
};

struct CGender
{
    const char* Gender;
};

struct CTypeCharacter
{
    const char* TypeCharacter;
};

struct CStatusBehavior
{
    const char* Status;
};

struct CSpecialization
{
    const char* Specialization;
};

struct CNameCharacter
{
    const char* NameChar;
};

struct CJump
{
    float Jump = 0.;
    bool InJumpBot = false;
    bool InJump = false;
};

struct CPortalPath
{
    int Path;
};

};