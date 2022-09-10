#include "IceBlizzard.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/PhysicsEngine.h"

IceBlizzard::IceBlizzard(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t collisionMask) : 
    GenericEntity(position), hurtZone(playerDamage, 200.0f, radius), animation()
{   
    auto& media = E2D::MediaContent::Instance();
    
    animation.SetFrames(media.GetAnimationFrames("IceBlizzard"));
    animation.SetDuration(0.25f);
    const sf::IntRect& firstFrame = animation.GetFrame(0);

    float scaleFactor = radius / SPRITE_RADIUS;
    sprite.SetTexture(*media.GetTexture("Bullets"));
    sprite.setOrigin(firstFrame.width * 0.5f, firstFrame.height * 0.5f);
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(position);
    sprite.SetAlpha(255 / 4);

    hurtZone.Damage = damage;
    hurtZone.PlayerDamage = playerDamage;
    hurtZone.CollisionType = ExplosionCollisionType;
    hurtZone.CollisionMask = collisionMask;
    hurtZone.IsActive = true;
    hurtZone.Position = position;
    hurtZone.LastPosition = position;
    
    PhysicsEngine::Instance().AddStaticEntity(&hurtZone);

    animation.Play(sprite);
}

void IceBlizzard::Update(float dt)
{
    animation.Update(sprite, dt);
    currentDamageTime += dt;

    if(currentDamageTime >= damageTime)
    {
        currentDamageTime = 0.0f;
        hurtZone.IsActive = true;
    }
    else
        hurtZone.IsActive = false;

    duration -= dt;

    if(duration <= 0.0f)
        IsDead = true;
}

IceBlizzard::~IceBlizzard()
{
    PhysicsEngine::Instance().RemoveStaticEntity(&hurtZone);
}
