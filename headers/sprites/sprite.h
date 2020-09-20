#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "globals.h"


enum SpriteDir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Graphics;
class Slope;

class Sprite {
public:
    Sprite();
    Sprite(Graphics &graphics, std::string filePath, float xText, float yText, 
        float wText, float hText, 
        float xMap, float yMap,
        float boundingBoxScale = 1.0f,
        bool isColliding = true);
    
    virtual ~Sprite() {;}

    SDL_Texture* GetSpriteTexture() const {
        return _spriteTexture;
    }

    virtual void Update(float time, GG_Vector2 offset = {0, 0});

    virtual void Draw(Graphics &graphics);

    virtual inline void SetVisible(bool visibility) { _isVisible = visibility; }

    virtual void HandleCollision(std::vector<GG_Rectangle> &object);
    virtual void HandleSlopeCollision(std::vector<Slope> &slopes);
    virtual void HandleFall();

    virtual SpriteDir GetFacing() const { return _facing; }

    virtual const GG_Rectangle GetBoundingbox() const { return _spriteBoundingbox; }

    virtual const sides::RectSide GetCollisionSide(GG_Rectangle &otherRectangle) const;

    virtual void SetTextureRectX(int valX);
    virtual void SetTextureRectY(int valY);
    virtual void SetTextureRectW(int valW);
    virtual void SetTextureRectH(int valH);

    virtual void SetOffset(GG_Vector2 offset);

protected:
    
    GG_Vector2 _spriteMapPosition;
    GG_Vector2 _spriteOffset;
    
    float _dx, _dy;

    SDL_Texture* _spriteTexture;

    GG_Rectangle _spriteBoundingbox;

    SDL_Rect _spriteTextureRect;

    SpriteDir _facing;

    float _bbScale;

    bool _isGrounded;

    bool _isVisible;

    bool _isColliding;

    bool _isAlive;

    
};

#endif