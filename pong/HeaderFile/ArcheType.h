#pragma once
#include "Component.h"

using namespace ECC;
void SpikeEvent(CTransform& Transform, CDamage& Damage);
void LoadTransform(CTransform& CTransform, float arr[]);
void CreateObject(CTransform& CTransform);
void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump);
void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity);
void ProcessSound(CSound& CSound);
void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation,CAnimationTimeLine& TimeLine, CImputTimer& ImputTimer);
void MoveCharacter(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation,CActionState& ActionState, CAction& Action, ChronoTimer& DetectionTimer);
void ActivityPlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity, CStatusAnimation& StatusAnimation, CAnimationTimeLine& TimeLine, CImputTimer& ImputTimer);
void AddCharacterModifier(
    CHealth& CHealth, CDefense& CDefense, CDamage& CDamage, CSpeed& CSpeed, CSpecialization& CSpecialization,
    CGender& CGender, CStatusBehavior& CStatusBehavior, CTypeCharacter& CTypeCharacter, CNameCharacter& CNameCharacter, CRank& CRank,
    string TypeDamage, string Status, string TypeCharacter, string Gender, string NameChar, string Specialization, int Rank);
void SwitchLotation(CPortalPath& PortalPath, CTransform& CTransform);
bool HealEvent(CTransform& Transform, CHealth& Health);
void SpikeEvent(CTransform& Transform,CDamage& Damage);



//áàçîâûå êëàññû
class BaseArcheType
{
protected:
    int Entity = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Entity);
	CTexture* Textures = ECS.SetComponent<CTexture>(Entity);
    CSound* Sound = ECS.SetComponent<CSound>(Entity);
    CNameObject* NameObject = ECS.SetComponent<CNameObject>(Entity);
	CTypeRender* TypeRenders = ECS.SetComponent<CTypeRender>(Entity);
	CAnimationTimeLine* AnimationTimeLine = ECS.SetComponent<CAnimationTimeLine>(Entity);
	CStatusAnimation* StatusAnimation = ECS.SetComponent<CStatusAnimation>(Entity);
	CImputTimer* ImputTimer = ECS.SetComponent<CImputTimer>(Entity);
    int WhatLocation;
    
    BaseArcheType(string NameFile, float arr[], TypeObject TypeRend)
    {
        LoadTransform(*Transform, arr);
		Textures->Texture = NameFile;
        CreateObject(*Transform);
        NameObject->Number = Entity;
		TypeRenders->TypeRender = TypeRend;
    }

    void DeleteAT()
    {
      ECS.DeleteEntity(Entity);
      /*delete this;*/ //êðàøèò ïðîãó 
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
	CTexture* GetTexture()
    {
        return ECS.GetComponent<CTexture>(Entity, Textures);
    }
	CTypeRender* GetRender()
	{
		return ECS.GetComponent<CTypeRender>(Entity, TypeRenders);
	}
	CNameObject* GetNameObj()
	{
		return ECS.GetComponent<CNameObject>(Entity, NameObject);
	}
	CAnimationTimeLine* GetTimeLine()
	{
		return ECS.GetComponent<CAnimationTimeLine>(Entity, AnimationTimeLine);
	}
	CStatusAnimation* GetStatusAnimation()
	{
		return ECS.GetComponent<CStatusAnimation>(Entity, StatusAnimation);
	}
};

class BasePerson : public BaseArcheType
{
protected:
    CDamage* Damage = ECS.SetComponent<CDamage>(Entity);
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
    CDefense* Defense = ECS.SetComponent<CDefense>(Entity);
    CSpeed* Speed = ECS.SetComponent<CSpeed>(Entity);
    CNameCharacter* NameCharacter = ECS.SetComponent<CNameCharacter>(Entity);
    CJump* Jump = ECS.SetComponent<CJump>(Entity);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Entity);
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);

    BasePerson(string NameFile, float arr[], TypeObject TypeRend) : BaseArcheType(NameFile, arr, TypeRend){}
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
    CNameCharacter* GetNameCharacter()
    {
        return ECS.GetComponent<CNameCharacter>(Entity, NameCharacter);
    }
};

//íàñëåäíèêè
class ATWall : public BaseArcheType
{
public:
    ATWall(string NameFile, float arr[], TypeObject TypeRend) : BaseArcheType(NameFile, arr, TypeRend)
    {
        NameObject->Name = "Wall";
    }
}*Wall;

class ATHealFlack : public BaseArcheType
{
    CHealth* Health = ECS.SetComponent<CHealth>(Entity);
public:
    ATHealFlack(string NameFile, float arr[], TypeObject TypeRend) : BaseArcheType(NameFile, arr, TypeRend)
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
    ATSpike(string NameFile, float arr[], TypeObject TypeRend) : BaseArcheType(NameFile, arr, TypeRend)
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
    CPortalPath* PortalPath = ECS.SetComponent<CPortalPath>(Entity);
public:
    ATPortal(string NameFile, float arr[], TypeObject TypeRend,int Paths) : BaseArcheType(NameFile, arr, TypeRend)
    {
        NameObject->Name = "Portal";
        PortalPath->Path = Paths;
    }

    void GoEvent()
    {
        SwitchLotation(*PortalPath,*Transform);
    }
}*Portal;

class ATEnemy : public BasePerson
{
private:
	CStatusBehavior* StatusBehavior = ECS.SetComponent<CStatusBehavior>(Entity);
	CTypeCharacter* TypeCharacter = ECS.SetComponent<CTypeCharacter>(Entity);
	CGender* Gender = ECS.SetComponent<CGender>(Entity);
	CSpecialization* Specialization = ECS.SetComponent<CSpecialization>(Entity);
	CRank* Rank = ECS.SetComponent<CRank>(Entity);
	CActionState* ActionState = ECS.SetComponent<CActionState>(Entity);
	CAction* Action = ECS.SetComponent<CAction>(Entity);
public:
	ATEnemy(string NameFile, float arr[], TypeObject TypeRend) : BasePerson(NameFile, arr, TypeRend)
	{
		NameObject->Name = "enemy";
		AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *TypeCharacter, *NameCharacter, *Rank,
			"TypeDamage", "Status", "TypeCh", "Gendr", "Frog", "Specialist", 0);
	}
	void Start()
	{
		MoveCharacter(*Jump, *Transform, *Speed, *Collider, *Gravity, *StatusAnimation, *ActionState, *Action, Timer);
		TracerCollide(*Collider, *Transform, *Jump);
		ProcessGravity(*Jump, *Transform, *Gravity);
	}
}*Enemy;

class ATPlayer : public BasePerson
{
public:
    ATPlayer(string NameFile, float arr[], TypeObject TypeRend) : BasePerson(NameFile, arr, TypeRend)
    {
        NameObject->Name = "player";
        NameCharacter->NameChar = "Komar";
    }
    void Start()
    {      
       MovePlayer(*Jump, *Transform, *Speed, *Collider, *Gravity, *StatusAnimation,*AnimationTimeLine,*ImputTimer);
	   ActivityPlayer(*Jump, *Transform, *Speed, *Collider, *Gravity, *StatusAnimation, *AnimationTimeLine,*ImputTimer);
       TracerCollide(*Collider, *Transform, *Jump);
       ProcessGravity(*Jump, *Transform, *Gravity);
    }

}*Player;


class ATLocation : public BaseArcheType
{
public:
    ATLocation(string NameFile, float arr[], TypeObject TypeRend) : BaseArcheType(NameFile, arr, TypeRend)
    {
        NameObject->Name = "Level";
    }
    vector<ATWall> VWall;
    vector<ATHealFlack> VHealFlack;
    vector<ATSpike> VSpike;
    vector<ATPortal> VPortal;
    vector<ATEnemy> VEnemy;

}*Location;
vector<ATLocation> VLocation; 

