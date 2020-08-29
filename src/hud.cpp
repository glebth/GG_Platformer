#include "hud.h"
#include "graphics.h"

Hud::Hud() {;}
Hud::Hud(Graphics &graphics, Player &player) {
    _player = player;

    _healthBar = Sprite(graphics, "data/sprites/TextBox.png", 0, 40, 64, 8, 35, 70, 1.0f, false);
    _currentHealthBar = Sprite(graphics, "data/sprites/TextBox.png", 0, 25, 39, 5, 83, 72, 1 , false);
    _healthDigit1 = Sprite(graphics, "data/sprites/TextBox.png", 0, 56, 8, 8, 66, 70, 1.0f, false);
    _lvWord = Sprite(graphics, "data/sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
    _lvNumber = Sprite(graphics, "data/sprites/TextBox.png", 0, 56, 8, 8, 66, 52);

}

void Hud::Update(float elapsedTime) {
    _healthDigit1.SetTextureRectX( 8 * _player.GetCurrentHealth() ); // *8 чтобы из текстуры прав взять

    //39px -- 100%
    float hd = (float)_player.GetCurrentHealth() / (float)_player.GetMaxHealth();
    _currentHealthBar.SetTextureRectW(std::floor(hd * 39)); //39 - dlina Bara polnogo
}

void Hud::Draw(Graphics &graphics) {

    _healthBar.Draw(graphics);
    _currentHealthBar.Draw(graphics);
    _healthDigit1.Draw(graphics);
    _lvWord.Draw(graphics);
    _lvNumber.Draw(graphics);
}