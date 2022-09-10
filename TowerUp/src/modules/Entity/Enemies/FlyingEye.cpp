#include "FlyingEye.h"
#include "../World.h"
#include "../Player.h"
#include "../../Engine2D/Renderer.h"
#include "../../Engine2D/MediaContent.h"
#include "../../SFMLUtils.hpp"
#include "../../Physics/CollisionData.h"

FlyingEye::FlyingEye(sf::Vector2f position, float sizeMultiplier, float health, int32_t damage) : 
    Enemy(position, HITBOX_BASE_RADIUS * sizeMultiplier, health, damage), pathfinding(PATHFINDING_UPDATE_TIME)
{
    CreateStateMachine();
    CreateAnimations();

    this->Friction = 0.1f;
    this->knockback = 500.0f;

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("FlyingEye"));
    sprite.setOrigin(32.0f, 32.0f);
    sprite.setScale(sizeMultiplier, sizeMultiplier);
}

void FlyingEye::CreateStateMachine()
{
    stateMachine.AddState(FlyingEye::State::Idle, &FlyingEye::Idle, nullptr, nullptr);
    stateMachine.AddState(FlyingEye::State::Following, &FlyingEye::Following, &FlyingEye::OnEnterFollowing, nullptr);
    stateMachine.AddState(FlyingEye::State::Attacking, &FlyingEye::Attacking, &FlyingEye::OnEnterAttacking, nullptr);
    stateMachine.AddState(FlyingEye::State::FallingToDeath, &FlyingEye::FallingToDeath, &FlyingEye::OnEnterFallingToDeath, nullptr);
    stateMachine.AddState(FlyingEye::State::Dying, &FlyingEye::Dying, nullptr, nullptr);


    stateMachine.AddTransitionToState(FlyingEye::State::Idle, FlyingEye::State::Following, &FlyingEye::IdleToFollowing);
    stateMachine.AddTransitionToState(FlyingEye::State::Idle, FlyingEye::State::FallingToDeath, &FlyingEye::ToFallingToDeath);
    stateMachine.AddTransitionToState(FlyingEye::State::Following, FlyingEye::State::Attacking, &FlyingEye::FollowingToAttacking);
    stateMachine.AddTransitionToState(FlyingEye::State::Following, FlyingEye::State::FallingToDeath, &FlyingEye::ToFallingToDeath);
    stateMachine.AddTransitionToState(FlyingEye::State::Attacking, FlyingEye::State::Following, &FlyingEye::AttackingToFollowing);
    stateMachine.AddTransitionToState(FlyingEye::State::Attacking, FlyingEye::State::FallingToDeath, &FlyingEye::ToFallingToDeath);
    stateMachine.AddTransitionToState(FlyingEye::State::FallingToDeath, FlyingEye::State::Dying, &FlyingEye::ToDying);

    stateMachine.SetCurrentState(FlyingEye::State::Idle);
}

void FlyingEye::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::IdleIndex];
    idle.SetFrames(media.GetAnimationFrames("FlyingEyeFly"));
    idle.SetDuration(0.75f);

    auto& follow = animations[AnimationIndex::FollowIndex];
    follow.SetFrames(media.GetAnimationFrames("FlyingEyeFly"));
    follow.SetDuration(0.25f);

    auto& attack = animations[AnimationIndex::AttackIndex];
    attack.SetFrames(media.GetAnimationFrames("FlyingEyeAttack1"));
    attack.SetDuration(0.5f);
    attack.SetLoop(false);

    auto& die = animations[AnimationIndex::DieIndex];
    die.SetFrames(media.GetAnimationFrames("FlyingEyeDeath"));
    die.SetDuration(0.5f);
    die.SetLoop(false);

    animations.ChangeAnimation(sprite, AnimationIndex::IdleIndex);
}

void FlyingEye::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& attack = sounds[SoundIndex::AttackSound];
    attack.SetBuffer(*media.GetSoundBuffer("FlyingEyeAttack"));
    attack.SetRelativeToListener(true);
    attack.SetVolume(35.0f);
    
    auto& die = sounds[SoundIndex::DieSound];
    die.SetBuffer(*media.GetSoundBuffer("FlyingEyeDie"));
    die.SetRelativeToListener(true);
    die.SetVolume(35.0f);
}

void FlyingEye::Update(float dt)
{
    stateMachine.Update(this, dt);

    isOnGround = false;
}

void FlyingEye::Idle(float dt)
{
    animations.CurrentAnimation().Update(sprite, dt);
}

void FlyingEye::Following(float dt)
{
    pathfinding.RequestNewPath(ID, Position, player.Position);

    if(pathfinding.HasNotReachedDestination())
    {
        sf::Vector2f dir = pathfinding.GetPathNextDirection(Position, speed, dt);

        Velocity += dir * speed * dt;

        SetSpriteOrientation(Velocity);
    }

    animations.CurrentAnimation().Update(sprite, dt);
}

void FlyingEye::Attacking(float dt)
{
    animations.CurrentAnimation().Update(sprite, dt);
}

void FlyingEye::FallingToDeath(float dt)
{
    if(animations.CurrentAnimation().GetCurrentFrameIndex() < 1)
    {
        animations.CurrentAnimation().Update(sprite, dt);
    }
}

void FlyingEye::Dying(float dt)
{
    if(animations.CurrentAnimation().HasFinished())
    {
        sprite.FadeOut(1.0f, dt);
        
        if(sprite.IsFadedOut())
            Kill();
    }

    animations.CurrentAnimation().Update(sprite, dt);
}

void FlyingEye::OnEnterFollowing()
{
    animations.ChangeAnimation(sprite, AnimationIndex::FollowIndex);
}

void FlyingEye::OnEnterAttacking()
{
    animations.ChangeAnimation(sprite, AnimationIndex::AttackIndex);
    PlaySound(sounds[SoundIndex::AttackSound]);                
    AddCollisionMask(CollisionType::PlayerCollisionType);

    sf::Vector2f dir = sf::getNormalized(player.Position - Position);
    
    Velocity = dir * 500.0f;

    SetSpriteOrientation(Velocity);
}

void FlyingEye::OnEnterFallingToDeath()
{
    animations.ChangeAnimation(sprite, AnimationIndex::DieIndex);
    PlaySound(sounds[SoundIndex::DieSound]);    
    RemoveCollisionMask(CollisionType::PlayerCollisionType);
    RemoveCollisionMask(CollisionType::PlayerBulletCollisionType);
    sprite.setOrigin(32.0f, 48.0f); 
    BaseAcceleration = sf::Vector2f(0.0f, 1000.0f);
}

void FlyingEye::OnExitAttacking()
{
    RemoveCollisionMask(CollisionType::PlayerCollisionType);
}

bool FlyingEye::IdleToFollowing()
{
    return HasVisionOfPlayer(FOLLOW_DISTANCE_SQUARED);
}

bool FlyingEye::FollowingToAttacking()
{
    return sf::lengthSquared(player.Position - this->Position) < ATTACK_DISTANCE_SQUARED;
}

bool FlyingEye::AttackingToFollowing()
{
    return animations[AnimationIndex::AttackIndex].HasFinished();
}

bool FlyingEye::ToFallingToDeath()
{
    return health <= 0.0f;
}

bool FlyingEye::ToDying()
{
    return animations.CurrentAnimation().GetCurrentFrameIndex() >= 1 && isOnGround;
}

void FlyingEye::OnCollision(Bullet* bullet, const CollisionData& data)
{
    Enemy::OnCollision(bullet, data);
    if(stateMachine.GetCurrentState()->Key == State::Idle)
        stateMachine.SetCurrentState(State::Following);
}

void FlyingEye::OnVerticalTileCollision(const TileCollisionData& data)
{
    DynamicEntity::OnVerticalTileCollision(data);
    if(data.Normal.y == -1.0f)
        isOnGround = true;
}