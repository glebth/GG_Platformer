#ifndef ENEMY_H
#define ENEMY_H

#include "sprites/npc.h"

class Enemy : public Npc {
public:
    Enemy(std::string name, std::string description, Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale = 1,
                bool isCollides  = true,
                std::string animName = "idleLeft") : 
                Npc(name, description, graphics, filePath, 
                xText, yText, 
                wText, hText, 
                xMap, yMap, 
                animUpdate, facing,
                boundingBoxScale,
                isCollides,
                animName) 
                {};
    virtual void ImpactOnPlayer() = 0;

protected:
    bool _isAlive;
    unsigned int _currentHP;
    unsigned int _maxHP;
};

#endif