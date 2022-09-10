#pragma once

#include "GenericEntity.h"
#include "HurtZone.h"
#include "../Animation.h"

class Explosion : public GenericEntity
{
public:
    Explosion(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t collisionMask);

    void Update(float dt) override;

    ~Explosion();
protected:
    HurtZone hurtZone;
    Animation animation;
    bool hasDoneDamage;
};