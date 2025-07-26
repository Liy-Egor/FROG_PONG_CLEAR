#pragma once
#include "SystemECS.h"
#include "Component.h"
#include "ECSSoft.h"
#define ECC ALLCopmonents
//çäåñü âûñòğàèâàşòñÿ êîíêğåòíûå òèïû ñóùíîñòåé è ïîäêëş÷àşò ê íèì ëîãèêó

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
    ECC::CTypeÑharacter* TypeÑharacter = ECS.SetComponent<ECC::CTypeÑharacter>(Enemy);
    ECC::CJump* Jump = ECS.SetComponent<ECC::CJump>(Enemy);
    ECC::CGender* Gender = ECS.SetComponent<ECC::CGender>(Enemy);
    ECC::CNameÑharacter* NameÑharacter = ECS.SetComponent<ECC::CNameÑharacter>(Enemy);
    ECC::ÑSpecialization* Specialization = ECS.SetComponent<ECC::ÑSpecialization>(Enemy);
    ECC::ÑRank* Rank = ECS.SetComponent<ECC::ÑRank>(Enemy);
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
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *TypeÑharacter, *NameÑharacter, *Rank,
            "TypeDamage", "Status", "TypeÑh", "Gendr", "NameChar", "Specialist", 0);
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
