#pragma once
#include "SystemECS.h"
#include "Component.h"
#include "ECSSoft.h"
#define ECC ALLCopmonents


//çäåñü âûñòğàèâàşòñÿ êîíêğåòíûå òèïû ñóùíîñòåé è ïîäêëş÷àşò ê íèì ñèñòåìû
class ATEnemy
{   
private:
    int Enemy = ECS.NewEntity();
    ECC::CTransform* Transform = ECS.SetComponent<ECC::CTransform>(Enemy);
    ECC::CBitmap* Bitmap = ECS.SetComponent<ECC::CBitmap>(Enemy);
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
    }
public:







};
