#include "AABB.h"
#include "CollisionData.h"
#include "CollisionDetection.h"

#include <cmath>

AABB::AABB(float size) : Width{ size }, Height{ size } { }
AABB::AABB(float width, float height) : Width{ width }, Height{ height } { }

CollisionData AABB::CollidesWith(sf::Vector2f pos, const Collider* collider, sf::Vector2f colliderPos) const noexcept
{
    return collider->CollidesWith(colliderPos, this, pos);
}

CollisionData AABB::CollidesWith(sf::Vector2f pos, const Circle* collider, sf::Vector2f colliderPos) const noexcept
{
    CollisionData data = CirclevsAABB(collider, colliderPos, this, pos);
    data.Normal = -data.Normal;
    return data;
}

CollisionData AABB::CollidesWith(sf::Vector2f pos, const AABB* collider, sf::Vector2f colliderPos) const noexcept
{
    return AABBvsAABB(this, pos, collider, colliderPos);
}

std::pair<TileCollisionData, TileCollisionData> AABB::CollidesWith(const DynamicEntity* entity, const CollisionLayer& collisionLayer) const noexcept
{
    return EntityAABBvsCollisionLayer(entity, this, collisionLayer);
}

RayCollisionData AABB::RayCast(sf::Vector2f rayPos, sf::Vector2f rayDir, sf::Vector2f colliderPos) const noexcept
{
    RayCollisionData data;

    if(rayDir.y == 0.0f)
    {
        if(rayPos.y < colliderPos.y - Height * 0.5f || 
           rayPos.y > colliderPos.y + Height * 0.5f)
           return data;

        float tMin = ((colliderPos.x + Width * 0.5f) - rayPos.x) / rayDir.x;
        float tMax = ((colliderPos.x - Width * 0.5f) - rayPos.x) / rayDir.x;

        if(tMin > tMax)
            std::swap(tMin, tMax);

        if(tMax > 0.0f)
        {
            data.Collision = true;
            data.Distance = (tMin < 0.0f) ? 0.0f : tMin;
        }
    }
    else if(rayDir.x == 0.0f)
    {
        if(rayPos.x < colliderPos.x - Width * 0.5f ||
           rayPos.x > colliderPos.x + Width * 0.5f)
           return data;


        float tMin = ((colliderPos.y + Height * 0.5f) - rayPos.y) / rayDir.y;
        float tMax = ((colliderPos.y - Height * 0.5f) - rayPos.y) / rayDir.y;

        if(tMin > tMax)
            std::swap(tMin, tMax);

        if(tMax > 0.0f)
        {
            data.Collision = true;
            data.Distance = (tMin < 0.0f) ? 0.0f : tMin;
        }
    }
    else
    {
        float txMin = ((colliderPos.x + Width * 0.5f) - rayPos.x) / rayDir.x;
        float txMax = ((colliderPos.x - Width * 0.5f) - rayPos.x) / rayDir.x;

        if(txMin > txMax)
            std::swap(txMin, txMax);

        float tyMin = ((colliderPos.y + Height * 0.5f) - rayPos.y) / rayDir.y;
        float tyMax = ((colliderPos.y - Height * 0.5f) - rayPos.y) / rayDir.y;

        if(tyMin > tyMax)
            std::swap(tyMin, tyMax);

        float tMin = std::max(txMin, tyMin);
        float tMax = std::min(txMax, tyMax);

        if(tMax > 0.0f && tMin < tMax)
        {
            data.Collision = true;
            data.Distance = (tMin < 0.0f) ? 0.0f : tMin;
        }
    }

    return data;
}