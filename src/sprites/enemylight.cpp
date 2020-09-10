#include "sprites/enemylight.h"
#include "sprites/player.h"

EnemyLight::EnemyLight(std::string name, std::string description, Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale /* = 1*/,
                bool isCollides /* = true*/,
                std::string animName /*= "idleLeft"*/)
    : Enemy(name, description, graphics, filePath, 
            xText, yText, 
            wText, hText, 
            xMap, yMap, 
            animUpdate, facing,
            boundingBoxScale,
            isCollides,
            animName)
{
    _isAlive = true;
    _maxHP = 1;
    _currentHP = _maxHP;

    _damageHP = -1;
};

void EnemyLight::ImpactOnPlayer(Player *player) {
    ;
}