#include "ShooterTrap.h"

#include "../Engine2D/MediaContent.h"
#include "BulletGenerator.h"
#include "CollidableEntity.h"

ShooterTrap::ShooterTrap(sf::Vector2f position, TrapDirection direction, 
    float startDelay, const std::string& framesName, float reloadTime) : 
    WallTrap(position, direction, startDelay), reloadTime(reloadTime), currentReloadTime(0.0f)
{
    auto& media = E2D::MediaContent::Instance();

    shootAnimation.SetFrames(media.GetAnimationFrames(framesName));
    shootAnimation.SetDuration(0.0f);
    shootAnimation.Play(sprite);

    stateMachine.SetState(State::Reloading, &ShooterTrap::UpdateReloading);
    stateMachine.SetState(State::Shooting, &ShooterTrap::UpdateShooting);
    stateMachine.SetState(State::Delay, &ShooterTrap::UpdateDelay);

    SetBulletGeneratorData();
    SetTrapIntoWall(shootAnimation.GetFrame(0), direction);
    SetBulletDirectionAndPosition();

    sound.SetBuffer(*E2D::MediaContent::Instance().GetSoundBuffer("ShooterTrapShoot"));
    sound.SetRelativeToListener(false);
    sound.SetVolume(300.0f);
    sound.SetMinDIstance(10.5f);
    sound.SetAttenuation(1.0f);
}

BulletGenerator& ShooterTrap::GetGenerator()
{
    return generator;
}

void ShooterTrap::SetBulletGeneratorData()
{
    auto& media = E2D::MediaContent::Instance();

    generator.BulletTexture = media.GetTexture("Bullets");
    generator.BulletAnimationFrames[0] = media.GetAnimationFrames("DefaultFireBallBulletAlive");
    generator.BulletAnimationFrames[1] = media.GetAnimationFrames("DefaultFireBallBulletDead");
    generator.BulletAnimationDurations[0] = 0.25f;
    generator.BulletAnimationDurations[1] = 0.25f;
    generator.BulletCollisionType = CollisionType::EnemyBulletCollisionType;
    generator.BulletCollisionMask = CollisionType::PlayerCollisionType | CollisionType::TileCollisionType;
    generator.BulletSpeed = 300.0f;
    generator.BulletSize = 8.0f;
    generator.BulletKnockBack = 500.0f;
    generator.BulletMaxAliveTime = 60.0f;
    generator.BulletType = BulletType::Default;
    generator.BulletPlayerDamage = 1;
    generator.BulletTexturePadding = 12.0f;
    generator.BulletColor = sf::Color(255, 180, 0);
}

void ShooterTrap::SetBulletDirectionAndPosition()
{
    bulletDirection = sf::Vector2f(0.0f, 0.0f);
    switch (direction)
    {
        case TrapDirection::Top:    bulletDirection.y = -1.0f; break;
        case TrapDirection::Right:  bulletDirection.x =  1.0f; break;
        case TrapDirection::Bottom: bulletDirection.y =  1.0f; break;
        case TrapDirection::Left:   bulletDirection.x = -1.0f; break;
    }
    bulletPosition = Position + bulletDirection * generator.BulletSize;
}

void ShooterTrap::Update(float dt)
{
    stateMachine.Update(this, dt);
}

void ShooterTrap::UpdateDelay( float  dt)
{
    startDelay -= dt;

    if(startDelay <= 0.0f)
        stateMachine.ChangeState(State::Shooting);
}


void ShooterTrap::UpdateReloading(float dt)
{
    currentReloadTime += dt;

    if(currentReloadTime >= reloadTime)
    {
        shootAnimation.Play(sprite);
        stateMachine.ChangeState(State::Shooting);
        PlaySound(sound);
    }
}

void ShooterTrap::UpdateShooting(float dt)
{
    shootAnimation.Update(sprite, dt);

    if(shootAnimation.HasFinished())
    {
        currentReloadTime = 0.0f;
        generator.GenerateBullet(bulletPosition, bulletDirection);
        stateMachine.ChangeState(State::Reloading);
    }
}