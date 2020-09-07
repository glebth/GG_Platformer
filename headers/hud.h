#ifndef HUD_H
#define HUD_H

#include "sprites/sprite.h"

class Graphics;
class Player;

class Hud {
public:
    Hud();
    Hud(Graphics &graphics, Player *player);
    void Update(float elapsedTime);
    void Draw(Graphics &graphics);

private:
    Player *_player;

    Sprite _healthDigit1;
    Sprite _healthBar;
    Sprite _currentHealthBar;

    Sprite _lvWord;
    Sprite _lvNumber;

};

#endif