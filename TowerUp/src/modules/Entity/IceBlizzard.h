#pragma once

#include "GenericEntity.h"
#include "HurtZone.h"
#include "../Animation.h"

class IceBlizzard : public GenericEntity
{
public:
    IceBlizzard(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t collisionMask);

    void Update(float dt) override;

    ~IceBlizzard();
protected:
    static constexpr float SPRITE_RADIUS = 24.0f;

    HurtZone hurtZone;
    Animation animation;
    float damageTime { 1.0f };
    float currentDamageTime { 1.0f };
    float duration { 5.0f };
};