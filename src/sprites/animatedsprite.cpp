#include "sprites/animatedsprite.h"
#include "graphics.h"

AnimatedSprite::AnimatedSprite(Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate,
                float boundingBoxScale /* = 1*/,
                bool isCollides /* = true */) : 
Sprite::Sprite(graphics, filePath, xText, yText, wText, hText, xMap, yMap, boundingBoxScale, isCollides)
{
    _animations.clear();
    _isToPlayCurrentAnimationOnce = false;

    _frameIndex = 0;

    _elapsedTime = 0;
    _animationFrameTime = animUpdate;

}

void AnimatedSprite::LoadAnimations(int numbOfFrames, int xTextFirst, int yTextFirst, const char* animationName) {

    std::vector<SDL_Rect> frms;

    for (int i = 0; i < numbOfFrames; i++) {
        SDL_Rect frm =  {
            xTextFirst + i * _spriteTextureRect.w, 
            yTextFirst, 
            _spriteTextureRect.w, 
            _spriteTextureRect.h
        };

        frms.push_back(frm);
    };

    std::string name(animationName);

    _animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, frms));
}
void AnimatedSprite::LoadAnimations(std::vector<int> &framesNum, int xTextFirst, int yTextFirst, 
    const char* animationName, int frameMargin /*= 0*/) {

    std::vector<SDL_Rect> frms;

    for (size_t i = 0; i < framesNum.size(); i++) {
        SDL_Rect frm = {
            xTextFirst + framesNum[i] * (frameMargin + _spriteTextureRect.w),
            yTextFirst,
            _spriteTextureRect.w,
            _spriteTextureRect.h
        };

        frms.push_back(frm);
    }

    std::string name(animationName);

    _animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, frms));
}

void AnimatedSprite::Update( float time, GG_Vector2 offset /* = {0, 0} */) {

    Sprite::Update(time, offset);

    _elapsedTime += time;
    if ( _elapsedTime > _animationFrameTime) {

        _elapsedTime -= _animationFrameTime;

        if ( _frameIndex < _animations[_currentAnimationName].size() - 1) {
            _frameIndex++;
        }
        else {

            if (_isToPlayCurrentAnimationOnce) {
                SetVisible(false);
            }

            StopAnimation();
        }
    }
}

void AnimatedSprite::Draw( Graphics &graphics ) {

    if (_isVisible) {

        SDL_Rect src = _animations[_currentAnimationName][_frameIndex];
        
        SDL_Rect dst = {(int)(_spriteMapPosition.x - //Чтобы bb был меньше спрайта, но снизу и посередине
                (src.w * globals::SPRITE_SCALE * (1.0f - _bbScale) / 2.0f) - 
                (int)_spriteOffset.x ), 
            (int)(_spriteMapPosition.y - 
                (src.h * globals::SPRITE_SCALE * (1.0f - _bbScale)) - 
                (int)_spriteOffset.y ), 
            (int)src.w * globals::SPRITE_SCALE, 
            (int)src.h * globals::SPRITE_SCALE};

        graphics.BlitToWindow(_spriteTexture, &src, &dst);
    }
}

void AnimatedSprite::ResetAnimations() {
    _animations.clear();
}

void AnimatedSprite::StopAnimation() {
    _frameIndex = 0;
    AnimationDone(_currentAnimationName);
}

void AnimatedSprite::PlayAnimation( const char* animationName, bool isToPlayOnce /* = false*/ ) {

    std::string animName(animationName);

    _isToPlayCurrentAnimationOnce = isToPlayOnce;

    if (_currentAnimationName != animName ) {
        _currentAnimationName = animName;
        _frameIndex = 0;
    }
}