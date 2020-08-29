#include "input.h"

/*
    Handles all inputs in da game
*/

/*
    Handle vse input pered nachalom kadra
    reset keys that are no longer relevant
*/
void Input::BeginNewFrame() {

    _pressedKeys.clear();
    _releasedKeys.clear();
}

/*
    Vnosit nazhatie v map _pressed when Event happens
*/
void Input::KeyDownEvent(const SDL_Event& event) {

    _pressedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = true;
}

//When key released
void Input::KeyUpEvent(const SDL_Event& event) {
    _releasedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = false;
}

//Check if key was pressed IN THIS FRAME
bool Input::WasKeyPressed(SDL_Scancode key) {

    return _pressedKeys[key];
}

//Check if key was released IN THIS FRAME
bool Input::WasKeyReleased(SDL_Scancode key) {

    return _releasedKeys[key];
}

//Check if key is being held IN THIS FRAME
bool Input::isKeyHeld(SDL_Scancode key) {

    return _heldKeys[key];
}