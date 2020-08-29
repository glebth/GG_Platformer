#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <map>

class Input
{
private:
    std::map<SDL_Scancode, bool> _heldKeys;
    std::map<SDL_Scancode, bool> _pressedKeys;
    std::map<SDL_Scancode, bool> _releasedKeys;
public:
    void BeginNewFrame();
    void KeyUpEvent(const SDL_Event&);
    void KeyDownEvent(const SDL_Event&);
    
    bool WasKeyPressed(SDL_Scancode);
    bool WasKeyReleased(SDL_Scancode);
    bool isKeyHeld(SDL_Scancode);
};

#endif