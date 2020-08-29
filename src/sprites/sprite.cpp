#include "sprites/sprite.h"
#include "graphics.h"
#include "utils/slope.h"


Sprite::Sprite(){};
Sprite::Sprite(Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float boundingBoxScale /*= 1*/,
                bool isColliding /* = true*/) {

    _spriteTextureRect = {(int)xText, (int)yText, (int)wText, (int)hText};

    _spriteMapPosition.x = xMap;
    _spriteMapPosition.y = yMap;
    _spriteOffset = {0, 0};
    
    _dx = 0;
    _dy = 0;

    _bbScale = boundingBoxScale;

    _facing = RIGHT;
    _isGrounded = false;
    _isVisible = true;
    _isColliding = isColliding;

    _spriteBoundingbox = GG_Rectangle(_spriteMapPosition.x, 
                        _spriteMapPosition.y, 
                        wText * _bbScale, 
                        hText * _bbScale) * globals::SPRITE_SCALE;

    _spriteTexture = graphics.LoadTexture(filePath);
}

void Sprite::Draw(Graphics &graphics) {
    
    if (_isVisible) {

        SDL_Rect dst = {(int)(_spriteMapPosition.x - //Чтобы bb был меньше спрайта, но снизу и посередине
                (_spriteTextureRect.w * globals::SPRITE_SCALE * (1.0f - _bbScale) / 2.0f) - 
                (int)_spriteOffset.x ), 
            (int)(_spriteMapPosition.y - 
                (_spriteTextureRect.h * globals::SPRITE_SCALE * (1.0f - _bbScale)) -
                (int)_spriteOffset.y ), 
            (int)_spriteTextureRect.w * globals::SPRITE_SCALE, 
            (int)_spriteTextureRect.h * globals::SPRITE_SCALE};

        graphics.BlitToWindow(_spriteTexture, &_spriteTextureRect, &dst);
    }
}

const sides::RectSide Sprite::GetCollisionSide (GG_Rectangle &otherRectangle) const {
    //Smotrit v kakoi storone bolshe collisia
    int amntRight, amntLeft, amntTop, amntBottom = 0;
    amntBottom = abs(GetBoundingbox().GetBottom() - otherRectangle.GetTop()); //if big then NO collision
    amntLeft = abs(GetBoundingbox().GetLeft() - otherRectangle.GetRight());
    amntRight = abs(GetBoundingbox().GetRight() - otherRectangle.GetLeft());
    amntTop = abs(otherRectangle.GetBottom() - GetBoundingbox().GetTop());

    int amnts[4] = {amntTop, amntRight, amntLeft, amntBottom};
    int minAmnt = amnts[0];
    for (int i = 1; i < 4; i++) {
        if (amnts[i] < minAmnt) {
            minAmnt = amnts[i];
        }
    }
    return
        minAmnt == amntTop ? sides::RectSide::TOP :
        minAmnt == amntRight ? sides::RectSide::RIGHT :
        minAmnt == amntLeft ? sides::RectSide::LEFT :
        minAmnt == amntBottom ? sides::RectSide::BOTTOM :
        sides::RectSide::NONE;
}

void Sprite::HandleCollision(std::vector<GG_Rectangle> &othersRectangles) { 

    if (_isColliding) {
        //Find ride side that collided;
        for (size_t i = 0; i < othersRectangles.size(); i++) {
            sides::RectSide collisionSide = Sprite::GetCollisionSide(othersRectangles[i]);
            //SDL_Log("collision side %i ", Sprite::GetCollisionSide(othersRectangles[i]));
            
            
                switch (collisionSide)
                {
                case sides::RectSide::BOTTOM:
                    this->_spriteMapPosition.y = othersRectangles[i].GetTop() - this->GetBoundingbox().GetHeight();
                    this->_dy = 0;
                    this->_isGrounded = true;
                    break;
                case sides::RectSide::TOP:
                    this->_spriteMapPosition.y = othersRectangles[i].GetBottom();
                    this->_dy = 0;
                    if (_isGrounded) { //esli na slope
                        _dx = 0;
                    }
                    break;
                case sides::RectSide::LEFT:
                    this->_spriteMapPosition.x = othersRectangles[i].GetRight();// + 1;
                    //this->_dx = 0;
                    break;
                case sides::RectSide::RIGHT:
                    this->_spriteMapPosition.x = othersRectangles[i].GetLeft() - this->GetBoundingbox().GetWidth();// - 1;
                    //this->_dx = 0;
                    break;                          
                default:
                    break;
                }

        }
    }
}

void Sprite::HandleFall() {
    if (_dy >= 0.1f && _isGrounded) {
        _isGrounded = false;
    }
}

void Sprite::HandleSlopeCollision(std::vector<Slope> &slopes) {

    if (_isColliding) {
        for (size_t i = 0; i < slopes.size(); i++) {
            //Where on the slope Sprite bottom center will be
            //slope is y = kx + b
            //k is _slope(tangens), b is y intercect(when x==0)

            float newY = slopes[i].GetSlope() * _spriteBoundingbox.GetCenterX() + slopes[i].GetB();

            if (_isGrounded) {//Uzhe na slope
                _spriteMapPosition.y = newY - _spriteBoundingbox.GetHeight();
                _dy = 0; 
            } 
            else if (_dy >= 0){//Padaet no uzhe v boundBox of slope
                _isGrounded = true;
            }
        }
    }
}

void Sprite::Update(float time, GG_Vector2 offset /* = {0, 0} */ ) {

    SetOffset(offset);
    
    if (_isColliding) {
        _spriteMapPosition.x += _dx * time;

        if (!_isGrounded || _dy < globals::MAX_GRAVITY_SPEED) {
            _dy += globals::GRAVITY_AX * time;
        }
        //SDL_Log("isGrounded %i _dy = %f", _isGrounded, _dy);
        _spriteMapPosition.y += _dy * time;

        _spriteBoundingbox._x = _spriteMapPosition.x;
        _spriteBoundingbox._y = _spriteMapPosition.y;
        //SDL_Log("BBy: %f", _spriteBoundingbox.GetCenterY());
    }
}

void Sprite::SetOffset(GG_Vector2 offset) {
    _spriteOffset = offset;
}

void Sprite::SetTextureRectX(int valX) {
    _spriteTextureRect.x = valX; 
}

void Sprite::SetTextureRectY(int valY) {
    _spriteTextureRect.x = valY;
}

void Sprite::SetTextureRectW(int valW) {
    _spriteTextureRect.w = valW;
}

void Sprite::SetTextureRectH(int valH) {
    _spriteTextureRect.h = valH;
}