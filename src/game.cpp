#include <SDL_image/include/SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.h"
#include "game.h"
#include "gg_timer.h"
#include "input.h"
#include "menu.h"


Game::Game() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL was not initialized! SDL Error: %s", SDL_GetError());
        return;
    }
    else
    {
        if(!IMG_Init(IMG_INIT_PNG)) {
            SDL_Log("SDL_Image was not initialized! SDL_image error: %s", IMG_GetError());
            return;
        }
        if(TTF_Init() == -1) {
            SDL_Log("SDL ttf error: %s", TTF_GetError());
            return;
        }
    }

    MainGameloop();
}

void Game::MainGameloop() {

    Graphics _gameGraphics;
    SDL_Event event;

    _input = new Input();

    _level = Level("data/maps/map2.tmx", _gameGraphics);
    _player = Player(_gameGraphics, 
        _level.GetPlayerSpawnpoint().x, 
        _level.GetPlayerSpawnpoint().y);
    _level.SetPlayer(&_player);
    
    _hud = Hud(_gameGraphics, &_player);
    _menu = new Menu(_gameGraphics);

    bool isQuit = false;

    // Ticks per frame controller
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    while (!isQuit)
    {
        _input->BeginNewFrame();

        if (SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                //key presssed
                if(event.key.repeat == 0) {
                    _input->KeyDownEvent(event);
                }
            }

            if (event.type == SDL_KEYUP) {
                //Some key was released
                _input->KeyUpEvent(event);
            }
        }

        if( _input->WasKeyPressed(SDL_SCANCODE_ESCAPE) && !_menu->_isMessageBoxOn ) {
            _menu->_isMenuOn = !_menu->_isMenuOn;
        }
        if (event.type == SDL_QUIT) {
            QuitGame(_gameGraphics);
            isQuit = true;
            continue;
        }

        if ( !_menu->_isMenuOn && !_menu->_isMessageBoxOn )
        {

            if (_input->isKeyHeld(SDL_SCANCODE_LEFT)) {
                _player.MoveLeft();
            }
            else if (_input->isKeyHeld(SDL_SCANCODE_RIGHT)) {
                _player.MoveRight();
            }
            else if ((!_input->isKeyHeld(SDL_SCANCODE_RIGHT) && (!_input->isKeyHeld(SDL_SCANCODE_LEFT))))
            {
                _player.MoveStop();
                if (_input->isKeyHeld(SDL_SCANCODE_DOWN)) {
                    _player.LookDown();
                }
            }
            
            if (_input->WasKeyPressed(SDL_SCANCODE_SPACE)) {
                _player.MoveJump();
            }

            if (_input->isKeyHeld(SDL_SCANCODE_UP)) {
                _player.LookUp();
            }
            if ( _input->WasKeyReleased(SDL_SCANCODE_UP) || !_input->isKeyHeld(SDL_SCANCODE_UP) ) {
                _player.StopLookingUp();
            }
            
            if ( _input->WasKeyReleased(SDL_SCANCODE_DOWN) || !_input->isKeyHeld(SDL_SCANCODE_DOWN) ) {
                _player.StopLookingDown();
            }
        
        }
        else {

            if (_menu->_isMenuOn) {
                if (_input->WasKeyPressed(SDL_SCANCODE_DOWN)) {
                    _menu->MoveDownButton();
                }
                if (_input->WasKeyPressed(SDL_SCANCODE_UP)) {
                    _menu->MoveUpButton();
                }

                if ( _input->WasKeyPressed(SDL_SCANCODE_RETURN) || _input->WasKeyPressed(SDL_SCANCODE_SPACE) ) {

                    if ( _menu->EnterButton() == menuBtns::MENU_EXIT ) {
                        QuitGame(_gameGraphics);
                        isQuit = true;
                        continue;
                    }
                    if ( _menu->EnterButton() == menuBtns::MENU_START ) {
                        _menu->_isMenuOn = false;
                    }
                    if ( _menu->EnterButton() == menuBtns::MENU_RESTART ) {
                        ClearGame(_gameGraphics);
                        MainGameloop();
                        return;
                    }

                }
            }
            else if (_menu->_isMessageBoxOn) {

                if ( _input->WasKeyPressed(SDL_SCANCODE_X) ) {
                    //_menu->_isMessageBoxOn = false;
                }
            }
        }
        
        //Update and fps control
        int ELAPSED_TIME = FpsControl(LAST_UPDATE_TIME);
        Update(ELAPSED_TIME);
        //---Update

        //Drawing---
        Draw(_gameGraphics);
        //---Drawing

        LAST_UPDATE_TIME = SDL_GetTicks();
    }
}

void Game::Update(float elapsedTime) {

    _level.Update(globals::SCREEN_TICKS_PER_FRAME);

    if (!_menu->_isMenuOn && !_menu->_isMessageBoxOn) {
        _player.Update(globals::SCREEN_TICKS_PER_FRAME, _level.GetOffset());
        _hud.Update(globals::SCREEN_TICKS_PER_FRAME);
    }
}

void Game::Draw(Graphics &graphics) {
    graphics.ClearScreen();

    if ( !_menu->_isMenuOn && !_menu->_isMessageBoxOn ) {

        //Collisions--
        std::vector<GG_Rectangle> collisionRectangles;
        if ((collisionRectangles = _level.CollidedRects(_player.GetBoundingbox())).size() > 0) {
            _player.HandleCollision(collisionRectangles);    
        }
        collisionRectangles.clear();


        std::vector<Slope> collisionSlopes;
        if ( (collisionSlopes = _level.CollidedSlopes(_player.GetBoundingbox())).size() > 0 ) {
            _player.HandleSlopeCollision(collisionSlopes);
        }
        /*
        else if (collisionRectangles.size() <= 0) {
            _player.HandleFall();
        } */ //Check if its falling and disable Jump 

        

        std::vector<Door> collidedDoors;
        if ( (collidedDoors = _level.CollidedDoors(_player.GetBoundingbox())).size() > 0 ) {
            _player.HandleDoorCollision(collidedDoors, graphics, _level);
        }
        //--Collisions
    }

    _level.Draw(graphics);

    _player.Draw(graphics);

    _hud.Draw(graphics);
    
    if ( _menu->_isMenuOn ) {
        _menu->Draw(graphics);
    }
    else {
        std::vector<Npc *> collidedNpc;
        if ( (collidedNpc = _level.CollidedNpc(_player.GetBoundingbox())).size() > 0 ) {
            _player.HandleNpcCollision(collidedNpc, graphics, _input, _menu);
        }

        std::vector<Enemy *> collidedEnemies;
        if ( (collidedEnemies = _level.CollidedEnemies(_player.GetBoundingbox())).size() > 0 ) {
            _player.HandleEnemyCollision(collidedEnemies);
        }
    }

    graphics.Flip();
}

void Game::ClearGame(Graphics &graphics) 
// Called when restarting Game
{
    delete _input;
    
    _menu->FreeMenu();
    delete _menu;

    graphics.FreeGraphics();
}

void Game::QuitGame(Graphics &graphics) {
    
    ClearGame(graphics);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int Game::FpsControl(int lastUpdate) {

    const int CURRENT_TIME = SDL_GetTicks();
    int ELAPSED_TIME = CURRENT_TIME - lastUpdate;

    if (ELAPSED_TIME < globals::SCREEN_TICKS_PER_FRAME) {
        SDL_Delay((Uint32)(globals::SCREEN_TICKS_PER_FRAME - ELAPSED_TIME));
    }

    return ELAPSED_TIME;
}