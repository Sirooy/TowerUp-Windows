#pragma once

#include <vector>
#include "SFML/Graphics/Rect.hpp"
#include "Engine2D/Sprite.h"

class Animation
{
public:
    Animation();
    Animation(std::vector<sf::IntRect>* frames);

    void SetFrames(std::vector<sf::IntRect>* frames);
    void SetDuration(float duration);
    void SetLoop(bool value);
    void SetReversed(bool value);
    void Play(E2D::Sprite& sprite);
    void Pause();
    void Resume();
    void SetCurrentFrameIndex(int32_t index, E2D::Sprite&  sprite);
    inline void Update(E2D::Sprite& sprite, float elapsedTime) { (this->*updateFunction)(sprite, elapsedTime); }
    inline void SetCurrentFrameIndex(int32_t index) { currentFrameIndex = index; }
    inline float GetDuration() { return duration; }
    inline bool GetLoop()      { return loop; }
    inline bool GetReversed()  { return reversed; }
    inline bool HasFinished()  { return hasFinished; }
    inline bool HasFinishedFrame() { return  hasFinishedFrame; }
    inline const sf::IntRect& GetFrame(int32_t index) { return (*frames)[index]; }
    inline std::vector<sf::IntRect>* GetFrames() { return frames; }
    inline int32_t GetCurrentFrameIndex() { return currentFrameIndex; }
    inline std::size_t GetNumFrames() { return frames->size(); }

private:
    std::vector<sf::IntRect>* frames;
    void (Animation::*updateFunction) (E2D::Sprite& sprite, float elapsedTime);
    float duration;
    float maxFrameDuration;
    float currentFrameDuration;
    int32_t numFrames;
    int32_t currentFrameIndex;
    uint8_t loop;
    uint8_t reversed;
    bool hasFinishedFrame;
    bool hasFinished;

    void DoNothing(E2D::Sprite& sprite, float elapsedTime);
    void UpdateWithLoop(E2D::Sprite& sprite, float elapsedTime);
    void UpdateWithLoopReversed(E2D::Sprite& sprite, float elapsedTime);
    void UpdateWithoutLoop(E2D::Sprite& sprite, float elapsedTime);
    void UpdateWithoutLoopReversed(E2D::Sprite& sprite, float elapsedTime);

    static inline void (Animation::*UpdateFuncJumpTable[2][2]) (E2D::Sprite& sprite, float elapsedTime) = 
    {
        { 
            &Animation::UpdateWithoutLoop,          //[0][0] [No Loop][No Reversed]
            &Animation::UpdateWithoutLoopReversed   //[0][1] [No Loop][Reversed]   
        },
        { 
            &Animation::UpdateWithLoop,             //[1][0] [Loop][No Reversed]
            &Animation::UpdateWithLoopReversed      //[1][1] [Loop][Reversed]   
        }
    };
};