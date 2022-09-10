#pragma once

#include "Enemy.h"
#include "../BulletGenerator.h"
#include "../../TransitionStateMachine.h"
#include "../../AnimationPack.h"

class AxeSkeleton : public Enemy
{
public:
    static constexpr float SPRITE_WIDTH = 28.0f;
    static constexpr float SPRITE_HEIGHT = 78.0f;
    static constexpr float FOLLOW_DISTANCE_SQUARED = 600.0f * 600.0f;
    static constexpr float BASE_FRICTION = 0.05f;
    static constexpr float BASE_HEALTH = 60.0f;
    static constexpr float ATTACK_Y_OFFSET = SPRITE_HEIGHT * 0.5f;
    static constexpr float WALK_TOWARDS_PLAYER_DISTANCE_SQUARED = 500.0f * 500.0f;
    static constexpr float WALK_AWAY_FROM_PLAYER_DISTANCE_SQUARED = 400.0f * 400.0f;
    static constexpr int32_t ATTACK_FRAME = 9;
    

    AxeSkeleton(sf::Vector2f position, float health = BASE_HEALTH);

    void Update(float dt) override;
    void CreateStateMachine();
    void CreateAnimations();
    void CreateSounds();
    void SetBulletGeneratorData();

    void Patrolling(float dt);
    void WalkingTowardsPlayer(float dt);
    void WalkingAwayFromPlayer(float dt);
    void PreparingAttack(float dt);
    void Attacking(float dt);
    void Dying(float dt);

    void OnEnterWalkingTowardsPlayer();
    void OnEnterWalkingAwayFromPlayer();
    void OnExitWalkingAwayFromPlayer();
    void OnEnterPreparingAttack();
    void OnEnterAttacking();
    void OnExitAttacking();
    void OnEnterDying();

    bool FromPatrollingToPreparingAttack();
    bool FromPreparingAttackToWalkingTowardsPlayer();
    bool FromPreparingAttackToWalkingAwayFromPlayer();
    bool FromWalkingTowardsPlayerToPreparingAttack();
    bool FromWalkingAwayFromPlayerToPreparingAttack();
    bool FromAttackingToPreparingAttack();
    bool ToAttacking();
    bool ToDying();

    inline BulletGenerator& GetGenerator() { return generator; }

protected:
    enum class State
    {
        Patrolling,
        WalkingTowardsPlayer,
        WalkingAwayFromPlayer,
        PreparingAttack,
        Attacking,
        Dying
    };

    enum AnimationIndex
    {
        IdleIndex = 0,
        WalkIndex,
        AttackIndex,
        DieIndex
    };

    enum SoundIndex
    {
        ChargingSound = 0,
        CollisionSound,
        DieSound
    };

    TransitionStateMachine<AxeSkeleton::State, AxeSkeleton> stateMachine {};
    AnimationPack<4> animations {};
    BulletGenerator generator {};
    float direction { 1.0f };
    float moveSpeed { 550.0f };
    float currentAttackTime { 0.0f };
    float attackTime { 2.0f };
};