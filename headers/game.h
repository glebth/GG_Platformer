#ifndef GAME_H
#define GAME_H

#include "sprites/player.h"
#include "level.h"
#include "hud.h"

class Input;
class Menu;
class Graphics;

class Game {
public:
    Game();

private:
    void MainGameloop();

    void Draw(Graphics &graphics);

    void Update(float elapsedTime);
    int FpsControl(int lastUpdate);

    void QuitGame(Graphics &graphics);
    void ClearGame(Graphics &graphics);

    Input* _input;

    Level* _level;

    Player* _player;

    Hud _hud;

    Menu* _menu;
};

#endif