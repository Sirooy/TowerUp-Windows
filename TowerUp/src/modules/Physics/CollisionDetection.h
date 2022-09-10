#pragma once

#include <utility>
#include "Collider.h"

class CollisionLayer;
class Entity;

sf::Vector2f ClosestPoint(sf::Vector2f aabbPosition, const AABB* aabb, sf::Vector2f pos);

CollisionData CirclevsCircle(const Circle* circle1, sf::Vector2f pos1, const Circle* circle2, sf::Vector2f pos2);
CollisionData CirclevsAABB(const Circle* circle, sf::Vector2f pos1, const AABB* aabb, sf::Vector2f pos2);
CollisionData AABBvsAABB(const AABB* aabb1, sf::Vector2f pos1, const AABB* aabb2, sf::Vector2f pos2);

std::pair<TileCollisionData, TileCollisionData> EntityAABBvsCollisionLayer
    (const DynamicEntity* entity, const AABB* aabb, const CollisionLayer& collisionLayer);
std::pair<TileCollisionData, TileCollisionData> EntityCirclevsCollisionLayer
    (const DynamicEntity* entity, const Circle* circle, const CollisionLayer& collisionLayer);