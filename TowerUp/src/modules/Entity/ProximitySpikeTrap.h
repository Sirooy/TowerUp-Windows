#pragma once
#include "WallTrap.h"

#include "HurtZone.h"
#include "../Animation.h"
#include "../StateMachine.h"

class Player;

class ProximitySpikeTrap : public WallTrap
{
public:
    ProximitySpikeTrap(sf::Vector2f position, TrapDirection direction, 
        const std::string& framesName, float triggerTime, float deactivateTime, int32_t damage);

    void Update(float dt) override;

    ~ProximitySpikeTrap();
protected:
    enum State
    {
        DetectPlayer = 0,
        GettingTriggered,
        TriggeredOff
    };

    enum SoundIndex
    {
        TriggerSound = 0,
        ActivateSound
    };

    Animation triggerAnimation;
    StateMachine<ProximitySpikeTrap, 3> stateMachine;
    HurtZone hurtZone;
    Player* player;
    float triggerTime;
    float currentTriggerTime;
    float deactivateTime;
    float currentDeactivateTime;
    float halfWidth;
    float halfHeight;
    E2D::Sound sounds[2];

    void CalculateSize();
    void UpdateDetectPlayer(float dt);
    void UpdateGettingTriggered(float dt);
    void UpdateTriggeredOff(float dt);
    void CreateSounds();
};