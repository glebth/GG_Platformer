#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>

namespace globals {
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;

    const int SCREEN_FPS = 50;
    const int SCREEN_TICKS_PER_FRAME = 1000.0f / SCREEN_FPS;

    const int SPRITE_SCALE = 2;
    const float BOUNDING_BOX_SCALE = 0.75;

    const float PLAYER_WALK_SPEED = 0.35f;
    const int PLAYER_STUN_TIME = 150;
    const float GRAVITY_AX = 0.002f;
    const float MAX_GRAVITY_SPEED = 0.7f;
    const float JUMP_SPEED = -0.55f;

    const float NPC_ANIM_SPEED = 200.0f;


};

namespace sides {
    enum RectSide {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        NONE
    };
}

struct GG_Rectangle
{
private:
    float _width, _height;

public:
    float _x, _y;

    GG_Rectangle (float x = 0, float y = 0, float width = 0, float height = 0) {
        _x = x;
        _y = y;
        _width = width;
        _height = height;
    }
    GG_Rectangle (int x, int y, int width, int height) {
        _x = (float)x;
        _y = (float)y;
        _width = (float)width;
        _height = (float)height;
    }

    float GetTop() const {
        return (_y);
    }
    float GetBottom() const {
        return (_y + _height);
    }
    float GetLeft() const {
        return _x;
    }
    float GetRight() const {
        return (_x + _width);
    }
    float GetCenterX() const {
        return (_x + (_width/2));
    }
    float GetCenterY() const {
        return (_y + (_height / 2));
    }
    float GetWidth() const {
        return _width;
    }
    float GetHeight() const {
        return _height;
    }

    SDL_Rect GetSDLRect() {
        SDL_Rect rect = {(int)this->_x, (int)this->_y, (int)this->_width, (int)this->_height};
        return rect;
    }

    const inline GG_Rectangle GetRectangle() const { return *this; }

    bool IsColliding(const GG_Rectangle &object ) {
        if ( 
            ( ( this->GetTop() >= object.GetBottom() ) || 
            ( this->GetBottom() <= object.GetTop() ) ||
            ( this->GetLeft() >= object.GetRight() ) ||
            ( this->GetRight() <= object.GetLeft() ) ) == false
        ) {
        //A collison detected
            return true;
        }   
        return false;
    }

    GG_Rectangle operator*(int multiplier) {
        return GG_Rectangle(this->_x, this->_y, this->_width * multiplier, this->_height * multiplier);
    }

    GG_Rectangle operator^(int multiplier) {
        return GG_Rectangle(this->_x * multiplier, this->_y * multiplier, this->_width * multiplier, this->_height * multiplier);
    }
};

struct GG_Vector2
{
    float x, y;
    
    GG_Vector2(float new_x = 0, float new_y = 0) {
        x = new_x;
        y = new_y;
    }

    GG_Vector2 GetZeroVector() {
        return GG_Vector2(0,0);
    }


    GG_Vector2 operator*(int multiplier) {
        return GG_Vector2(this->x * multiplier, this->y * multiplier);
    }
};


#endif