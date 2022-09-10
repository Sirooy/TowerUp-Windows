#pragma once

#include "Animation.h"

template<uint32_t TNumAnimations>
class AnimationPack
{
public:
    inline Animation& CurrentAnimation() { return animations[currentAnimationIndex]; }
    inline int32_t GetCurrentAnimationIndex() { return currentAnimationIndex; }
    inline Animation& operator[](uint32_t index) { return animations[index]; }
    inline void ChangeAnimation(E2D::Sprite& sprite, int32_t index) 
    {  
        currentAnimationIndex = index; 
        animations[index].Play(sprite);
    }
private:
    Animation animations[TNumAnimations];
    int32_t currentAnimationIndex;
};