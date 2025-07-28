#pragma once
#include "Component.h"
#include "ECSSoft.h"
#include "SystemECS.h"

using namespace ECC;

//·‡ÁÓ‚˚Â ÍÎ‡ÒÒ˚
class BaseArcheType
{
protected:
    int Entity = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Entity);
    CBitmap* Bitmap = ECS.SetComponent<CBitmap>(Entity);
    CSound* Sound = ECS.SetComponent<CSound>(Entity);
    CNameObject* NameObject = ECS.SetComponent<CNameObject>(Entity);

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
class ATLocation : public BaseArcheType
{
public:
    ATLocation(string BitmapNameFile, float arr[]) : BaseArcheType(BitmapNameFile, arr)
    {
        NameObject->Name = "Level";
    }
    void show()
    {
        ShowWindow(*Bitmap);
    }
    void go()
    {
        Show(*ECS.GetComponent<CBitmap>(Entity, Bitmap), *ECS.GetComponent<CTransform>(Entity, Transform));
    }
}*Location;

class ATWall : public BaseArcheType
{
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
    CTransform* GetPosition()
    {
        return ECS.GetComponent<CTransform>(Entity, Transform);
    }
    void go()
    {
        Show(*ECS.GetComponent<CBitmap>(Entity, Bitmap), *ECS.GetComponent<CTransform>(Entity, Transform));
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
    void Start(CTransform *TransformWalls)
    {
        Show(*ECS.GetComponent<CBitmap>(Entity, Bitmap), *ECS.GetComponent<CTransform>(Entity, Transform));

        MoveCharacter(
            *TransformWalls,
            *ECS.GetComponent<CJump>(Entity, Jump),
            *ECS.GetComponent<CTransform>(Entity, Transform),
            *ECS.GetComponent<CSpeed>(Entity, Speed),
            *ECS.GetComponent<CCollider>(Entity, Collider),
            *ECS.GetComponent<CGravity>(Entity, Gravity),
            0);
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
    void Start(CTransform *TransformWalls)
    {
       
        
        MovePlayer(
            *TransformWalls,
            *ECS.GetComponent<CJump>(Entity, Jump),
            *ECS.GetComponent<CTransform>(Entity, Transform),
            *ECS.GetComponent<CSpeed>(Entity, Speed),
            *ECS.GetComponent<CCollider>(Entity, Collider),
            *ECS.GetComponent<CGravity>(Entity, Gravity),
            0);
        
        float cameraHalfWidth = (window.width / 2) / Transform->Scale;
        float cameraHalfHeight = (window.height / 2) / Transform->Scale;

        float targetX = Transform->x;
        float targetY = Transform->y;

        targetX = max(0 + cameraHalfWidth,
            min(MapSizeW - cameraHalfWidth, targetX));
        targetY = max(0 + cameraHalfHeight,
            min(MapSizeH - cameraHalfHeight, targetY));

        player_view.x = lerp(player_view.x, targetX, 0.1f);
        player_view.y = lerp(player_view.y, targetY, 0.1f);
    }

    void go()
    {
        Show(*ECS.GetComponent<CBitmap>(Entity, Bitmap), *ECS.GetComponent<CTransform>(Entity, Transform));
    }

}*Player;




