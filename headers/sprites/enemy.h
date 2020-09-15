#ifndef ENEMY_H
#define ENEMY_H

#include "sprites/npc.h"

class Player;

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
    virtual void ImpactOnPlayer(Player *player) = 0;
    virtual void Update(float time, Player* player , GG_Vector2 offset = {0, 0}) = 0;

    void HandleCollision(std::vector<GG_Rectangle> &othersRectangles) = 0;

    virtual void ChangeHP(int hpAmount) = 0;

    const int GetDamage() const { return _damageHP; }
    const bool GetAliveStatus() const { return _isAlive; }
    const bool GetCollisonStatus() const { return _isColliding; }

protected:
    bool _isAlive;
    unsigned int _currentHP;
    unsigned int _maxHP;
    int _damageHP;

    int _lastDamageTime;

    void AnimationDone(std::string currentAnimation) {;};
};

#endif