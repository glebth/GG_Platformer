#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <vector>
#include <map>

#include "sprites/sprite.h"

class Graphics;

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite() {};
    AnimatedSprite(Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, float frmTime,
                float boundingBoxScale = 1,
                bool isColliding = true);
    virtual ~AnimatedSprite() {};

    virtual void Update(float elapsedTime, GG_Vector2 offset = {0, 0});

    virtual void Draw(Graphics &graphics);

    virtual void LoadAnimations(int numbOfFrames, 
        int xTextFirst, int yTextFirst, 
        const char* animationName);
    virtual void LoadAnimations(std::vector<int> &framesNum, 
        int xTextFirst, int yTextFirst, 
        const char* animationName, int frameMargin = 0);

protected:

    // Performs logic when animation is Done (finished) every time.
    virtual void AnimationDone(std::string currentAnimation) = 0;
    virtual void SetupAnimations() = 0;

    virtual void StopAnimation();

    std::string _currentAnimationName;
    virtual void PlayAnimation( const char* animationName, bool isToPlayOnce = false);

private:

    size_t _frameIndex;
    float _elapsedTime;
    float _animationFrameTime;
    
    std::map<std::string, std::vector<SDL_Rect>> _animations;
    bool _isToPlayCurrentAnimationOnce;
   
    void ResetAnimations();

};

#endif