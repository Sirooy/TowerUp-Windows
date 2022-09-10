#pragma once
#include "CollidableEntity.h"

class HurtZone : public CollidableEntity
{
public:
    HurtZone(int32_t playerDamage, float knockback, float radius);
    HurtZone(int32_t playerDamage, float knockback, float width, float height);

    void OnCollision(CollidableEntity* other, const CollisionData& data) override;

    int32_t PlayerDamage;
    float Damage;
    float KnockBack;
};