#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <SDL2/SDL_ttf.h>

class Graphics {
public:
    Graphics();

    //Return one SDL_Texture * or NULL 
    SDL_Texture* LoadTexture(std::string pathTexture);

    //Return one SDL_Surface * or NULL 
    SDL_Surface* LoadSurface(std::string pathSurface);

    TTF_Font* LoadFont(std::string pathFont, int fontSize);

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetRenderer() const;

/*
    Set _window and _renderer to NULL
    Used before closing program
*/
    void FreeGraphics();

/*
    Draw(Show) what was blitted before

    See when you draw to the screen, you are not typically drawing to the image on the 
    screen you see. By default, most rendering systems out there are double buffered. 
    These two buffers are the front and back buffer.

When you make draw calls like SDL_BlitSurface, you render to the back buffer. 
What you see on the screen is the front buffer. The reason we do this is because most frames 
require drawing multiple objects to the screen. If we only had a front buffer, we would be 
able to see the frame as things are being drawn to it which means we would see unfinished frames. 
So what we do is draw everything to the back buffer first and once we're done we swap 
the back and front buffer so now the user can see the finished frame.
*/
    void Flip();

/*
    BlitSurface to the main window surface
*/
    void BlitToWindow(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);

/*
    Clear _renderer before Flipping
*/
    void ClearScreen();


private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    
    std::map<std::string, SDL_Texture*> _loadedSprites;
    std::map<std::string, TTF_Font*> _loadedFonts;


};

#endif