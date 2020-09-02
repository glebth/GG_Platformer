#include "menu.h"
#include "graphics.h"

std::string GAME_MENU_FONT = "data/data-unifon.ttf";
const int GAME_MENU_FONT_SIZE = 50;

std::string MESSAGE_BOX_FONT = "data/rusFont.ttf";
const int MESSAGE_BOX_FONT_SIZE = 25;
const float MESSAGE_BOX_SCALE = 0.9f;
const int MESSAGE_BOX_X = (int) (globals::WINDOW_WIDTH / 2.0);
const int MESSAGE_BOX_Y = (int) (globals::WINDOW_HEIGHT / 3.0);
const int PARAGRAPH_LINES_COUNT = 4;



Menu::Menu(Graphics &graphics) {

    _isMenuOn = false;
    _isMessageBoxOn = false;
    _messagePart = 0;

    _menuFont = graphics.LoadFont(GAME_MENU_FONT, GAME_MENU_FONT_SIZE);
    _textBoxFont = graphics.LoadFont(MESSAGE_BOX_FONT, MESSAGE_BOX_FONT_SIZE);

    _menuBackground = graphics.LoadTexture( menuBtns::MENU_BUTTON_FILE );

    SetupButtons(graphics);
}

void Menu::Draw(Graphics &graphics) {

    //Draw background texxture
    SDL_Rect srcBg = { 3, 3, 238, 18};
    SDL_Rect dstBg = {0, 0, globals::WINDOW_WIDTH, globals::WINDOW_HEIGHT};
    
    SDL_SetTextureAlphaMod( _menuBackground, 120 );
    graphics.BlitToWindow( _menuBackground, &srcBg, &dstBg);
    SDL_SetTextureAlphaMod( _menuBackground, 255);

    //Draw buttons
    for (size_t i = 0; i < _buttons.size(); i++ ) {

        SDL_Rect srcB = {0, 0, 244, 24}; // Ramka vokrug menu

        SDL_Rect srcT = {0, 0, (int)_buttons[i].buttonRect.GetWidth(), (int)_buttons[i].buttonRect.GetHeight()};
        SDL_Rect dstT = _buttons[i].buttonRect.GetSDLRect();

        if ( i != _currentButtonNum ) {

            SDL_SetTextureColorMod(_buttons[i].textTexture, 150, 150, 150);

            graphics.BlitToWindow(_buttons[i].buttonTexture, &srcB, &dstT);
            graphics.BlitToWindow(_buttons[i].textTexture, &srcT, &dstT);

            SDL_SetTextureColorMod(_buttons[i].textTexture, _buttons[i].buttonColor.r,
                _buttons[i].buttonColor.g, _buttons[i].buttonColor.b);

            continue;
        }

        graphics.BlitToWindow(_buttons[i].buttonTexture, &srcB, &dstT);
        graphics.BlitToWindow(_buttons[i].textTexture, &srcT, &dstT);
    }
}

void Menu::ShowMessage(Graphics &graphics, std::string message, 
    SDL_Color color /* = {255,255,255}*/, SDL_Surface *textSurface /* = NULL*/) {

    if (textSurface == NULL) //esli ne predzagruzhena iz ShowNExt
        textSurface = TTF_RenderUTF8_Blended_Wrapped(_textBoxFont, message.c_str(), 
            color, (Uint32) (globals::WINDOW_WIDTH / 2) );

    if( textSurface == NULL ) {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else {

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(graphics.GetRenderer(), textSurface);

        if ( textTexture == NULL ) {
            SDL_Log( "Unable to create texture from rendered text! SDL Error: %s", SDL_GetError() );
        }
        else {

            int tWidth = textSurface->w;
            int tHeight = textSurface->h;

            //Bg message box
            SDL_Rect srcBg = { 3, 3, 238, 18};

            int fontLineSkip = TTF_FontLineSkip(_textBoxFont);
            int paragraphOnScreen = fontLineSkip * PARAGRAPH_LINES_COUNT;
            int bgBoxHeight = tHeight + fontLineSkip;
            
            

            if (tHeight > paragraphOnScreen) { //esli text slishkom dlinnii

                bgBoxHeight = fontLineSkip + paragraphOnScreen;
                int deltaLines = tHeight - _messagePart * paragraphOnScreen;

                if (deltaLines > paragraphOnScreen) {
                    tHeight = paragraphOnScreen;
                }
                else {
                    tHeight = deltaLines;
                }
            }
                
            SDL_Rect dstBg = {
                MESSAGE_BOX_X, 
                MESSAGE_BOX_Y, 
                tWidth, 
                bgBoxHeight};
            graphics.BlitToWindow(_menuBackground, &srcBg, &dstBg);

            //Text texture
            SDL_Rect srcText = {0, paragraphOnScreen * _messagePart, tWidth, tHeight};
            SDL_Rect dstText = {
                (int) (MESSAGE_BOX_X / MESSAGE_BOX_SCALE), 
                (int) (MESSAGE_BOX_Y / MESSAGE_BOX_SCALE),
                (int) (tWidth * MESSAGE_BOX_SCALE), 
                (int) (tHeight * MESSAGE_BOX_SCALE)};
            graphics.BlitToWindow(textTexture, &srcText, &dstText);   

            _isMessageBoxOn = true;
        }
    }

    SDL_FreeSurface(textSurface);
}

void Menu::ShowNextPartMessage(Graphics &graphics, std::string message, 
    SDL_Color color /*= {255, 255, 255}*/) {

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(_textBoxFont, message.c_str(), 
        color, (Uint32) (globals::WINDOW_WIDTH / 2) );

    if( textSurface == NULL ) {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else {
        int tHeight = textSurface->h;
        int fontLineSkip = TTF_FontLineSkip(_textBoxFont);       

        int messagePartsAll = std::ceil<int>( tHeight / (fontLineSkip * PARAGRAPH_LINES_COUNT) );

        _messagePart++;
        if (_messagePart <= messagePartsAll) {
            ShowMessage(graphics, message, color);
        }
        else {
            _messagePart = 0;
            _isMessageBoxOn = false;
            SDL_FreeSurface(textSurface);
        };
    }
}

void Menu::SetupButtons(Graphics &graphics) {

    LoadButton(graphics, "Start", globals::WINDOW_WIDTH / 2, globals::WINDOW_HEIGHT * 1 / 4);
    LoadButton(graphics, "Exit", globals::WINDOW_WIDTH / 2, globals::WINDOW_HEIGHT * 2 / 4);

    _currentButtonNum = menuBtns::MENU_START;
}

void Menu::LoadButton(Graphics &graphics, std::string buttonText, int x, 
                        int y, SDL_Color color /* = {255,255,255}*/) {

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid( _menuFont, buttonText.c_str(), color );

    if( textSurface == NULL ) {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else {

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface (graphics.GetRenderer(), textSurface);

        if ( textTexture == NULL ) {
            SDL_Log( "Unable to create texture from rendered text! SDL Error: %s", SDL_GetError() );
        }
        else {

            int tWidth = textSurface->w;
            int tHeight = textSurface->h;

            GG_Rectangle btnRect = {x - (tWidth / 2), y, tWidth, tHeight}; // chtobi poseredke 

            SDL_Texture* btnTexture = graphics.LoadTexture( (menuBtns::MENU_BUTTON_FILE).c_str() );

            MenuButton newButton = {btnTexture, textTexture, buttonText, btnRect, color};

            _buttons.push_back(newButton);
        }
        
    }
    SDL_FreeSurface(textSurface);
}

void Menu::MoveDownButton() {
    
    if (_currentButtonNum != _buttons.size() - 1) {
        _currentButtonNum++;
    }
    else {
        _currentButtonNum = 0;
    }

}

void Menu::MoveUpButton() {
    
    if (_currentButtonNum > 0) {
        _currentButtonNum--;
    }
    else {
        _currentButtonNum = _buttons.size() - 1;
    }

}

const int Menu::EnterButton() {

    return _currentButtonNum;

}

void Menu::FreeMenu() {

    for (size_t i = 0; i < _buttons.size(); i++) {

        SDL_DestroyTexture(_buttons[i].buttonTexture);

    }

}