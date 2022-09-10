#include "Circle.h"
#include "CollisionData.h"
#include "CollisionDetection.h"

#include "../SFMLUtils.hpp"

Circle::Circle(float radius) : Radius{ radius } { }

CollisionData Circle::CollidesWith(sf::Vector2f pos, const Collider* collider, sf::Vector2f colliderPos) const noexcept
{
    return collider->CollidesWith(colliderPos, this, pos);
}

CollisionData Circle::CollidesWith(sf::Vector2f pos, const Circle* collider, sf::Vector2f colliderPos) const noexcept 
{
    return CirclevsCircle(this, pos, collider, colliderPos);
}

CollisionData Circle::CollidesWith(sf::Vector2f pos, const AABB* collider, sf::Vector2f colliderPos) const noexcept
{
    return CirclevsAABB(this, pos, collider, colliderPos);
}

std::pair<TileCollisionData, TileCollisionData> Circle::CollidesWith(const DynamicEntity* entity, const CollisionLayer& collisionLayer) const noexcept
{
    return EntityCirclevsCollisionLayer(entity, this, collisionLayer);
}

RayCollisionData Circle::RayCast(sf::Vector2f rayPos, sf::Vector2f rayDir, sf::Vector2f colliderPos) const noexcept
{
    RayCollisionData data;

    sf::Vector2f rayToCenter = colliderPos - rayPos;
    float projection = sf::dot(rayToCenter, rayDir);

    if(projection < 0.0f)
        return data;

    sf::Vector2f point = rayPos + rayDir * projection;
    sf::Vector2f centerToPoint = point - colliderPos;
    float squaredRadius = Radius * Radius;
    float squaredLength = sf::lengthSquared(centerToPoint);

    if(sf::lengthSquared(rayToCenter) < squaredRadius)
    {
        data.Collision = true;
        data.Distance = 0.0f;
    }
    else if(squaredLength < squaredRadius) //Contains the point
    {
        data.Collision = true;
        data.Distance = projection - (sqrt(squaredRadius - squaredLength));
    }

    return data;
}
