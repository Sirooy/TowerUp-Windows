#pragma once

#include "Enemy.h"
#include "../BulletGenerator.h"
#include "../../PathFindingFollower.h"
#include "../../TransitionStateMachine.h"
#include "../../AnimationPack.h"

#include <vector>

class Player;

class BatEye : public Enemy
{
public:
    static constexpr float FOLLOW_DISTANCE_SQUARED = 600.0f * 600.0f;
    static constexpr float SHOOT_DISTANCE_SQUARED = 400.0f * 400.0f;
    static constexpr float DASH_POWER = 500.0f;
    static constexpr float PATHFINDING_UPDATE_TIME = 1 / 5.0f;
    static constexpr float MAX_ESCAPE_TIME = 1.0f;
    static constexpr float HITBOX_RADIUS = 10.0f; //24.0f
    static constexpr float BASE_HEALTH = 30.0f;

    enum class State
    {
        Patrolling,
        Following,
        Escaping,
        Shooting,
        Dying
    };

    BatEye(sf::Vector2f position, float sizeMultiplier = 1.0f, float health = BASE_HEALTH);

    void Update(float dt) override;
    void Draw(float tickPercent) override;

    void CreateStateMachine();
    void CreateAnimations();
    void CreateSounds();
    void Patrolling(float dt);
    void Following(float dt);
    void Escaping(float dt);
    void Shooting(float dt);
    void Dying(float dt);


    void OnEnterFollowing();
    void OnEnterShooting();
    void OnEnterScaping();
    void OnEnterDying();
    void OnExitShooting();
    
    bool PatrollingToFollowing();
    bool FollowingToShooting();
    bool ShootingToScaping();
    bool ScapingToFollowing();
    bool ToDying();
    
    inline void ResetInterpolatedPosition() override { LastPosition = Position; lastRotation = rotation;} 

    BulletGenerator& GetGenerator() { return generator; }
    float GetPatrollingSpeed() { return patrollingSpeed; }
    float GetFollowSpeed() { return followSpeed; }
    int32_t GetMaxNumShoots() { return maxNumShots; }
    void SetPatrollingSpeed(float speed) { patrollingSpeed = speed; } 
    void SetFollowSpeed(float speed) { followSpeed = speed; }
    void SetMaxNumShoots(int32_t shots) { maxNumShots = shots; }
protected:
    enum AnimationIndex
    {
        PatrolIndex = 0,
        FlyIndex,
        ShootIndex,
        DieIndex
    };

    enum SoundIndex
    {
        ShootSound = 0,
        DieSound
    };

    PathFindingFollower pathfinding;
    TransitionStateMachine<BatEye::State, BatEye> stateMachine {};
    AnimationPack<4> animations {};
    BulletGenerator generator {};
    int32_t currentNumShots { 0 };
    int32_t maxNumShots { 1 }; 
    float patrollingSpeed { 400.0f };
    float followSpeed { 750.0f };
    float currentScapeTime { 0.0f };
    float rotation { 0.0f };
    float lastRotation { 0.0f };
    bool hasShoot { false };
    E2D::Sound sounds[2];

    void SetBulletGeneratorData();
   
};