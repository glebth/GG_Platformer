#include <SDL_image/include/SDL2/SDL_image.h>

#include "graphics.h"
#include "globals.h"

Graphics::Graphics() :
    _window(NULL),
    _renderer(NULL)
    {
    _window = SDL_CreateWindow("New Gleb Game",  SDL_WINDOWPOS_UNDEFINED,  
            SDL_WINDOWPOS_UNDEFINED, globals::WINDOW_WIDTH, globals::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (_window) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

        if (_renderer) {
            SDL_SetRenderDrawColor(_renderer, 100, 0, 0, 255);//set color of window
	        SDL_RenderClear(_renderer); // draw 
	        SDL_RenderPresent(_renderer); //Update screen and show   
        }
        else
        {
            SDL_Log("Error creating a renderer SDL_Error: %s", SDL_GetError());
        }
    }
    else {
            SDL_Log("Error creating a window SDL_Error: %s", SDL_GetError());
    }

    //LoadTexture("data/PrtCave.png");
}

SDL_Surface* Graphics::LoadSurface(std::string pathSurface) {
        SDL_Surface* loadedSurface = NULL;

        loadedSurface = IMG_Load(pathSurface.c_str());
        //loadedTexture = SDL_CreateTextureFromSurface(_renderer, LoadSurface(pathTexture.c_str()));
        if (loadedSurface == NULL) {
            SDL_Log("Unable to load Surface  IMGError: %s", IMG_GetError());
        }

        return loadedSurface;
}

SDL_Texture* Graphics::LoadTexture(std::string pathTexture) {
    
    if (_loadedSprites.count(pathTexture) == 0) {

        _loadedSprites[pathTexture] = IMG_LoadTexture(_renderer, pathTexture.c_str());

        if (_loadedSprites[pathTexture] == NULL) {
            SDL_Log("Error loading image %s SDL_Error: %s", pathTexture.c_str(), IMG_GetError());
        }
    }
    
    return _loadedSprites[pathTexture];
}

TTF_Font* Graphics::LoadFont(std::string pathFont, int fontSize) {

    if ( _loadedFonts.count(pathFont) == 0 ) {

        _loadedFonts[pathFont] = TTF_OpenFont(pathFont.c_str(), fontSize);

        if (_loadedFonts[pathFont] == NULL) {
            SDL_Log("Error loading font %s SDL_Error: %s", pathFont.c_str(), TTF_GetError());
        }
    }

    return _loadedFonts[pathFont];
}

void Graphics::BlitToWindow(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect) {    
    SDL_RenderCopy(_renderer, texture, srcRect, destRect);    
}

void Graphics::ClearScreen() {
    SDL_RenderClear(_renderer);
}


void Graphics::Flip() {
    SDL_RenderPresent(_renderer);
    
}

SDL_Window* Graphics::GetWindow() const {
    return _window;
}
SDL_Renderer* Graphics::GetRenderer() const {
    return _renderer;
}

void Graphics::FreeGraphics() {

    for(std::map<std::string, SDL_Texture*>::const_iterator it = _loadedSprites.begin(); 
                                                    it != _loadedSprites.end(); ++it) {
        SDL_DestroyTexture(it->second);
    }

    for(std::map<std::string, TTF_Font*>::const_iterator it = _loadedFonts.begin();
                                                    it != _loadedFonts.end(); ++it) {
        TTF_CloseFont(it->second);
    }
    
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = NULL;
    _renderer = NULL;
}

