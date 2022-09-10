#pragma once

#include "InteractiveEntity.h"
#include "../Engine2D/Sprite.h"
#include "../StateMachine.h"
#include "../Animation.h"
#include "SFML/Graphics/RenderStates.hpp"
#include "EntityTypeEnums.h"

class World;
class Curse;

class Door : public InteractiveEntity
{
public:
    static constexpr float WIDTH = 144.0f;
    static constexpr float HEIGHT = 144.0f;

    Door(sf::Vector2f position, int32_t id, DoorType type, const std::vector<Curse*>& curses);

    void Update(float dt) override;
    void WhenClosed(float dt);
    void WhenOpened(float dt);
    void Draw(float tickPercent) override;
    void OnInteract() override;

    bool IsOpen() { return stateMachine.GetState() == State::Opened; }

private:
    static constexpr float ACTIVE_DISTANCE_SQUARED = 64.0f * 64.0f;

    enum State
    {
        Closed = 0,
        Opened
    };

    enum SoundIndex
    {
        EnterSound = 0
    };
    
    std::vector<E2D::Sprite> cursesSprites;
    StateMachine<Door, 2> stateMachine;
    Animation openAnimation;
    int32_t doorId;
    E2D::Sound sounds[1];

    void CreateOpenAnimation(DoorType type);
    void CreateCursesSprites(const std::vector<Curse*>& curses);
    void CreateSounds();
};