#pragma once
#include "SystemECS.h"
#include "Component.h"
#include "ECSSoft.h"
using namespace ECC;
//����� ������������� ���������� ���� ��������� � ���������� � ��� ������

class ATEnemy
{
private:
    int Enemy = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Enemy);
    CBitmap* Bitmap = ECS.SetComponent<CBitmap>(Enemy);
    CSound* Sound = ECS.SetComponent<CSound>(Enemy);
    CHealth* Health = ECS.SetComponent<CHealth>(Enemy);
    CDamage* Damage = ECS.SetComponent<CDamage>(Enemy);
    CDefense* Defense = ECS.SetComponent<CDefense>(Enemy);
    CSpeed* Speed = ECS.SetComponent<CSpeed>(Enemy);
    CStatusBehavior* StatusBehavior = ECS.SetComponent<CStatusBehavior>(Enemy);
    CType�haracter* Type�haracter = ECS.SetComponent<CType�haracter>(Enemy);
    CJump* Jump = ECS.SetComponent<CJump>(Enemy);
    CGender* Gender = ECS.SetComponent<CGender>(Enemy);
    CName�haracter* Name�haracter = ECS.SetComponent<CName�haracter>(Enemy);
    �Specialization* Specialization = ECS.SetComponent<�Specialization>(Enemy);
    �Rank* Rank = ECS.SetComponent<�Rank>(Enemy);
    CCollider* Collider = ECS.SetComponent<CCollider>(Enemy);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Enemy);
    void DeleteAT()
    {
        ECS.DeleteEntity(Enemy);
        delete this;
    }
    
public:
    
    ATEnemy(string BitmapNameFile)
    {   
        Bitmap->HBitMap = GetBitmap(BitmapNameFile);
        LoadTransform(*Transform, GetTransformData());
        CreateCharacter(*Transform);
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *Type�haracter, *Name�haracter, *Rank,
            "TypeDamage", "Status", "Type�h", "Gendr", "NameChar", "Specialist", 0);
    }
    int Start()
    {
        Show(*ECS.GetComponent<CBitmap>(Enemy, Bitmap), *ECS.GetComponent<CTransform>(Enemy, Transform));

        MoveCharacter(
            *ECS.GetComponent<CJump>(Enemy, Jump),
            *ECS.GetComponent<CTransform>(Enemy, Transform),
            *ECS.GetComponent<CSpeed>(Enemy, Speed),
            *ECS.GetComponent<CCollider>(Enemy, Collider),
            *ECS.GetComponent<CGravity>(Enemy, Gravity),
            0);
    }

    void Destroy()
    {
        DeleteAT();
    }
    
}*Enemy;
