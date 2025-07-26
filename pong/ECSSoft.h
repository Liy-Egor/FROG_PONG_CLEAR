#pragma once
#include "Structures.h"

class ЕntityComponentSystem
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
        char* pData = new char[Mask + IDEntity];
        
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
    
   
    void DeleteComp(size_t Mask, int id)
    {
        for (int i = 0; i < VComponentPool.size(); i++)
        {
            if (Mask == VComponentPool[i].Mask || Mask  == false)
            {
                if (id == VComponentPool[i].IDEntity)
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
    T* GetComponent(int id)
    {
        size_t Mask = sizeof(T);
        for (ComponentPool val : VComponentPool)
        {
            if (Mask == val.Mask && id == val.IDEntity)
            {
                T* Comp = new (val.get()) T();
                return Comp;
                break;
            }
        };
    }
    
    template <typename T>
    void DeleteComponent(int id)
    {
        size_t Mask = sizeof(T);
        DeleteComp(Mask, id);
    }

    void DeleteEntity(int id)
    {
        for (int i = 0; i < VEntity.size(); i++)
        {
            if (i == id)
            {
                for (int i = 0; i < VComponentPool.size(); i++)
                { //////////////////// нужно обнулять итератор а то он продолжает накпливаться
                    ///выше сделать еще бетот как гет только с *char = dasd new char[] return dasd delete dasd
                    //это только для сравнения масок не более для точности крч
                    DeleteComp(false, id);
                }
                vector<Entity>::iterator it;
                it = VEntity.begin() + i;
                VEntity.erase(it);
                break;
            }
        }
    }

}ECS;
