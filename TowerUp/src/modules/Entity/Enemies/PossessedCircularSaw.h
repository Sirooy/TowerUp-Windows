#pragma once

#include "Enemy.h"
#include "../../TransitionStateMachine.h"
#include "../../PathFindingFollower.h"
#include "../../AnimationPack.h"

class PossessedCircularSaw : public Enemy
{
public:
    static constexpr float CHARGE_DISTANCE_SQUARED = 250.0f  * 250.0f;
    static constexpr float PATHFINDING_UPDATE_TIME = 1 / 5.0f;
    static constexpr float BASE_FRICTION = 0.1f;
    static constexpr float SPINNING_ARMOR = 0.5f;
    static constexpr float BASE_HEALTH = 60.0f;

    PossessedCircularSaw(sf::Vector2f position, float health = BASE_HEALTH, float followSpeed = 350.0f, float bounceSpeed = 450.0f);

    void Update(float dt) override;

    void CreateStateMachine();
    void CreateAnimations();
    void CreateSounds();
    void Following(float dt);
    void Charging(float dt);
    void Dying(float dt);

    void OnExitIdle();
    void OnEnterCharging();
    void OnExitCharging();
    void OnEnterBouncing();
    void OnExitBouncing();
    void OnEnterFollowing();
    void OnEnterDying();

    bool IdleToCharging();
    bool FollowingToCharging();
    bool ChargingToBouncing();
    bool BouncingToFollowing();
    bool ToDying();

    void OnCollision(Bullet* bullet, const CollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;
    void OnHorizontalTileCollision(const TileCollisionData& data) override;

    float GetBounceSpeed() { return bounceSpeed; }
    void SetBounceSpeed(float speed) { this->bounceSpeed = speed; }
    float GetFollowSpeed() { return followSpeed; }
    void SetFollowSpeed(float speed) { this->followSpeed = speed; }
protected:
    static constexpr float RADIUS = 28.0f;

    enum class State
    {
        Idle,
        Following,
        Charging,
        Bouncing,
        Dying
    };

    enum AnimationIndex
    {
        SpinIndex = 0,
        FollowIndex,
        DieIndex
    };

    enum SoundIndex
    {
        ChargingSound = 0,
        CollisionSound,
        DieSound
    };

    PathFindingFollower pathfinding;
    TransitionStateMachine<PossessedCircularSaw::State, PossessedCircularSaw> stateMachine {};
    AnimationPack<4> animations {};
    float followSpeed{ 350.0f };
    float bounceSpeed{ 450.0f };
    float currentChargeTime { 0.0f };
    float maxChargeTime { 2.0f };
    int32_t currentNumBounces { 0 };
    int32_t maxNumBounces { 5 };
    E2D::Sound sounds[3];
};