#include "sprites/npc.h"
#include "menu.h"
#include "input.h"



Npc::Npc(std::string name, std::string description, 
        Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale /* = 1*/,
                bool isCollides /* = true */,
                std::string animName /*= "idleLeft"*/) :
    AnimatedSprite(graphics, filePath, xText, yText, wText, hText, 
                    xMap, yMap, animUpdate, boundingBoxScale, isCollides),
    _npcName(name),
    _npcDescription(description)
{
    _currentAnimationName = animName;

    _facing = facing;

    SetupAnimations();
    //PlayAnimation("idleLeft");
}

void Npc::SetupAnimations() {

    // LoadAnimations(1, 4, 0, "idleLeft");

    // std::vector<int> frames = {0, 1, 0, 1};
    // LoadAnimations(frames, 4, 0, "talkLeft", 7);
}

void Npc::ShowMessageBox(Graphics &graphics, Input *input, Menu *menu) {

    bool isToPlayAnimation = false;

    if ( input->WasKeyPressed(SDL_SCANCODE_ESCAPE) ) {
        menu->_isMessageBoxOn = false;
        menu->_messagePart = 0;
    }

    //Show Description part.
    if ( menu->_isMessageBoxOn && input->WasKeyPressed(SDL_SCANCODE_X) ) {
        menu->ShowNextPartMessage(graphics, GetNpcDescription());
        isToPlayAnimation = true;
    }
    else if ( menu->_isMessageBoxOn ) {
        menu->ShowMessage(graphics, GetNpcDescription());
    }
    else if ( input->WasKeyPressed(SDL_SCANCODE_X) && !menu->_isMessageBoxOn ) {
        menu->ShowMessage(graphics, GetNpcDescription());
        isToPlayAnimation = true;
    }

    // Show Animation part.
    if (isToPlayAnimation && menu->_isMessageBoxOn) {
        PlayAnimation( ( _facing == LEFT ) ? "talkLeft" : "talkRight");
    }
}

bool Npc::IsColliding(const GG_Rectangle &otherRect) {

    GG_Rectangle bb = Sprite::GetBoundingbox();
    
    return bb.IsColliding(otherRect);

}

void Npc::AnimationDone(std::string currentAnimation) {

    if (_facing == LEFT) { 

        PlayAnimation("idleLeft");
    }
    else {
        PlayAnimation("idleRight");
    }
    SetVisible(true);
}