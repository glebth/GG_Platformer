#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>

#include "globals.h"


namespace menuBtns {

    const int MENU_START = 0;
    const int MENU_EXIT = 1;
    const int MENU_RESTART = 2;

    const std::string MENU_BUTTON_FILE = "data/sprites/TextBox.png";
    const std::string MESSAGE_BOX_FILE = "data/sprites/TextBox.png";
    
};


class Graphics;
struct MenuButton;

class Menu {
public:
    Menu() {};
    Menu(Graphics &graphics);
    void Update(float elapsedTime);

    void Draw(Graphics &graphics);

    //Pokazat odno soobshenie iz odnoi chasti
    void ShowMessage(Graphics &graphics, std::string message, 
        SDL_Color color = {255, 255, 255}, SDL_Surface *textSurface = NULL);

    //Pokazaviat bolshoe soobshenie po chastyam
    void ShowNextPartMessage(Graphics &graphics, std::string message, 
        SDL_Color color = {255, 255, 255});

    void MoveDownButton();
    void MoveUpButton();
    const int EnterButton();

    void FreeMenu();

    bool _isMenuOn;
    bool _isMessageBoxOn;
    int _messagePart;
private:
    std::vector<MenuButton> _mainMenuButtons;
    size_t _currentButtonNum;

    TTF_Font* _menuFont;
    TTF_Font* _textBoxFont;

    SDL_Texture* _menuBackground;

    void LoadButton(Graphics &graphics, std::string buttonText, 
                    int x, int y, SDL_Color color = {255, 255, 255});
    void SetupButtons(Graphics &graphics);
};

struct MenuButton {
    SDL_Texture* buttonTexture;
    SDL_Texture* textTexture;
    std::string buttonText;
    GG_Rectangle buttonRect;
    SDL_Color buttonColor;
};

#endif