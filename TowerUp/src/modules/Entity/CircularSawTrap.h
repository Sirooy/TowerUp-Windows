#pragma once
#include "Trap.h"
#include "../Animation.h"
#include "HurtZone.h"
#include "../StateMachine.h"

class CircularSawTrap : public Trap
{
public:
    CircularSawTrap(sf::Vector2f position, TrapDirection direction, float startDelay, 
        const std::string& framesName, float sizeMultiplier = 1.0f);

    void Update(float dt) override;
    void Draw(float tickPercent) override;

    void SetDamage(int32_t damage);
    void SetSpeed(float speed);

    ~CircularSawTrap();
protected:

    static constexpr float KNOCKBACK = 500.0f;

    enum State
    {
        Delay = 0,
        Moving = 1
    };

    enum SoundIndex
    {
        Collision = 0
    };

    HurtZone hurtZone;
    Animation spinAnimation;
    StateMachine<CircularSawTrap, 2> stateMachine;
    sf::Vector2f currentDirection;
    sf::Vector2f positionPoints[2];
    int32_t currentPositionPoint;
    float speed;
    E2D::Sound sounds[1];

    void CalculatePositions();
    void UpdateDelay(float dt);
    void UpdateMoving(float dt);
    void CreateSounds();
};