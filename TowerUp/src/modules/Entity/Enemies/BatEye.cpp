#include "BatEye.h"
#include "../World.h"
#include "../Player.h"
#include "../../Engine2D/Renderer.h"
#include "../../Engine2D/MediaContent.h"
#include "../../SFMLUtils.hpp"
#include "../../Physics/CollisionData.h"
#include "../BulletGenerator.h"
#include "../../SFMLUtils.hpp"
#include "../../Random.h"
#include <cmath>

BatEye::BatEye(sf::Vector2f position, float sizeMultiplier, float health) : 
    Enemy(position, HITBOX_RADIUS * sizeMultiplier, health, 1), pathfinding(PATHFINDING_UPDATE_TIME)
{
    CreateStateMachine();
    CreateAnimations();
    CreateSounds();
    SetBulletGeneratorData();

    this->CollisionType = CollisionType::EnemyCollisionType;
    AddCollisionMask(CollisionType::TileCollisionType);
    AddCollisionMask(CollisionType::PlayerBulletCollisionType);
    this->Friction = 0.1f;

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("BatEye"));
    sprite.setOrigin(32.0f, 32.0f);
    sprite.SetScale(sizeMultiplier, sizeMultiplier);

    pathfinding.RequestNewPath(ID, Position, GetRandomMapPosition());
}

void BatEye::CreateStateMachine()
{
    stateMachine.AddState(State::Patrolling, &BatEye::Patrolling, nullptr, nullptr);
    stateMachine.AddState(State::Following, &BatEye::Following, &BatEye::OnEnterFollowing, nullptr);
    stateMachine.AddState(State::Shooting, &BatEye::Shooting, &BatEye::OnEnterShooting, &BatEye::OnExitShooting);
    stateMachine.AddState(State::Escaping, &BatEye::Escaping, &BatEye::OnEnterScaping, nullptr);
    stateMachine.AddState(State::Dying, &BatEye::Dying, &BatEye::OnEnterDying, nullptr);
    
    stateMachine.AddTransitionToState(State::Patrolling, State::Following, &BatEye::PatrollingToFollowing);
    stateMachine.AddTransitionToState(State::Following, State::Shooting, &BatEye::FollowingToShooting);
    stateMachine.AddTransitionToState(State::Shooting, State::Escaping, &BatEye::ShootingToScaping);
    stateMachine.AddTransitionToState(State::Escaping, State::Following, &BatEye::ScapingToFollowing);

    stateMachine.AddTransitionToAllStates(State::Dying, &BatEye::ToDying);
    stateMachine.SetCurrentState(State::Patrolling);
}

void BatEye::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::PatrolIndex];
    idle.SetFrames(media.GetAnimationFrames("BatEyeFly"));
    idle.SetDuration(0.7f);

    auto& follow = animations[AnimationIndex::FlyIndex];
    follow.SetFrames(media.GetAnimationFrames("BatEyeFly"));
    follow.SetDuration(0.3f);

    auto& shoot = animations[AnimationIndex::ShootIndex];
    shoot.SetFrames(media.GetAnimationFrames("BatEyeShoot"));
    shoot.SetDuration(0.8f);
    //shoot.SetLoop(false);

    auto& die = animations[AnimationIndex::DieIndex];
    die.SetFrames(media.GetAnimationFrames("BatEyeDeath"));
    die.SetDuration(1.5f);
    die.SetLoop(false);

    animations.ChangeAnimation(sprite, AnimationIndex::PatrolIndex);
}

void BatEye::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& die = sounds[SoundIndex::DieSound];
    die.SetBuffer(*media.GetSoundBuffer("BatEyeDie"));
    die.SetRelativeToListener(true);
    die.SetVolume(15.0f);

    auto& shoot = sounds[SoundIndex::ShootSound];
    shoot.SetBuffer(*media.GetSoundBuffer("BatEyeShoot"));
    shoot.SetRelativeToListener(true);
    shoot.SetVolume(15.0f);
}

void BatEye::SetBulletGeneratorData()
{
    auto& media = E2D::MediaContent::Instance();

    generator.BulletTexture = media.GetTexture("Bullets");
    generator.BulletAnimationFrames[0] = media.GetAnimationFrames("KamekamehaBulletAlive");
    generator.BulletAnimationFrames[1] = media.GetAnimationFrames("KamekamehaBulletDead");
    generator.BulletAnimationDurations[0] = 0.1f;
    generator.BulletAnimationDurations[1] = 0.1f;
    generator.BulletCollisionType = CollisionType::EnemyBulletCollisionType;
    generator.BulletCollisionMask = CollisionType::PlayerCollisionType | CollisionType::TileCollisionType;
    generator.BulletSpeed = 650.0f;
    generator.BulletSize = 8.0f;
    generator.BulletKnockBack = 500.0f;
    generator.BulletMaxAliveTime = 10.0f;
    generator.BulletType = BulletType::Default;
    generator.BulletPlayerDamage = 1;
    generator.BulletTexturePadding = 23.0f;
    generator.BulletColor = sf::Color(120, 255, 255);
}

void BatEye::Update(float dt)
{
    stateMachine.Update(this, dt);

    animations.CurrentAnimation().Update(sprite, dt);
}

void BatEye::Patrolling(float dt)
{
    if(pathfinding.HasReachedDestination()) //Si ha llegado a la posicion pedimos otra
    {
        pathfinding.RequestNewPath(ID, Position, GetRandomMapPosition());
    }
    else
    {
        sf::Vector2f dir = pathfinding.GetPathNextDirection(Position, patrollingSpeed, dt);
        Velocity += dir * patrollingSpeed * dt;

        SetSpriteOrientation(Velocity);
    }
}

void BatEye::Following(float dt)
{
    pathfinding.RequestNewPath(ID, Position, player.Position);

    if(pathfinding.HasNotReachedDestination())
    {
        sf::Vector2f dir = pathfinding.GetPathNextDirection(Position, followSpeed, dt);

        Velocity += dir * followSpeed * dt;

        SetSpriteOrientation(Velocity);
    }
}

void BatEye::Shooting([[maybe_unused]] float dt)
{
    sf::Vector2f playerDir = player.Position - Position;
    float angle = std::atan2(playerDir.y, playerDir.x);

    lastRotation = rotation;
    rotation = angle;

    if(playerDir.x < 0)
        sprite.SetFlipVertically(true);
    else
        sprite.SetFlipVertically(false);

    if(!hasShoot && animations.CurrentAnimation().GetCurrentFrameIndex() == 9)
    {
        generator.GenerateBullet(Position, sf::getNormalized(playerDir));
        currentNumShots++;
        hasShoot = true;

        PlaySound(sounds[SoundIndex::ShootSound]);

        //Hacer dash aleatorio
        float sign = Random::GlobalInstance.NextFloatSign();
        sf::Vector2f dir = sf::cross(sf::getNormalized(player.Position - Position) * sign);
        Velocity += dir * DASH_POWER;
    }
    else if(animations.CurrentAnimation().HasFinished())
        hasShoot = false;
}

void BatEye::Escaping(float dt)
{
    sf::Vector2f scapeDir = sf::getNormalized(Position - player.Position);

    Velocity += scapeDir * followSpeed * dt;

    currentScapeTime += dt;
}

void BatEye::Dying([[maybe_unused]] float dt)
{
    if(animations.CurrentAnimation().HasFinished())
        Kill();
}

void BatEye::OnEnterFollowing()
{
    animations.ChangeAnimation(sprite, AnimationIndex::FlyIndex);
}

void BatEye::OnEnterShooting()
{
    animations.ChangeAnimation(sprite, AnimationIndex::ShootIndex);
    sprite.SetFlipHorizontally(false);
    currentNumShots = 0;
    hasShoot = false;
}

void BatEye::OnExitShooting()
{
    sprite.SetFlipVertically(false);
    lastRotation = rotation = 0.0f;
}

void BatEye::OnEnterScaping()
{
    animations.ChangeAnimation(sprite, AnimationIndex::FlyIndex);
    currentScapeTime = Random::GlobalInstance.NextFloat(MAX_ESCAPE_TIME * 0.5f);
}

void BatEye::OnEnterDying()
{
    animations.ChangeAnimation(sprite, AnimationIndex::DieIndex);
    PlaySound(sounds[SoundIndex::DieSound]);

    RemoveCollisionMask(CollisionType::PlayerCollisionType);
    RemoveCollisionMask(CollisionType::PlayerBulletCollisionType);
}

bool BatEye::PatrollingToFollowing()
{
    return HasVisionOfPlayer(FOLLOW_DISTANCE_SQUARED);
}

bool BatEye::FollowingToShooting()
{
    return HasVisionOfPlayer(SHOOT_DISTANCE_SQUARED);
}

bool BatEye::ShootingToScaping()
{
    return currentNumShots == maxNumShots;
}

bool BatEye::ScapingToFollowing()
{
    return currentScapeTime >= MAX_ESCAPE_TIME;
}

void BatEye::Draw(float tickPercent)
{
    sprite.setRotation(sf::degrees(sf::lerpAngle(lastRotation, rotation, tickPercent)));

    Enemy::Draw(tickPercent);
}

bool BatEye::ToDying()
{
    return health <= 0.0f; 
}

