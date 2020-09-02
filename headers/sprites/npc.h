#ifndef NPC_H
#define NPC_H

#include "sprites/animatedsprite.h"

class Graphics;
class Input;
class Menu;

class Npc : public AnimatedSprite {
public:
    Npc(){};
    Npc(std::string name, std::string description, Graphics &graphics, std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale = 1,
                bool isCollides  = true,
                std::string animName = "idleLeft");
    
    bool IsColliding(const GG_Rectangle &otherRect);

    void ShowMessageBox(Graphics &graphics, Input *input, Menu *menu);

    const inline std::string GetNpcName() const { return _npcName; }
    const inline std::string GetNpcDescription() const { return _npcDescription; }
private:
    std::string _npcName;
    std::string _npcDescription;

    void AnimationDone(std::string currentAnimation);
    void SetupAnimations();
};

#endif