#include "Animation.h"

Animation::Animation() : frames(nullptr), updateFunction(&Animation::UpdateWithLoop), duration(0.0f), 
    maxFrameDuration(0), currentFrameDuration(0), numFrames(0), currentFrameIndex(0), loop(true), reversed(false), hasFinished(false)
{ }

Animation::Animation(std::vector<sf::IntRect>* frames) : frames(frames), updateFunction(&Animation::UpdateWithLoop), duration(0.0f),
    maxFrameDuration(0), currentFrameDuration(0), numFrames(0), currentFrameIndex(0), loop(true), reversed(false), hasFinished(false)
{ }

void Animation::SetFrames(std::vector<sf::IntRect>* frames)
{
    this->frames = frames;

    numFrames = static_cast<int32_t>(frames->size());
    this->maxFrameDuration = duration / numFrames;
    currentFrameDuration = 0.0f;
    currentFrameIndex = 0;
}

void Animation::SetDuration(float duration)
{
    this->duration = duration;

    if(frames)
    {
        maxFrameDuration = duration / numFrames;
        currentFrameDuration = 0.0f;
        currentFrameIndex = 0;
    }
}

void Animation::SetLoop(bool value)
{
    this->loop = value;

    updateFunction = UpdateFuncJumpTable[loop][reversed];
}

void Animation::SetReversed(bool value)
{
    this->reversed = value;

    updateFunction = UpdateFuncJumpTable[loop][reversed];
}

void Animation::Play(E2D::Sprite& sprite)
{
    hasFinished = false;
    currentFrameDuration = 0.0f;
    currentFrameIndex = (reversed) ? numFrames - 1 : 0;
    sprite.SetTextureRect((*frames)[currentFrameIndex]);

    //Volver a establecer la funcion por si el loop esta desactivado y se encuentra dentro de la funcion DoNothing
    updateFunction = UpdateFuncJumpTable[loop][reversed];
}

void Animation::Pause()
{
    updateFunction = &Animation::DoNothing;
}

void Animation::Resume()
{
    updateFunction = UpdateFuncJumpTable[loop][reversed];
}

void Animation::DoNothing([[maybe_unused]] E2D::Sprite& sprite, [[maybe_unused]] float elapsedTime) { }

void Animation::SetCurrentFrameIndex(int32_t index, E2D::Sprite&  sprite) 
{ 
    currentFrameIndex = index; 
    sprite.SetTextureRect((*frames)[currentFrameIndex]);
}

void Animation::UpdateWithLoop(E2D::Sprite& sprite, float elapsedTime)
{
    hasFinished = false;
    hasFinishedFrame = false;
    currentFrameDuration += elapsedTime;

    if(currentFrameDuration >= maxFrameDuration)
    {
        currentFrameDuration = 0.0f;
        ++currentFrameIndex;
        hasFinishedFrame = true;

        if(currentFrameIndex == numFrames)
        {
            currentFrameIndex = 0;
            hasFinished = true;
        }
        
        sprite.SetTextureRect((*frames)[currentFrameIndex]);
    }
}

void Animation::UpdateWithLoopReversed(E2D::Sprite& sprite, float elapsedTime)
{
    hasFinished = false;
    hasFinishedFrame = false;
    currentFrameDuration += elapsedTime;

    if(currentFrameDuration >= maxFrameDuration)
    {
        currentFrameDuration = 0.0f;
        --currentFrameIndex;
        hasFinishedFrame = true;
        
        if(currentFrameIndex == -1)
        {
            currentFrameIndex = numFrames - 1;
            hasFinished = true;
        }
        
        sprite.SetTextureRect((*frames)[currentFrameIndex]);
    }
}

void Animation::UpdateWithoutLoop(E2D::Sprite& sprite, float elapsedTime)
{
    hasFinished = false;
    hasFinishedFrame = false;
    currentFrameDuration += elapsedTime;

    if(currentFrameDuration >= maxFrameDuration)
    {
        ++currentFrameIndex;
        hasFinishedFrame = true;

        if(currentFrameIndex == numFrames)
        {
            currentFrameIndex = 0;
            hasFinished = true;
            updateFunction = &Animation::DoNothing;
        }
        else
        {
            sprite.SetTextureRect((*frames)[currentFrameIndex]);
            currentFrameDuration = 0.0f;
        }
    }
}

void Animation::UpdateWithoutLoopReversed(E2D::Sprite& sprite, float elapsedTime)
{
    hasFinished = false;
    hasFinishedFrame = false;
    currentFrameDuration += elapsedTime;

    if(currentFrameDuration >= maxFrameDuration)
    {
        --currentFrameIndex;
        hasFinishedFrame = true;

        if(currentFrameIndex == -1)
        {
            currentFrameIndex = numFrames - 1;
            hasFinished = true;
            updateFunction = &Animation::DoNothing;
        }
        else
        {
            sprite.SetTextureRect((*frames)[currentFrameIndex]);
            currentFrameDuration = 0.0f;
        }
    }
}