#pragma once
#include "WallTrap.h"
#include "BulletGenerator.h"
#include "../Animation.h"
#include "../StateMachine.h"

class BulletGenerator;

class ShooterTrap : public WallTrap
{
public: 
    ShooterTrap(sf::Vector2f position, TrapDirection direction, 
        float startDelay, const std::string& framesName, float reloadTime);

    void Update(float dt) override;

    BulletGenerator& GetGenerator();
protected:
    enum State
    {
        Delay = 0,
        Reloading = 1,
        Shooting = 2
    };

    Animation shootAnimation {};
    StateMachine<ShooterTrap, 3> stateMachine {};
    BulletGenerator generator {};
    sf::Vector2f bulletDirection {};
    sf::Vector2f bulletPosition {};
    float reloadTime {};
    float currentReloadTime {};
    E2D::Sound sound {};

    void SetBulletGeneratorData();
    void SetBulletDirectionAndPosition();
    void UpdateDelay(float dt);
    void UpdateReloading(float dt);
    void UpdateShooting(float dt);
};