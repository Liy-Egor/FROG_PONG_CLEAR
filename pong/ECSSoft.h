#pragma once
#include "Structures.h"

class ÅntityComponentSystem
{   
private:
    struct Entity
    {   
       unsigned int IDEntity;
    };

    struct ComponentPool
    {
        int IDEntity;
        int IDComponent;
        size_t Mask;
        char* pData = new char [Mask + IDEntity];

        inline char* get()
        {
            return pData;
        }

        void del()
        {
            delete[] pData;
            pData = nullptr;
        }
    };
    
    void DeleteComp(char* MaskTemp, int id)
    {
        for (int i = 0; i < VComponentPool.size(); i++)
        {
            if (id == VComponentPool[i].IDEntity)
            {
                if (MaskTemp == VComponentPool[i].pData || MaskTemp == nullptr)
                {
                    vector<ComponentPool>::iterator it;
                    it = VComponentPool.begin() + i;
                    VComponentPool[i].del();
                    VComponentPool.erase(it);
                    break;
                }
            }
        }
    }

    vector<Entity> VEntity;
    vector<ComponentPool> VComponentPool;
public:

    int NewEntity()
    {
        VEntity.push_back({VEntity.size()});
        return VEntity.back().IDEntity;
    }
    
    template <typename T>
    T* SetComponent(int id)
    {
        size_t Mask = sizeof(T);
        int IDComponent = VComponentPool.size();
            VComponentPool.push_back({ id , IDComponent, Mask });
            T* Comp = new (VComponentPool[IDComponent].get()) T();
            return Comp;
    }

    template <typename T>
    T* GetComponent(int id,T* MaskBit)
    {
        char* MaskTemp = (char*)*&MaskBit;
        for (ComponentPool val : VComponentPool)
        {
            if (id == val.IDEntity && MaskTemp == val.pData)
            {
                T* Comp = (T*)val.pData;
                return Comp;
                break;
            }
        };
    }
    
    template <typename T>
    void DeleteComponent(int id, T* MaskBit)
    {
        char* MaskTemp = (char*)*&MaskBit;
        DeleteComp(MaskTemp, id);
    }

    void DeleteEntity(int id)
    {
        for (int i = 0; i < VEntity.size(); i++)
        {
            if (i == id)
            {
                while (VComponentPool.size() != 0)
                {
                    DeleteComp(nullptr, id);
                }
                vector<Entity>::iterator it;
                it = VEntity.begin() + i;
                VEntity.erase(it);
                break;
            }
        }
    }

}ECS;
