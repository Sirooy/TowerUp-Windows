#pragma once

#include <utility>
#include <SFML/System/Vector2.hpp>

struct CollisionData;
struct TileCollisionData;
struct RayCollisionData;
class DynamicEntity;
class CollisionLayer;
class Circle;
class AABB;

class Collider
{
public:
    Collider() = default;

    virtual CollisionData CollidesWith(sf::Vector2f pos, const Collider* collider, sf::Vector2f colliderPos) const noexcept = 0;
    virtual CollisionData CollidesWith(sf::Vector2f pos, const Circle* collider, sf::Vector2f colliderPos) const noexcept = 0;
    virtual CollisionData CollidesWith(sf::Vector2f pos, const AABB* collider, sf::Vector2f colliderPos) const noexcept = 0;
    virtual RayCollisionData RayCast(sf::Vector2f rayPos, sf::Vector2f rayDir, sf::Vector2f colliderPos) const noexcept = 0;

    virtual std::pair<TileCollisionData, TileCollisionData> CollidesWith(const DynamicEntity* entity, const CollisionLayer& tileMap) const noexcept = 0;

    virtual ~Collider() = default;
};