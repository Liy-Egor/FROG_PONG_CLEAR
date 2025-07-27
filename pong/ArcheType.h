#pragma once
#include "SystemECS.h"
#include "Component.h"
#include "ECSSoft.h"
using namespace ECC;
//çäåñü âûñòğàèâàşòñÿ êîíêğåòíûå òèïû ñóùíîñòåé è ïîäêëş÷àşò ê íèì ëîãèêó

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
    CTypeÑharacter* TypeÑharacter = ECS.SetComponent<CTypeÑharacter>(Enemy);
    CJump* Jump = ECS.SetComponent<CJump>(Enemy);
    CGender* Gender = ECS.SetComponent<CGender>(Enemy);
    CNameÑharacter* NameÑharacter = ECS.SetComponent<CNameÑharacter>(Enemy);
    ÑSpecialization* Specialization = ECS.SetComponent<ÑSpecialization>(Enemy);
    ÑRank* Rank = ECS.SetComponent<ÑRank>(Enemy);
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
        AddCharacterModifier(*Health, *Defense, *Damage, *Speed, *Specialization, *Gender, *StatusBehavior, *TypeÑharacter, *NameÑharacter, *Rank,
            "TypeDamage", "Status", "TypeÑh", "Gendr", "NameChar", "Specialist", 0);
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
