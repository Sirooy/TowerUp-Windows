#pragma once

#include "Collider.h"

//Centered AABB
class AABB : public Collider
{
public:
    AABB() = default;
    AABB(float size);
    AABB(float width, float height);

    CollisionData CollidesWith(sf::Vector2f pos, const Collider* collider, sf::Vector2f colliderPos) const noexcept override;
    CollisionData CollidesWith(sf::Vector2f pos, const Circle* collider, sf::Vector2f colliderPos) const noexcept override;
    CollisionData CollidesWith(sf::Vector2f pos, const AABB* collider, sf::Vector2f colliderPos) const noexcept override;
    RayCollisionData RayCast(sf::Vector2f rayPos, sf::Vector2f rayDir, sf::Vector2f colliderPos) const noexcept override;

    std::pair<TileCollisionData, TileCollisionData> CollidesWith(const DynamicEntity* entity, const CollisionLayer& tileMap) const noexcept override;

    float Width { 0.0f }, Height { 0.0f };
};