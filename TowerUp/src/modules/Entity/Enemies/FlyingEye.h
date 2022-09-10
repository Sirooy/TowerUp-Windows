#pragma once

#include "Enemy.h"
#include <vector>
#include "../../PathFindingFollower.h"
#include "../../TransitionStateMachine.h"
#include "../../AnimationPack.h"


class Player;

class FlyingEye : public Enemy
{
public:
    static constexpr float FOLLOW_DISTANCE_SQUARED = 650.0f * 650.0f;
    static constexpr float ATTACK_DISTANCE_SQUARED = 100.0f * 100.0f;
    static constexpr float PATHFINDING_UPDATE_TIME = 1 / 5.0f;
    static constexpr float BASE_HEALTH = 45.0f;
    static constexpr int32_t BASE_DAMAGE = 1;

    FlyingEye(sf::Vector2f position, float sizeMultiplier, float health = BASE_HEALTH, int32_t attack = 1);

    void Update(float dt) override;

    void CreateStateMachine();
    void CreateAnimations();
    void CreateSounds();
    void Idle(float dt);
    void Following(float dt);
    void Attacking(float dt);
    void FallingToDeath(float dt);
    void Dying(float dt);
    void OnEnterFollowing();
    void OnEnterAttacking();
    void OnExitAttacking();
    void OnEnterFallingToDeath();
    bool IdleToFollowing();
    bool FollowingToAttacking();
    bool AttackingToFollowing();
    bool ToFallingToDeath();
    bool ToDying();

    void OnCollision(Bullet* bullet, const CollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;

    void SetSpeed(float speed) { this->speed = speed; }
    float GetSpeed() { return speed; }

protected:
    static constexpr float HITBOX_BASE_RADIUS = 10.0f;

    enum class State
    {
        Idle,
        Following,
        Attacking,
        FallingToDeath,
        Dying
    };

    enum AnimationIndex
    {
        IdleIndex = 0,
        FollowIndex,
        AttackIndex,
        DieIndex
    };

    enum SoundIndex
    {
        DieSound,
        AttackSound
    };

    PathFindingFollower pathfinding;
    TransitionStateMachine<FlyingEye::State, FlyingEye> stateMachine {};
    AnimationPack<4> animations {};
    float speed{ 650.0f };
    bool isOnGround { false };
    E2D::Sound sounds[2];
};