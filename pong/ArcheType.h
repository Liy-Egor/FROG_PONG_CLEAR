#pragma once
#include "SystemECS.h"
#include "Component.h"
#include "ECSSoft.h"
#define ECC ALLCopmonents
//����� ������������� ���������� ���� ��������� � ���������� � ��� ������

class ATEnemy
{
private:
    int Enemy = ECS.NewEntity();
    ECC::CTransform* Transform = ECS.SetComponent<ECC::CTransform>(Enemy);
    ECC::CBitmap* Bitmap = ECS.SetComponent<ECC::CBitmap>(Enemy);
    ECC::CSound* Sound = ECS.SetComponent<ECC::CSound>(Enemy);
    ECC::CHealth* Health = ECS.SetComponent<ECC::CHealth>(Enemy);
    ECC::CDamage* Damage = ECS.SetComponent<ECC::CDamage>(Enemy);
    ECC::CDefense* Defense = ECS.SetComponent<ECC::CDefense>(Enemy);
    ECC::CSpeed* Speed = ECS.SetComponent<ECC::CSpeed>(Enemy);
    ECC::CStatusBehavior* StatusBehavior = ECS.SetComponent<ECC::CStatusBehavior>(Enemy);
    ECC::CType�haracter* Type�haracter = ECS.SetComponent<ECC::CType�haracter>(Enemy);
    ECC::CJump* Jump = ECS.SetComponent<ECC::CJump>(Enemy);
    ECC::CGender* Gender = ECS.SetComponent<ECC::CGender>(Enemy);
    ECC::CName�haracter* Name�haracter = ECS.SetComponent<ECC::CName�haracter>(Enemy);
    ECC::�Specialization* Specialization = ECS.SetComponent<ECC::�Specialization>(Enemy);
    ECC::�Rank* Rank = ECS.SetComponent<ECC::�Rank>(Enemy);
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
        Show(*ECS.GetComponent < ECC::CBitmap>(Enemy), *ECS.GetComponent < ECC::CTransform >(Enemy));

        MoveCharacter(
            *ECS.GetComponent<ECC::CJump>(Enemy),
            *ECS.GetComponent<ECC::CTransform>(Enemy),
            *ECS.GetComponent<ECC::CSpeed>(Enemy),
            *ECS.GetComponent<ECC::CCollider>(Enemy),
            *ECS.GetComponent<ECC::CGravity>(Enemy),
            0);
    }

    void Destroy()
    {
        DeleteAT();
    }
    
}*Enemy;
