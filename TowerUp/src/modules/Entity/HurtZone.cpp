#include "HurtZone.h"

HurtZone::HurtZone(int32_t playerDamage, float knockback, float radius) : 
    CollidableEntity(sf::Vector2f(0.0f, 0.0f), radius), PlayerDamage(playerDamage), Damage(0.0f), KnockBack(knockback)
{ 
    CollisionType = CollisionType::HurtZoneCollisionType;
    CollisionMask = CollisionType::PlayerCollisionType;
}

HurtZone::HurtZone(int32_t playerDamage, float knockback, float width, float height) : 
    CollidableEntity(sf::Vector2f(0.0f, 0.0f), width, height), PlayerDamage(playerDamage), Damage(0.0f), KnockBack(knockback)
{ 
    CollisionType = CollisionType::HurtZoneCollisionType;
    CollisionMask = CollisionType::PlayerCollisionType;
}

void HurtZone::OnCollision(CollidableEntity* other, const CollisionData& data)
{
    other->OnCollision(this, data);
}