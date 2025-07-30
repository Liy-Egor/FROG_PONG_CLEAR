#pragma once
#include "Component.h"

using namespace ECC;
HBITMAP GetBitmap(string BitmapNameFile);
void LoadTransform(CTransform& CTransform, float arr[]);
void CreateObject(CTransform& CTransform);
void Show(CBitmap& CBitmap, CTransform& Transform);
void ShowWindow(CBitmap& CBitmap);
void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump, int MapLVL);
void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity);
void ProcessSound(CSound& CSound);
void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL);
void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, int MapLVL);
void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, —Specialization& —Specialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CType—haracter& CType—haracter, CName—haracter& CName—haracter, —Rank& —Rank,
    string TypeDamage, string Status, string Type—haracter, string Gender, string NameChar, string Specialization, int Rank);
void SwitchLotation();
void Heal();
void Spikes();
void UpdateView();
void UpdatePhysics();

//·‡ÁÓ‚˚Â ÍÎ‡ÒÒ˚
class BaseArcheType
{
protected:
    int Entity = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Entity);
    CBitmap* Bitmap = ECS.SetComponent<CBitmap>(Entity);
    CSound* Sound = ECS.SetComponent<CSound>(Entity);
    CNameObject* NameObject = ECS.SetComponent<CNameObject>(Entity);
    int WhatLocation;

    BaseArcheType(string BitmapNameFile, float arr[])
    {
        LoadTransform(*Transform, arr);
        Bitmap->HBitMap = GetBitmap(BitmapNameFile);
        CreateObject(*Transform);
        NameObject->Number = Entity;
    }

    void DeleteAT()
    {
      ECS.DeleteEntity(Entity);
      delete this;
    }
public:
    virtual void Destroy()
    {
        DeleteAT();
    };
public:
    void SetLocation(int IdLocation)
    {
        WhatLocation = IdLocation;
    }
    int GetLocation()
    {
        return WhatLocation;
    }
    CTransform* GetPosition()
    {
        return ECS.GetComponent<CTransform>(Entity, Transform);
    }
    CBitmap* GetBitmaps()
    {
        return ECS.GetComponent<CBitmap>(Entity, Bitmap);
    }
};

class BasePerson : public BaseArcheType
{
protected:
    CDamage* Damage = ECS.SetComponent<CDamage>(Entity);
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
    CDefense* Defense = ECS.SetComponent<CDefense>(Entity);
    CSpeed* Speed = ECS.SetComponent<CSpeed>(Entity);
    CName—haracter* Name—haracter = ECS.SetComponent<CName—haracter>(Entity);
    CJump* Jump = ECS.SetComponent<CJump>(Entity);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Entity);
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);

    BasePerson(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr){}
};

//Ì‡ÒÎÂ‰ÌËÍË
class ATWall : public BaseArcheType
{
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);
public:
    ATWall(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Wall";
    }
    bool CheckCollision(float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2)
    {
        return x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2;
    }
}*Wall;



class ATHealFlack : public BaseArcheType
{
public:
    ATHealFlack(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "HealFlack";
    }
}*HealFlack;

class ATSpike : public BaseArcheType
{
public:
    ATSpike(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Spike";
    }
}*Spike;

class ATPortal : public BaseArcheType
{
public:
    ATPortal(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Portal";
    }
}*Portal;



class ATEnemyFrog : public BasePerson
{
private:
    CStatusBehavior* StatusBehavior = ECS.SetComponent<CStatusBehavior>(Entity);
    CType—haracter* Type—haracter = ECS.SetComponent<CType—haracter>(Entity);
    CGender* Gender = ECS.SetComponent<CGender>(Entity);
    —Specialization* Specialization = ECS.SetComponent<—Specialization>(Entity);
    —Rank* Rank = ECS.SetComponent<—Rank>(Entity);
public:
    ATEnemyFrog(string BitmapNameFile, float arr[]) : BasePerson(BitmapNameFile, arr)
    {
        NameObject->Name = "EnemyFrog";
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *Type—haracter, *Name—haracter, *Rank,
            "TypeDamage", "Status", "Type—h", "Gendr", "Frog", "Specialist", 0);

        

    }
    void Start()
    {
        MoveCharacter(*Jump,*Transform,*Speed,*Collider,*Gravity, 0);
        TracerCollide(*Collider, *Transform, *Jump, 0);
        ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*EnemyFrog;

class ATPlayer : public BasePerson
{
public:
    ATPlayer(string BitmapNameFile, float arr[]) : BasePerson(BitmapNameFile, arr)
    {
        NameObject->Name = "Player";
        Name—haracter->NameChar = "Komar";
    }
    void Start()
    {      
       MovePlayer(*Jump, *Transform, *Speed, *Collider, *Gravity, 0); 
       TracerCollide(*Collider, *Transform, *Jump, 0);
       ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*Player;


//ÍÓÌÂˆ ‚ÒÂı ‡ıÂÚËÔÓ‚
class ATLocation : public BaseArcheType
{
public:
    ATLocation(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Level";
    }
    vector<ATWall> VWall;
    vector<ATHealFlack> VHealFlack;
    vector<ATSpike> VSpike;
    vector<ATPortal> VPortal;
    vector<ATEnemyFrog> VEnemyFrog;
}*Location;
vector<ATLocation> VLocation;

