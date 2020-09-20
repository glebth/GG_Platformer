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

    _dx = -globals::ENEMY_LIGHT_WALK_SPEED;

    _previousCollisionSide = sides::RectSide::NONE;

    _lastDamageTime = SDL_GetTicks();

    _damageHP = -1;
};

void EnemyLight::Update(float time, Player* player , GG_Vector2 offset /*= {0, 0}*/ ) {

    if (_currentHP <= 0) {

        PlayAnimation("killLeft");
        _isColliding = false;

        if (_isGrounded) {
            _isGrounded = false;
            _dy = globals::JUMP_SPEED;
        }
        if (SDL_GetTicks() - _lastDamageTime > globals::ENEMY_LIGHT_DEATH_TIME)
            _isAlive = false;
    }

    AnimatedSprite::Update(time, offset);

}

void EnemyLight::HandleCollision(std::vector<GG_Rectangle> &othersRectangles) {
    
    if (_isColliding) {
        
        for (size_t i = 0; i < othersRectangles.size(); i++) {
            sides::RectSide collisionSide = Sprite::GetCollisionSide(othersRectangles[i]);            
            
            switch (collisionSide) {
                case sides::RectSide::BOTTOM:
                    this->_spriteMapPosition.y = othersRectangles[i].GetTop() - this->GetBoundingbox().GetHeight();
                    this->_dy = 0;
                    this->_isGrounded = true;
                    break;
                case sides::RectSide::TOP:
                    this->_spriteMapPosition.y = othersRectangles[i].GetBottom();
                    this->_dy = 0;
                    if (_isGrounded) {
                        _dx = 0;
                    }
                    break;
                case sides::RectSide::LEFT:
                    this->_spriteMapPosition.x = othersRectangles[i].GetRight();// + 1;
                    break;
                case sides::RectSide::RIGHT:
                    this->_spriteMapPosition.x = othersRectangles[i].GetLeft() - this->GetBoundingbox().GetWidth();// - 1;
                    break;                          
                default:
                    break;
            }    

            if (collisionSide == sides::RectSide::LEFT || collisionSide == sides::RectSide::RIGHT) {
                _facing = (collisionSide == sides::RectSide::LEFT) ? RIGHT : LEFT;
                _dx = -_dx;
            }
            if ( collisionSide == sides::RectSide::BOTTOM && _previousCollisionSide == sides::RectSide::BOTTOM &&
                (othersRectangles[i].GetLeft() == _spriteMapPosition.x || 
                (othersRectangles[i].GetRight() == _spriteMapPosition.x + _spriteTextureRect.w)) 
                ) 
            {
                _facing = (_dx > 0) ? LEFT : RIGHT;
                _dx = -_dx;
            }
            _previousCollisionSide = collisionSide;

            PlayAnimation(_facing == LEFT && _currentHP > 0 ? "runLeft" : "runRight");
        }
    }
}

void EnemyLight::ImpactOnPlayer(Player *player) {
    ;
}

void EnemyLight::ChangeHP(int hpAmount) {

    if (_currentHP > 0) {
        _currentHP += hpAmount;
        _lastDamageTime = SDL_GetTicks();
    }
}

void EnemyLight::AnimationDone(std::string currentAnimation) {
    ;
}