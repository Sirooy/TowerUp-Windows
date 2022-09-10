#include "AxeSkeleton.h"

#include "PossessedCircularSaw.h"

#include "../../Physics/PhysicsEngine.h"
#include "../../Physics/CollisionData.h"
#include "../../Engine2D/MediaContent.h"
#include "../../SFMLUtils.hpp"
#include "../Player.h"
#include "../Bullet.h"
#include "../../Random.h"

AxeSkeleton::AxeSkeleton(sf::Vector2f position, float health) 
    : Enemy(position, SPRITE_WIDTH, SPRITE_HEIGHT, health, 1)
{
    CreateStateMachine();
    CreateAnimations();
    CreateSounds();
    this->Friction = BASE_FRICTION;
    this->BaseAcceleration = PhysicsEngine::BaseGravity;
    this->knockbackResistance = 0.25f;

    const sf::IntRect& firstFrame = animations.CurrentAnimation().GetFrame(0);
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("AxeSkeleton"));
    sprite.setOrigin(firstFrame.width * 0.5f, firstFrame.width * 0.5f);
    SetSpriteOrientation(direction); 
    SetBulletGeneratorData();
}

void AxeSkeleton::CreateStateMachine()
{
    stateMachine.AddState(State::Patrolling, &AxeSkeleton::Patrolling, nullptr, nullptr); 
    stateMachine.AddState(State::WalkingTowardsPlayer, &AxeSkeleton::WalkingTowardsPlayer, &AxeSkeleton::OnEnterWalkingTowardsPlayer, 
        nullptr); 
    stateMachine.AddState(State::WalkingAwayFromPlayer, &AxeSkeleton::WalkingAwayFromPlayer, &AxeSkeleton::OnEnterWalkingAwayFromPlayer, 
        &AxeSkeleton::OnExitWalkingAwayFromPlayer); 
    stateMachine.AddState(State::PreparingAttack, &AxeSkeleton::PreparingAttack, &AxeSkeleton::OnEnterPreparingAttack, nullptr); 
    stateMachine.AddState(State::Attacking, &AxeSkeleton::Attacking, &AxeSkeleton::OnEnterAttacking, &AxeSkeleton::OnExitAttacking);
    stateMachine.AddState(State::Dying, &AxeSkeleton::Dying, &AxeSkeleton::OnEnterDying, nullptr); 

    stateMachine.AddTransitionToState(State::Patrolling, State::PreparingAttack, &AxeSkeleton::FromPatrollingToPreparingAttack);
    stateMachine.AddTransitionToState(State::PreparingAttack, State::Attacking, &AxeSkeleton::ToAttacking);
    stateMachine.AddTransitionToState(State::WalkingTowardsPlayer, State::Attacking, &AxeSkeleton::ToAttacking);
    stateMachine.AddTransitionToState(State::WalkingAwayFromPlayer, State::Attacking, &AxeSkeleton::ToAttacking);

    stateMachine.AddTransitionToState(State::PreparingAttack, State::WalkingTowardsPlayer, 
        &AxeSkeleton::FromPreparingAttackToWalkingTowardsPlayer);
    stateMachine.AddTransitionToState(State::PreparingAttack, State::WalkingAwayFromPlayer, 
        &AxeSkeleton::FromPreparingAttackToWalkingAwayFromPlayer);
    stateMachine.AddTransitionToState(State::WalkingTowardsPlayer, State::PreparingAttack, 
        &AxeSkeleton::FromWalkingTowardsPlayerToPreparingAttack);
    stateMachine.AddTransitionToState(State::WalkingAwayFromPlayer, State::PreparingAttack, 
        &AxeSkeleton::FromWalkingAwayFromPlayerToPreparingAttack);

    stateMachine.AddTransitionToState(State::Attacking, State::PreparingAttack, &AxeSkeleton::FromAttackingToPreparingAttack);
    stateMachine.AddTransitionToAllStates(State::Dying, &AxeSkeleton::ToDying);
    /*
    stateMachine.AddState(State::Idle, nullptr, nullptr, 
        &PossessedCircularSaw::OnExitIdle);
    stateMachine.AddState(State::Following, &PossessedCircularSaw::Following, 
        &PossessedCircularSaw::OnEnterFollowing, nullptr);
    stateMachine.AddState(State::Charging, &PossessedCircularSaw::Charging, &PossessedCircularSaw::OnEnterCharging, 
        &PossessedCircularSaw::OnExitCharging);
    stateMachine.AddState(State::Bouncing, nullptr, 
        &PossessedCircularSaw::OnEnterBouncing, &PossessedCircularSaw::OnExitBouncing);
    stateMachine.AddState(State::Dying, &PossessedCircularSaw::Dying, 
        &PossessedCircularSaw::OnEnterDying, nullptr);

    stateMachine.AddTransitionToState(State::Idle, State::Charging, &PossessedCircularSaw::IdleToCharging);
    stateMachine.AddTransitionToState(State::Following, State::Charging, &PossessedCircularSaw::FollowingToCharging);
    stateMachine.AddTransitionToState(State::Charging, State::Bouncing, &PossessedCircularSaw::ChargingToBouncing);
    stateMachine.AddTransitionToState(State::Bouncing, State::Following, &PossessedCircularSaw::BouncingToFollowing);
    stateMachine.AddTransitionToAllStates(State::Dying, &PossessedCircularSaw::ToDying);

    stateMachine.SetCurrentState(State::Idle);
    */
    stateMachine.SetCurrentState(State::Patrolling);
}

void AxeSkeleton::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::IdleIndex];
    idle.SetFrames(media.GetAnimationFrames("AxeSkeletonIdle"));
    idle.SetDuration(0.4f);

    auto& walk = animations[AnimationIndex::WalkIndex];
    walk.SetFrames(media.GetAnimationFrames("AxeSkeletonWalk"));
    walk.SetDuration(0.6f);

    auto& attack = animations[AnimationIndex::AttackIndex];
    attack.SetFrames(media.GetAnimationFrames("AxeSkeletonAttack"));
    attack.SetDuration(0.2f);
    attack.SetLoop(false);

    auto& die = animations[AnimationIndex::DieIndex];
    die.SetFrames(media.GetAnimationFrames("AxeSkeletonDeath"));
    die.SetDuration(1.0f);
    die.SetLoop(false);

    animations.ChangeAnimation(sprite, AnimationIndex::WalkIndex);
}

void AxeSkeleton::SetBulletGeneratorData()
{
    auto& media = E2D::MediaContent::Instance();

    generator.BulletTexture = media.GetTexture("AxeSkeleton");
    generator.BulletAnimationFrames[0] = media.GetAnimationFrames("AxeSkeletonBulletAlive");
    generator.BulletAnimationFrames[1] = media.GetAnimationFrames("AxeSkeletonBulletDead");
    generator.BulletAnimationDurations[0] = 0.2f;
    generator.BulletAnimationDurations[1] = 0.1f;
    generator.BulletCollisionType = CollisionType::EnemyBulletCollisionType;
    generator.BulletCollisionMask = CollisionType::PlayerCollisionType;
    generator.BulletSpeed = 650.0f;
    generator.BulletSize = 22.5f;
    generator.BulletKnockBack = 500.0f;
    generator.BulletMaxAliveTime = 5.0f;
    generator.BulletType = BulletType::Default;
    generator.BulletPlayerDamage = 1;
    generator.BulletTexturePadding = 1.0f;
    generator.BulletBaseAcceleration = PhysicsEngine::BaseGravity * 0.25f;
}

void AxeSkeleton::CreateSounds()
{

}

void AxeSkeleton::Update(float dt)
{
    stateMachine.Update(this, dt);
    animations.CurrentAnimation().Update(sprite, dt);
}

void AxeSkeleton::Patrolling(float dt)
{
   Velocity.x += direction * moveSpeed * dt;

   if(IsNearEdge(SPRITE_HEIGHT * 0.5f, direction, SPRITE_WIDTH * 1.75f) || IsNearWall(direction, SPRITE_WIDTH * 1.75f))
   {
       direction = -direction;
       SetSpriteOrientation(direction);
   }
}

void AxeSkeleton::WalkingTowardsPlayer(float dt)
{
    currentAttackTime += dt;

    float xDir = sf::sign(player.Position.x - Position.x);
    Velocity.x += xDir * moveSpeed * dt;

    SetSpriteOrientation(xDir);
}

void AxeSkeleton::WalkingAwayFromPlayer(float dt)
{
    currentAttackTime += dt;

    float xDir = sf::sign(player.Position.x - Position.x);
    Velocity.x -= xDir * moveSpeed * dt;

    SetSpriteOrientation(xDir);
}


void AxeSkeleton::PreparingAttack(float dt)
{
    currentAttackTime += dt;

    SetSpriteOrientation(player.Position.x - Position.x);
}

void AxeSkeleton::Attacking([[maybe_unused]] float dt)
{
    float xDir = player.Position.x - Position.x;
    SetSpriteOrientation(xDir);

    if(animations.CurrentAnimation().HasFinishedFrame() && 
        animations.CurrentAnimation().GetCurrentFrameIndex() == ATTACK_FRAME)
    {
        constexpr float TIME = 1.8f; //Valor empirico que funciona con 650 de velocidad para las balas
        
        float angle = 0.0f;
        float acosVal = xDir / (generator.BulletSpeed * TIME);
        if (acosVal >= 1.0f)
            angle = sf::PI_OVER_4;
        else
            angle = std::acos(acosVal);

        sf::Vector2f bulletDir = sf::Vector2f(std::cos(angle), -std::sin(angle));
        generator.GenerateBullet(Position - sf::Vector2f(0.0f, ATTACK_Y_OFFSET), bulletDir);
    }
}

void AxeSkeleton::Dying(float dt)
{
    if(animations.CurrentAnimation().HasFinished())
    {
        sprite.FadeOut(1.0f, dt);
        
        if(sprite.IsFadedOut())
            Kill();
    }
}

void AxeSkeleton::OnEnterWalkingTowardsPlayer()
{
    animations.ChangeAnimation(sprite, AnimationIndex::WalkIndex);
}

void AxeSkeleton::OnEnterWalkingAwayFromPlayer()
{
    animations.ChangeAnimation(sprite, AnimationIndex::WalkIndex);
    animations.CurrentAnimation().SetReversed(true);
}

void AxeSkeleton::OnExitWalkingAwayFromPlayer()
{
    animations.CurrentAnimation().SetReversed(false);
}


void AxeSkeleton::OnEnterPreparingAttack()
{
    animations.ChangeAnimation(sprite, AnimationIndex::IdleIndex);
}

void AxeSkeleton::OnEnterAttacking()
{
    animations.ChangeAnimation(sprite, AnimationIndex::AttackIndex);
}

void AxeSkeleton::OnExitAttacking()
{
    currentAttackTime = Random::GlobalInstance.NextFloat(attackTime * 0.5f);
}

void AxeSkeleton::OnEnterDying()
{
    RemoveCollisionMask(PlayerCollisionType);
    RemoveCollisionMask(PlayerBulletCollisionType);
    RemoveCollisionMask(ExplosionCollisionType);

    animations.ChangeAnimation(sprite, AnimationIndex::DieIndex);
    //PlaySound(sounds[SoundIndex::DieSound]);
}

bool AxeSkeleton::FromPatrollingToPreparingAttack()
{
    return IsCloseToPlayer(400.0f * 400.0f);
}

bool AxeSkeleton::FromPreparingAttackToWalkingTowardsPlayer()
{
    return !IsCloseToPlayer(WALK_TOWARDS_PLAYER_DISTANCE_SQUARED);
}

bool AxeSkeleton::FromPreparingAttackToWalkingAwayFromPlayer()
{
    return IsCloseToPlayer(WALK_AWAY_FROM_PLAYER_DISTANCE_SQUARED);
}

bool AxeSkeleton::FromWalkingTowardsPlayerToPreparingAttack()
{
    return IsCloseToPlayer(WALK_TOWARDS_PLAYER_DISTANCE_SQUARED);
}

bool AxeSkeleton::FromWalkingAwayFromPlayerToPreparingAttack()
{
    return !IsCloseToPlayer(WALK_AWAY_FROM_PLAYER_DISTANCE_SQUARED);
}

bool AxeSkeleton::FromAttackingToPreparingAttack()
{
    return animations.CurrentAnimation().HasFinished();
}

bool AxeSkeleton::ToAttacking()
{
    return currentAttackTime >= attackTime;
}

bool AxeSkeleton::ToDying()
{
    return health <= 0.0f;
}



/*

bool AxeSkeleton::ToDying()
{
    return health <= 0.0f;
}

void AxeSkeleton::OnCollision(Bullet* bullet, const CollisionData& data)
{
    if(stateMachine.GetCurrentState()->Key == State::Idle)
    {
        Friction = BASE_FRICTION;
        stateMachine.SetCurrentState(State::Following);
        animations.ChangeAnimation(sprite, AnimationIndex::FollowIndex);
    }

    Enemy::OnCollision(bullet, data);
}

void AxeSkeleton::OnVerticalTileCollision(const TileCollisionData& data)
{
    if(stateMachine.GetCurrentState()->Key == State::Bouncing || 
        stateMachine.GetCurrentState()->Key == State::Idle)
    {
        Position += data.Normal * data.Penetration;
        Velocity.y = -Velocity.y;
        ++currentNumBounces;
        PlaySound(sounds[SoundIndex::CollisionSound]);
    }
    else
        DynamicEntity::OnVerticalTileCollision(data);

    
}

void AxeSkeleton::OnHorizontalTileCollision(const TileCollisionData& data)
{
    if(stateMachine.GetCurrentState()->Key == State::Bouncing || 
        stateMachine.GetCurrentState()->Key == State::Idle)
    {
        Position += data.Normal * data.Penetration;
        Velocity.x = -Velocity.x;
        ++currentNumBounces;
        PlaySound(sounds[SoundIndex::CollisionSound]);
    }
    else
        DynamicEntity::OnVerticalTileCollision(data);

    
}
*/