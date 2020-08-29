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
                bool isCollides /* = true */) :
    AnimatedSprite(graphics, filePath, xText, yText, wText, hText, 
                    xMap, yMap, animUpdate, boundingBoxScale, isCollides),
    _npcName(name),
    _npcDescription(description)
{
    _currentAnimationName = "";

    _facing = facing;

    SetupAnimations();
    PlayAnimation("idleLeft");
}

//NADO BUDET DELAT SPRITEMAPы все одного типа : расположение анимаций на схожих координатах
void Npc::SetupAnimations() {

    LoadAnimations(1, 3, 2, "idleLeft");

    std::vector<int> frames = {0, 1, 0, 2};
    LoadAnimations(frames, 3, 2, "talkLeft");
}

void Npc::ShowDescriptionMB(Graphics &graphics, Input *input, Menu *menu) {

    bool isToPlayAnimation = false;

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
    SetVisible(true);
}