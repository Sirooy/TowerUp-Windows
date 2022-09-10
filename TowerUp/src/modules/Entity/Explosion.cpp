#include "Explosion.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/PhysicsEngine.h"

Explosion::Explosion(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t collisionMask) : 
    GenericEntity(position), hurtZone(playerDamage, 200.0f, radius), animation(), hasDoneDamage(false)
{   
    auto& media = E2D::MediaContent::Instance();

    animation.SetFrames(media.GetAnimationFrames("DefaultExplosion"));
    animation.SetDuration(0.25f);
    animation.SetLoop(false);
    const sf::IntRect& firstFrame = animation.GetFrame(0);

    float scaleFactor = radius / (firstFrame.width * 0.5f);
    sprite.SetTexture(*media.GetTexture("Bullets"));
    sprite.setOrigin(firstFrame.width * 0.5f, firstFrame.height * 0.5f);
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(position);

    hurtZone.Damage = damage;
    hurtZone.CollisionType = ExplosionCollisionType;
    hurtZone.CollisionMask = collisionMask;
    hurtZone.IsActive = true;
    hurtZone.Position = position;
    hurtZone.LastPosition = position;
    
    PhysicsEngine::Instance().AddStaticEntity(&hurtZone);

    animation.Play(sprite);
}

void Explosion::Update(float dt)
{
    animation.Update(sprite, dt);

    if(animation.HasFinished())
        IsDead = true;

    if(hasDoneDamage)
        hurtZone.IsActive = false;

    hasDoneDamage = true;
}

Explosion::~Explosion()
{
    PhysicsEngine::Instance().RemoveStaticEntity(&hurtZone);
}

