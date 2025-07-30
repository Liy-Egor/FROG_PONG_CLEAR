#pragma once
#include "Component.h"

using namespace ECC;
void SpikeEvent(CTransform& Transform, CDamage& Damage);
HBITMAP GetBitmap(string BitmapNameFile);
void LoadTransform(CTransform& CTransform, float arr[]);
void CreateObject(CTransform& CTransform);
void Show(CBitmap& CBitmap, CTransform& Transform);
void ShowWindow(CBitmap& CBitmap);
void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump);
void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity);
void ProcessSound(CSound& CSound);
void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);
void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);
void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, ÑSpecialization& ÑSpecialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CTypeÑharacter& CTypeÑharacter, CNameÑharacter& CNameÑharacter, ÑRank& ÑRank,
    string TypeDamage, string Status, string TypeÑharacter, string Gender, string NameChar, string Specialization, int Rank);
void SwitchLotation(CPortalPàth& PortalPàth, CTransform& CTransform);
bool HealEvent(CTransform& Transform, CHealth& Health);
void SpikeEvent(CTransform& Transform,CDamage& Damage);
void UpdateView();
void UpdatePhysics();


//áàçîâûå êëàññû
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
      /*delete this;*/ //êğàøèò ïğîãó 
    }
public:
    virtual void Destroy()
    {
        DeleteAT();
    };
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
    CNameÑharacter* NameÑharacter = ECS.SetComponent<CNameÑharacter>(Entity);
    CJump* Jump = ECS.SetComponent<CJump>(Entity);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Entity);
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);

    BasePerson(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr){}
public:
    CDamage* GetDamage()
    {
        return ECS.GetComponent<CDamage>(Entity, Damage);
    }
    CHealth* GetHealth()
    {
        return ECS.GetComponent<CHealth>(Entity, Health);
    }
    CDefense* GetDefense()
    {
        return ECS.GetComponent<CDefense>(Entity, Defense);
    }
    CJump* GetJump()
    {
        return ECS.GetComponent<CJump>(Entity, Jump);
    }
    CSpeed* GetSpeed()
    {
        return ECS.GetComponent<CSpeed>(Entity, Speed);
    }
    CNameÑharacter* GetNameÑharacter()
    {
        return ECS.GetComponent<CNameÑharacter>(Entity, NameÑharacter);
    }
};

//íàñëåäíèêè
class ATWall : public BaseArcheType
{
public:
    ATWall(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Wall";
    }
}*Wall;

class ATHealFlack : public BaseArcheType
{
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
public:
    ATHealFlack(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "HealFlack";
    }
    bool GoEvent()
    {
       return HealEvent(*Transform, *Health);
    }
}*HealFlack;

class ATSpike : public BaseArcheType
{
    CDamage* Damage = ECS.SetComponent<CDamage>(Entity);
public:
    ATSpike(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Spike";
    }
    void GoEvent()
    {
        SpikeEvent(*Transform, *Damage);
    }
}*Spike;

class ATPortal : public BaseArcheType
{
    CPortalPàth* PortalPàth = ECS.SetComponent<CPortalPàth>(Entity);
public:
    ATPortal(string BitmapNameFile, float arr[],int Paths) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Portal";
        PortalPàth->Pàth = Paths;
    }

    void GoEvent()
    {
        SwitchLotation(*PortalPàth,*Transform);
    }
}*Portal;

class ATEnemy : public BasePerson
{
private:
    CStatusBehavior* StatusBehavior = ECS.SetComponent<CStatusBehavior>(Entity);
    CTypeÑharacter* TypeÑharacter = ECS.SetComponent<CTypeÑharacter>(Entity);
    CGender* Gender = ECS.SetComponent<CGender>(Entity);
    ÑSpecialization* Specialization = ECS.SetComponent<ÑSpecialization>(Entity);
    ÑRank* Rank = ECS.SetComponent<ÑRank>(Entity);
public:
    ATEnemy(string BitmapNameFile, float arr[]) : BasePerson(BitmapNameFile, arr)
    {
        NameObject->Name = "Enemy";
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *TypeÑharacter, *NameÑharacter, *Rank,
            "TypeDamage", "Status", "TypeÑh", "Gendr", "Frog", "Specialist", 0);
    }
    void Start()
    {
        MoveCharacter(*Jump,*Transform,*Speed,*Collider,*Gravity);
        TracerCollide(*Collider, *Transform, *Jump);
        ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*Enemy;

class ATPlayer : public BasePerson
{
public:
    ATPlayer(string BitmapNameFile, float arr[]) : BasePerson(BitmapNameFile, arr)
    {
        NameObject->Name = "Player";
        NameÑharacter->NameChar = "Komar";
    }
    void Start()
    {      
       MovePlayer(*Jump, *Transform, *Speed, *Collider, *Gravity); 
       TracerCollide(*Collider, *Transform, *Jump);
       ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*Player;



//êîíåö âñåõ àğõåòèïîâ
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
    vector<ATEnemy> VEnemy;
}*Location;
vector<ATLocation> VLocation; // ïğè óäàëåíèè îò ñşäà òîæå íàäî áóäåò óäàëÿòü ıêçåìàëÿğû èíà÷å áóäåò óòå÷êà ïàìÿòè (íåáîëüøàÿ íî âñå æå)

