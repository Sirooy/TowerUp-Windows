#pragma once

#include "DynamicEntity.h"
#include "../Engine2D/Sprite.h"
#include "../AnimationPack.h"
#include "../StateMachine.h"

class Consumable : public DynamicEntity
{
public:
    Consumable(sf::Vector2f position, float radius, float aliveTime);
    Consumable(sf::Vector2f position, float width, float height, float aliveTime);

    void Update(float dt) override;
    void Draw(float tickPercent) override;

    void UpdateDefault(float dt);
    void UpdateDissapearing(float dt);
    void UpdatePickedUp(float dt);

    void OnCollision(CollidableEntity* other, const CollisionData& data) override;
    virtual void OnCollision(Player* player, const CollisionData& data) override;

    virtual ~Consumable() = default;

protected:
    static constexpr float DEFAULT_TO_DISSAPEARING_TIME = 4.0f;
    static constexpr float BLINK_CHANGE_TIME = 0.1f;
    static constexpr float SPRITE_SIZE = 32.0f;

    enum AnimationIndex
    {
        DefaultIndex = 0,
        PickedUpIndex
    };

    enum State
    {
        Default = 0,
        Dissapearing,
        PickedUp
    };

    E2D::Sprite sprite;
    AnimationPack<2> animations;
    StateMachine<Consumable, 3> stateMachine;
    E2D::Sound sound;

    float aliveTime;
    float blinkTime;
};