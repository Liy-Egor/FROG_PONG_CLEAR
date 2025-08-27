#pragma once
#include "Component.h"

using namespace ECC;
void SpikeEvent(CTransform& Transform, CDamage& Damage);
void LoadTransform(CTransform& CTransform, float arr[]);
void CreateObject(CTransform& CTransform);
void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump);
void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity);
void ProcessSound(CSound& CSound);
void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);
void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);
void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, �Specialization& �Specialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CType�haracter& CType�haracter, CName�haracter& CName�haracter, �Rank& �Rank,
    string TypeDamage, string Status, string Type�haracter, string Gender, string NameChar, string Specialization, int Rank);
void SwitchLotation(CPortalP�th& PortalP�th, CTransform& CTransform);
bool HealEvent(CTransform& Transform, CHealth& Health);
void SpikeEvent(CTransform& Transform,CDamage& Damage);
void UpdateView();
void UpdatePhysics();


//������� ������
class BaseArcheType
{
protected:
    int Entity = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Entity);
	CTexture* Textures = ECS.SetComponent<CTexture>(Entity);
    CSound* Sound = ECS.SetComponent<CSound>(Entity);
    CNameObject* NameObject = ECS.SetComponent<CNameObject>(Entity);
    int WhatLocation;
    

    BaseArcheType(string NameFile, float arr[])
    {
        LoadTransform(*Transform, arr);
		Textures->Texture = NameFile;
        CreateObject(*Transform);
        NameObject->Number = Entity;
    }

    void DeleteAT()
    {
      ECS.DeleteEntity(Entity);
      /*delete this;*/ //������ ����� 
    }


public:
	int GetID()
	{
		return Entity;
	}
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
	CTexture* GetTexture()
    {
        return ECS.GetComponent<CTexture>(Entity, Textures);
    }
};

class BasePerson : public BaseArcheType
{
protected:
    CDamage* Damage = ECS.SetComponent<CDamage>(Entity);
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
    CDefense* Defense = ECS.SetComponent<CDefense>(Entity);
    CSpeed* Speed = ECS.SetComponent<CSpeed>(Entity);
    CName�haracter* Name�haracter = ECS.SetComponent<CName�haracter>(Entity);
    CJump* Jump = ECS.SetComponent<CJump>(Entity);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Entity);
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);

    BasePerson(string NameFile, float arr[]) : BaseArcheType(NameFile, arr){}
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
    CName�haracter* GetName�haracter()
    {
        return ECS.GetComponent<CName�haracter>(Entity, Name�haracter);
    }
};

//����������
class ATWall : public BaseArcheType
{
public:
    ATWall(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
    {
        NameObject->Name = "Wall";
    }
}*Wall;

class ATHealFlack : public BaseArcheType
{
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
public:
    ATHealFlack(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
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
    ATSpike(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
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
    CPortalP�th* PortalP�th = ECS.SetComponent<CPortalP�th>(Entity);
public:
    ATPortal(string NameFile, float arr[],int Paths) : BaseArcheType(NameFile, arr)
    {
        NameObject->Name = "Portal";
        PortalP�th->P�th = Paths;
    }

    void GoEvent()
    {
        SwitchLotation(*PortalP�th,*Transform);
    }
}*Portal;

class ATEnemy : public BasePerson
{
private:
    CStatusBehavior* StatusBehavior = ECS.SetComponent<CStatusBehavior>(Entity);
    CType�haracter* Type�haracter = ECS.SetComponent<CType�haracter>(Entity);
    CGender* Gender = ECS.SetComponent<CGender>(Entity);
    �Specialization* Specialization = ECS.SetComponent<�Specialization>(Entity);
    �Rank* Rank = ECS.SetComponent<�Rank>(Entity);
public:
    ATEnemy(string NameFile, float arr[]) : BasePerson(NameFile, arr)
    {
        NameObject->Name = "Enemy";
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *Type�haracter, *Name�haracter, *Rank,
            "TypeDamage", "Status", "Type�h", "Gendr", "Frog", "Specialist", 0);
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
    ATPlayer(string NameFile, float arr[]) : BasePerson(NameFile, arr)
    {
        NameObject->Name = "Player";
        Name�haracter->NameChar = "Komar";
    }
    void Start()
    {      
       MovePlayer(*Jump, *Transform, *Speed, *Collider, *Gravity); 
       TracerCollide(*Collider, *Transform, *Jump);
       ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*Player;

//����� ���� ���������
class ATLocation : public BaseArcheType
{
public:
    ATLocation(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
    {
        NameObject->Name = "Level";
    }
    vector<ATWall> VWall;
    vector<ATHealFlack> VHealFlack;
    vector<ATSpike> VSpike;
    vector<ATPortal> VPortal;
    vector<ATEnemy> VEnemy;

}*Location;
vector<ATLocation> VLocation; // ��� �������� �� ���� ���� ���� ����� ������� ���������� ����� ����� ������ ������ (��������� �� ��� ��)

