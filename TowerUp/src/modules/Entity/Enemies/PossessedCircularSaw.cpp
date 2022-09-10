#include "PossessedCircularSaw.h"

#include "../../Physics/CollisionData.h"
#include "../../Engine2D/MediaContent.h"
#include "../../SFMLUtils.hpp"
#include "../Player.h"
#include "../Bullet.h"
#include "../../Random.h"

PossessedCircularSaw::PossessedCircularSaw(sf::Vector2f position, float health, float followSpeed, float bounceSpeed) 
    : Enemy(position, RADIUS - 8.0f, health, 1), 
    pathfinding(PATHFINDING_UPDATE_TIME)
{
    this->followSpeed = followSpeed;
    this->bounceSpeed = bounceSpeed;

    CreateStateMachine();
    CreateAnimations();
    CreateSounds();
    this->Friction = 1.0f;
    AddCollisionMask(CollisionType::PlayerCollisionType);

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("PossessedCircularSaw"));
    sprite.setOrigin(RADIUS, RADIUS);

    //Establecer una dirección aleatoria en la que rebotara en el estado por defecto
    if(Random::GlobalInstance.NextInt(0, 2) == 0)
    {
        Velocity.x = (Random::GlobalInstance.NextInt(0, 2) == 0) ? 1.0f : -1.0f;
        Velocity.x *= bounceSpeed;
    }   
    else
    {
        Velocity.y = (Random::GlobalInstance.NextInt(0, 2) == 0) ? 1.0f : -1.0f;
        Velocity.y *= bounceSpeed;
    }
}

void PossessedCircularSaw::CreateStateMachine()
{
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
}

void PossessedCircularSaw::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::SpinIndex];
    idle.SetFrames(media.GetAnimationFrames("PossessedCircularSawSpin"));
    idle.SetDuration(0.4f);

    auto& follow = animations[AnimationIndex::FollowIndex];
    follow.SetFrames(media.GetAnimationFrames("PossessedCircularSawFly"));
    follow.SetDuration(1.0f);

    auto& attack = animations[AnimationIndex::DieIndex];
    attack.SetFrames(media.GetAnimationFrames("PossessedCircularSawDeath"));
    attack.SetDuration(2.0f);
    attack.SetLoop(false);

    animations.ChangeAnimation(sprite, AnimationIndex::SpinIndex);
}

void PossessedCircularSaw::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& collision = sounds[SoundIndex::CollisionSound];
    collision.SetBuffer(*media.GetSoundBuffer("PossessedCircularSawCollision"));
    collision.SetRelativeToListener(false);
    collision.SetVolume(300.0f);
    collision.SetAttenuation(0.3f);

    auto& charge = sounds[SoundIndex::ChargingSound];
    charge.SetBuffer(*media.GetSoundBuffer("PossessedCircularSawCharging"));
    charge.SetRelativeToListener(false);
    charge.SetVolume(300.0f);
    charge.SetAttenuation(0.1f);

    auto& dead = sounds[SoundIndex::DieSound];
    dead.SetBuffer(*media.GetSoundBuffer("PossessedCircularSawDie"));
    dead.SetRelativeToListener(false);
    dead.SetVolume(300.0f);
    dead.SetAttenuation(0.15f);
}

void PossessedCircularSaw::Update(float dt) 
{
    stateMachine.Update(this, dt);

    animations.CurrentAnimation().Update(sprite, dt);
}

void PossessedCircularSaw::Following(float dt)
{
    pathfinding.RequestNewPath(ID, Position, player.Position);

    if(pathfinding.HasNotReachedDestination())
    {
        sf::Vector2f dir = pathfinding.GetPathNextDirection(Position, followSpeed, dt);

        Velocity += dir * followSpeed * dt;
    }
}

void PossessedCircularSaw::Charging(float dt)
{
    currentChargeTime += dt;
}

void PossessedCircularSaw::Dying([[maybe_unused]] float dt)
{
    if(animations.CurrentAnimation().HasFinished())
    {
        sprite.FadeOut(1.0f, dt);
        
        if(sprite.IsFadedOut())
            Kill();
    }
}

void PossessedCircularSaw::OnExitIdle()
{
    currentChargeTime = maxChargeTime * 0.5f; //Si a charging directamente desde idle el tiempo de carga se reduce a la mitad
}

void PossessedCircularSaw::OnEnterCharging()
{
    animations.ChangeAnimation(sprite, AnimationIndex::SpinIndex);
    PlaySound(sounds[SoundIndex::ChargingSound]);
    Velocity = sf::Vector2f(0.0f, 0.0f);
    armor = SPINNING_ARMOR;
    knockbackResistance = 0.0f;
}

void PossessedCircularSaw::OnExitCharging()
{
    currentChargeTime = 0.0f; //Reseteamos en el OnExit para que pueda pasar desde idle con la mitad de tiempo de carga
}

void PossessedCircularSaw::OnEnterBouncing()
{
    Friction = 1.0f;
    currentNumBounces = 0;

    Velocity = sf::getNormalized(player.Position - Position) * bounceSpeed;
  
}

void PossessedCircularSaw::OnExitBouncing()
{
    Friction = BASE_FRICTION;
    knockbackResistance = 1.0f;
    armor = 1.0f;
}

void PossessedCircularSaw::OnEnterFollowing()
{
    animations.ChangeAnimation(sprite, AnimationIndex::FollowIndex);
}

void PossessedCircularSaw::OnEnterDying()
{
    RemoveCollisionMask(PlayerCollisionType);
    RemoveCollisionMask(PlayerBulletCollisionType);

    animations.ChangeAnimation(sprite, AnimationIndex::DieIndex);
    PlaySound(sounds[SoundIndex::DieSound]);
}

bool PossessedCircularSaw::IdleToCharging()
{
    return HasVisionOfPlayer(CHARGE_DISTANCE_SQUARED);
}

bool PossessedCircularSaw::FollowingToCharging()
{
    return HasVisionOfPlayer(CHARGE_DISTANCE_SQUARED) && !IsNearWall(Velocity.x, 34.0f);
}

bool PossessedCircularSaw::ChargingToBouncing()
{
    return currentChargeTime >= maxChargeTime;
}

bool PossessedCircularSaw::BouncingToFollowing()
{
    return currentNumBounces >= maxNumBounces;
}

bool PossessedCircularSaw::ToDying()
{
    return health <= 0.0f;
}

void PossessedCircularSaw::OnCollision(Bullet* bullet, const CollisionData& data)
{
    if(stateMachine.GetCurrentState()->Key == State::Idle)
    {
        Friction = BASE_FRICTION;
        stateMachine.SetCurrentState(State::Following);
        animations.ChangeAnimation(sprite, AnimationIndex::FollowIndex);
    }

    Enemy::OnCollision(bullet, data);
}

void PossessedCircularSaw::OnVerticalTileCollision(const TileCollisionData& data)
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

void PossessedCircularSaw::OnHorizontalTileCollision(const TileCollisionData& data)
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