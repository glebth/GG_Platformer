#ifndef ENEMY_LIGHT_H
#define ENEMY_LIGHT_H

#include "sprites/enemy.h"

class Player;

class EnemyLight : public Enemy {
public:
    EnemyLight(std::string name, std::string description, Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale = 1,
                bool isCollides  = true,
                std::string animName = "idleLeft");
    
    void ImpactOnPlayer(Player *player) override;

    void Update(float time, Player* player , GG_Vector2 offset = {0, 0});

};

#endif