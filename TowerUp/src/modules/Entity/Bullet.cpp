#include "Bullet.h"

#include "BulletGenerator.h"
#include "../Engine2D/Renderer.h"
#include "../Physics/CollisionData.h"
#include "../SFMLUtils.hpp"

//! BASE BULLET
Bullet::Bullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks) : 
    DynamicEntity(position, generator->BulletSize), Generator(generator), updateFunc(&Bullet::UpdateAlive), animations{}, sprite(),
    Direction(direction), Damage(generator->BulletDamage), PlayerDamage(generator->BulletPlayerDamage), MaxAliveTime(generator->BulletMaxAliveTime),
    AliveTime(0.0f), KnockBack(generator->BulletKnockBack), Size(generator->BulletSize), HasCollided(false), executeCallbacks(executeCallbacks)
{
    Friction = generator->BulletFriction;
    Velocity = direction * generator->BulletSpeed;
    BaseAcceleration = generator->BulletBaseAcceleration;
    CollisionType = generator->BulletCollisionType;
    CollisionMask = generator->BulletCollisionMask;

    auto& firstAnimationFrame = generator->BulletAnimationFrames[AnimationIndex::AliveIndex]->at(0);
    float scale = (generator->BulletSize * 2.0f) / 
        (firstAnimationFrame.width - generator->BulletTexturePadding * 2.0f);
    sprite.SetTexture(*generator->BulletTexture);
    sprite.SetColor(generator->BulletColor);
    sprite.setRotation(sf::degrees(std::atan2(Velocity.y, Velocity.x)));
    sprite.setOrigin(sf::Vector2f(firstAnimationFrame.width * 0.5f, firstAnimationFrame.height * 0.5f));
    sprite.setScale(scale, scale);

    Animation& alive = animations[AnimationIndex::AliveIndex];
    alive.SetFrames(generator->BulletAnimationFrames[AnimationIndex::AliveIndex]);
    alive.SetDuration(generator->BulletAnimationDurations[AnimationIndex::AliveIndex]);

    Animation& dead = animations[AnimationIndex::DeadIndex];
    dead.SetFrames(generator->BulletAnimationFrames[AnimationIndex::DeadIndex]);
    dead.SetDuration(generator->BulletAnimationDurations[AnimationIndex::DeadIndex]);
    dead.SetLoop(false);

    animations.ChangeAnimation(sprite, AnimationIndex::AliveIndex);
    CreateSounds();
}

void Bullet::CreateSounds()
{
    /*
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& enemy = sounds[SoundIndex::CollisionEnemy];
    enemy.SetBuffer(*media.GetSoundBuffer("BulletCollisionEnemy"));
    enemy.SetRelativeToListener(true);
    enemy.SetVolume(35.0f);

    
    auto& tile = sounds[SoundIndex::CollisionTile];
    tile.SetBuffer(*media.GetSoundBuffer("BulletCollisionTile"));
    tile.SetRelativeToListener(true);
    tile.SetVolume(35.0f);

    auto& magic = sounds[SoundIndex::CollisionMagic];
    magic.SetBuffer(*media.GetSoundBuffer("FireMagicBulletCollisionTile"));
    magic.SetRelativeToListener(true);
    magic.SetVolume(35.0f);
    */
}

void Bullet::Update(float dt)
{
    (this->*updateFunc)(dt);
}

void Bullet::UpdateAlive(float dt)
{
    AliveTime += dt;

    if(AliveTime >= MaxAliveTime)
    {
        IsDead = true;

        if(executeCallbacks)
        {
            for(auto& c : Generator->OnReachMaxAliveTimeCallbacks)
                c(this);
        }
    }
    else
        animations.CurrentAnimation().Update(sprite, dt);

    //? Añadir eventos de update
}

void Bullet::UpdateDead(float dt)
{
    animations.CurrentAnimation().Update(sprite, dt);

    if(animations.CurrentAnimation().HasFinished())
        IsDead = true;
}

void Bullet::KillBulet()
{
   updateFunc = &Bullet::UpdateDead;
   animations.ChangeAnimation(sprite, AnimationIndex::DeadIndex);
   auto& firstAnimationFrame = Generator->BulletAnimationFrames[AnimationIndex::DeadIndex]->at(0);
   sprite.setOrigin(sf::Vector2f(firstAnimationFrame.width * 0.5f, firstAnimationFrame.height * 0.5f));
   BaseAcceleration = sf::Vector2f(0.0f, 0.0f);
   Velocity = sf::Vector2f(0.0f, 0.0f);
   CollisionType = 0;
   CollisionMask = 0;
}

void Bullet::Draw(float tickPercent)
{
    sf::Vector2f position = GetInterpolatedPosition(tickPercent);

    E2D::Renderer::Instance().Draw(position, sprite);
}


void Bullet::OnCollision(CollidableEntity* other, const CollisionData& data)
{
    other->OnCollision(this, data);
}

void Bullet::OnCollision([[maybe_unused]] Player* player, const CollisionData& data)
{
    Collision(data.Normal, data.Penetration);
}

void Bullet::OnCollision([[maybe_unused]] Enemy* enemy, const CollisionData& data)
{
    Collision(data.Normal, data.Penetration);
    if(executeCallbacks)
    {
        for(auto& c : Generator->OnCollisionWithEnemyCallbacks)
            c(this, enemy, data);
    }

    //PlaySound(sounds[SoundIndex::CollisionEnemy]);
}

void Bullet::OnCollision([[maybe_unused]] Bullet* other, const CollisionData& data)
{
    Collision(data.Normal, data.Penetration);
    //PlaySound(sounds[SoundIndex::CollisionTile]);
}

void Bullet::OnHorizontalTileCollision(const TileCollisionData& data)
{
    Collision(data.Normal, data.Penetration);

    if(executeCallbacks)
    {
        for(auto& c : Generator->OnCollisionWithTileCallbacks)
            c(this, data);
    }
}

void Bullet::OnVerticalTileCollision(const TileCollisionData& data)
{
    Collision(data.Normal, data.Penetration);

    if(executeCallbacks)
    {
        for(auto& c : Generator->OnCollisionWithTileCallbacks)
            c(this, data);
    }
}

void Bullet::Collision(sf::Vector2f normal, float penetration)
{
    if(penetration > Size)
        Position += normal * (penetration - Size);
    else
        Position -= normal * (Size - penetration);

    sprite.setRotation(sf::degrees(std::atan2(normal.y, normal.x)));
    KillBulet();
}

//! RUBBER BULLET

RubberBullet::RubberBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f velocity, bool executeCallbacks) : 
    Bullet(generator, position, velocity, executeCallbacks), numBounces { 3 } { }

void RubberBullet::OnHorizontalTileCollision(const TileCollisionData& data)
{
    
    if(numBounces == 0)
    {
        if(data.Penetration > Size)
            Position.x += data.Normal.x * (data.Penetration - Size);
        else
            Position.x -= data.Normal.x * (Size - data.Penetration);

        sprite.setRotation(sf::degrees(std::atan2(data.Normal.y, data.Normal.x)));
        KillBulet();

        if(executeCallbacks)
        {
            for(auto& c : Generator->OnCollisionWithTileCallbacks)
                c(this, data);
        }
    }
    else
    {
        Position.x += (data.Normal.x * data.Penetration) + (data.Normal.x * BOUNCE_SEPARATION);
        Velocity.x = -Velocity.x;
        sprite.setRotation(sf::degrees(std::atan2(Velocity.y, Velocity.x)));
    }
    
    --numBounces;
}

void RubberBullet::OnVerticalTileCollision(const TileCollisionData& data)
{
    sprite.setRotation(sf::degrees(std::atan2(data.Normal.y, data.Normal.x)));
    if(numBounces == 0)
    {
        if(data.Penetration > Size)
            Position.y += data.Normal.y * (data.Penetration - Size);
        else
            Position.y -= data.Normal.y * (Size - data.Penetration);
        sprite.setRotation(sf::degrees(std::atan2(data.Normal.y, data.Normal.x)));
        KillBulet();
        
        if(executeCallbacks)
        {
            for(auto& c : Generator->OnCollisionWithTileCallbacks)
                c(this, data);
        }
    }
    else
    {
        Position.y += (data.Normal.y * data.Penetration) + (data.Normal.y * BOUNCE_SEPARATION);
        Velocity.y = -Velocity.y;
        sprite.setRotation(sf::degrees(std::atan2(Velocity.y, Velocity.x)));
    }

    --numBounces;
}
