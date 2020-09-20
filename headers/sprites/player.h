#ifndef PLAYER_H
#define PLAYER_H

#include "sprites/animatedsprite.h"
#include "sprites/enemy.h"
#include "utils/door.h"
#include "npc.h"

class Input;
class Level;
class Menu;

class Player : public AnimatedSprite {
public:
    Player() {};
    Player(Graphics &graphics, const float xMap, const float yMap);

    void MoveRight();
    void MoveLeft();
    void MoveStop();
    void MoveJump();

    void LookUp();
    void StopLookingUp();
    void LookDown();
    void StopLookingDown();

    void Draw(Graphics &graphics);
    void Update(float time, GG_Vector2 offset = {0, 0});

    void HandleDoorCollision(std::vector<Door> &doors, Graphics &graphics, Level *level, Menu *menu);
    void HandleNpcCollision(std::vector<Npc *> &npc, Graphics &graphics, Input *input, Menu *menu);
    void HandleEnemyCollision(std::vector<Enemy *> &enemys);

    const inline int GetCurrentHealth() const { return _currentHealth; }
    const inline int GetMaxHealth() const { return _maxHealth; }

    const inline float GetX() const { return _spriteBoundingbox.GetCenterX(); }
    const inline float GetY() const { return _spriteBoundingbox.GetCenterY(); }

    const inline bool IsDamaged() const { return _isDamaged; }
    const inline void DamagedStateReverse() { _isDamaged = !_isDamaged; _dx = 0; }
    const bool IsAlive() const { return _isAlive; }

    void ChangePlayerHP(int hpAmount);
private:
    void AnimationDone(std::string currentAnimation);
    void SetupAnimations();

    bool _isLookingUp;
    bool _isLookingDown;

    bool _isDamaged;

    int _currentHealth;
    int _maxHealth;

    int _damageHP;
};

#endif