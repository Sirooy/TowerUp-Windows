#pragma once

#include "Collider.h"

class Circle : public Collider
{
public:
    Circle() = default;
    Circle(float radius);

    CollisionData CollidesWith(sf::Vector2f pos, const Collider* collider, sf::Vector2f colliderPos) const noexcept override;
    CollisionData CollidesWith(sf::Vector2f pos, const Circle* collider, sf::Vector2f colliderPos) const noexcept override;
    CollisionData CollidesWith(sf::Vector2f pos, const AABB* collider, sf::Vector2f colliderPos) const noexcept override;
    RayCollisionData RayCast(sf::Vector2f rayPos, sf::Vector2f rayDir, sf::Vector2f colliderPos) const noexcept override;

    std::pair<TileCollisionData, TileCollisionData> CollidesWith(const DynamicEntity* entity, const CollisionLayer& tileMap) const noexcept override;

    float Radius { 0.0f };
};
