#include "sprites/player.h"
#include "level.h"
#include "input.h"
#include "menu.h"
#include "sprites/enemy.h"


namespace playerData {
    std::string PLAYER_FILE = "data/sprites/MyChar.png";

    float X_TEXT = 0.0f;
    float Y_TEXT = 0.0f;
    float W_TEXT = 16.0f;
    float H_TEXT = 16.0f;

    float ANIM_SPEED = 200.0f;

    int MAX_HEALTH = 3; 

};

Player::Player(Graphics &graphics, const float xMap, const float yMap) : 
    AnimatedSprite::AnimatedSprite (graphics, playerData::PLAYER_FILE, playerData::X_TEXT,
                                playerData::Y_TEXT, playerData::W_TEXT, playerData::H_TEXT, 
                                xMap, yMap, playerData::ANIM_SPEED, globals::BOUNDING_BOX_SCALE)
{
    _currentAnimationName = "";

    _maxHealth = playerData::MAX_HEALTH;
    _currentHealth = _maxHealth;

    _isLookingDown = false;
    _isLookingUp = false;

    _isDamaged = false;

    SetupAnimations();
    PlayAnimation("idleRight");
}

void Player::SetupAnimations() {
    std::vector<int> runFramesVector = {0, 1, 0, 2};
    
    LoadAnimations(runFramesVector, 0, 0, "runLeft");
    LoadAnimations(runFramesVector, 0, 16, "runRight");

    LoadAnimations(1, 0, 0, "idleLeft");
    LoadAnimations(1, 0, 16, "idleRight");

    LoadAnimations(1, 3*16, 0, "IdleLeftUp");
    LoadAnimations(1, 3*16, 16, "IdleRightUp");
    LoadAnimations(runFramesVector, 3*16, 0, "RunLeftUp");
    LoadAnimations(runFramesVector, 3*16, 16, "RunRightUp");

    LoadAnimations(1, 6*16, 0, "IdleLeftDown");
    LoadAnimations(1, 6*16, 16, "IdleRightDown");
    LoadAnimations(1, 7*16, 0, "IdleLeftBack");
    LoadAnimations(1, 7*16, 16, "IdleRightBack");
}

void Player::Draw( Graphics &graphics ) {
    AnimatedSprite::Draw(graphics);
}
void Player::AnimationDone(std::string currentAnimation) {
    ;
}

void Player::MoveLeft() {
    if (_isLookingUp || _isLookingDown) return;
    
    _dx = -globals::PLAYER_WALK_SPEED;
    PlayAnimation("runLeft");
    _facing = LEFT;
}
void Player::MoveRight() {
    if (_isLookingUp || _isLookingDown) return;
    
    _dx = globals::PLAYER_WALK_SPEED;
    PlayAnimation("runRight");
    _facing = RIGHT;
}
void Player::MoveJump() {
    if (_isGrounded) {
        _isGrounded = false;
        _dy = globals::JUMP_SPEED;
    } 
}
void Player::MoveStop() {
    _dx = 0;
    if (!_isLookingDown && !_isLookingUp)
        PlayAnimation(_facing == LEFT ? "idleLeft" : "idleRight"); 
}


void Player::LookUp() {
    _isLookingUp = true;
    if (_dx == 0) {
        PlayAnimation( (_facing == LEFT) ? "IdleLeftUp" : "IdleRightUp" );
    }
    else
    {
        PlayAnimation( (_facing == LEFT) ? "RunLeftUp" : "RunRightUp" );
    }
}
void Player::LookDown() {
    _isLookingDown = true;
    if (_isGrounded) {
        PlayAnimation( (_facing == LEFT) ? "IdleLeftBack" : "IdleRightBack");
    }
    else {
        PlayAnimation( (_facing == LEFT) ? "IdleLeftDown" : "IdleRightDown");
    }
}
void Player::StopLookingDown() {
    _isLookingDown = false;
}
void Player::StopLookingUp() {
    _isLookingUp = false;
}

void Player::HandleDoorCollision( std::vector<Door> &doors, Graphics &graphics, Level &level) {
    
    // If isGrounded and arrowDown -> go through the door.
    for (size_t i = 0; i < doors.size(); i++) {

        if( _isGrounded && _isLookingDown ) {
            level = Level(doors[i].GetDestination(), graphics);
            _spriteMapPosition = level.GetPlayerSpawnpoint();
            level.SetPlayer(this);
        }
    }

}

void Player::HandleNpcCollision(std::vector<Npc *> &npc, Graphics &graphics, Input *input, Menu *menu) {

    for (size_t i = 0; i < npc.size(); i++) {

        if ( (input->isKeyHeld(SDL_SCANCODE_DOWN) && _isGrounded) || menu->_isMessageBoxOn ) {

            npc[i]->ShowMessageBox(graphics, input, menu);
        }
    }
}

void Player::HandleEnemyCollision(std::vector<Enemy *> &enemys) {

    for (size_t i = 0; i < enemys.size(); i++) {

        enemys[i]->ImpactOnPlayer(this);
        

        if (_dy > 0 && GetBoundingbox().GetBottom() > enemys[i]->GetBoundingbox().GetTop() ) {
            _dy = globals::JUMP_SPEED;
        }
        else {
            if (!_isDamaged) {
                ChangePlayerHP(enemys[i]->_damageHP);

                MoveJump();
                _dx = ((enemys[i]->GetFacing() == LEFT) ? 1 : -1) * globals::JUMP_SPEED;
                
                _isDamaged = true;
            }
        }
    }
}

void Player::ChangePlayerHP(int hpAmount) {
    if (_currentHealth > 0)
        _currentHealth += hpAmount;
}