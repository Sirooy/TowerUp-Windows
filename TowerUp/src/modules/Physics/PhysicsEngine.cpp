#include "PhysicsEngine.h"
#include "Collider.h"
#include "../CollisionLayer.h"
#include "../Entity/DynamicEntity.h"

#include <cmath>
#include <algorithm>

PhysicsEngine::PhysicsEngine() { }

void PhysicsEngine::AddStaticEntity(CollidableEntity* staticEntity)
{
    staticEntities.push_back(staticEntity);
}

void PhysicsEngine::AddDynamicEntity(DynamicEntity* dynamicEntity)
{
    dynamicEntities.push_back(dynamicEntity);
}

void PhysicsEngine::SetCollisionLayer(CollisionLayer* collisionLayer)
{
    this->collisionLayer = collisionLayer;
}

void PhysicsEngine::UpdatePhysics(float dt)
{
    Integrate(dt);
    DetectTileCollisions();
    DetectStaticEntityCollisions();
    DetectDynamicEntityCollisions();

    ResolveTileCollisions();
    ResolveEntityCollisions();
}

void PhysicsEngine::RemoveStaticEntity(CollidableEntity* entity)
{
    auto it = std::find(staticEntities.begin(), staticEntities.end(), entity);
    if(it != staticEntities.end())
        staticEntities.erase(it);
}

void PhysicsEngine::RemoveDynamicEntity(DynamicEntity* entity)
{
    auto it = std::find(dynamicEntities.begin(), dynamicEntities.end(), entity);
    dynamicEntities.erase(it);
}

void PhysicsEngine::ClearEntities()
{
    staticEntities.clear();
    dynamicEntities.clear();
    entityCollisions.clear();
    horizontalTileCollisions.clear();
    verticalTileCollisions.clear();
}

void PhysicsEngine::Integrate(float dt)
{
    for (auto entity : dynamicEntities)
	{
		sf::Vector2f acceleration = entity->BaseAcceleration + entity->Force * entity->GetInverseMass();

        entity->Velocity += acceleration * dt;

        entity->LastPosition = entity->Position;
        entity->Position += entity->Velocity * dt;

        entity->LastVelocity = entity->Velocity * dt;
        entity->Velocity *= std::pow(entity->Friction, dt);
        //entity->Velocity -= entity->Velocity * entity->Friction * dt;

		entity->Force = sf::Vector2f{ 0.0f, 0.0f };
	}
}

void PhysicsEngine::DetectTileCollisions()
{
    for(auto entity : dynamicEntities)
    {
        if((entity->CollisionType & collisionLayer->CollisionMask) == 0 ||
            (entity->CollisionMask & collisionLayer->CollisionType) == 0)
               continue;

        auto c = entity->GetCollider()->CollidesWith(entity, *collisionLayer);

        if(c.first.Collision)
        {
            c.first.entity = entity;
            horizontalTileCollisions.push_back(c.first);
        }

        if(c.second.Collision)
        {
            c.second.entity = entity;
            verticalTileCollisions.push_back(c.second);
        }
    }
}

void PhysicsEngine::DetectStaticEntityCollisions()
{
    for(auto a : staticEntities)
    {
        if(!a->IsActive)
            continue;

        for(auto b : dynamicEntities)
        {
            if(!b->IsActive)
                continue;

            CheckCollision(a, b);
        }
    }
}

void PhysicsEngine::DetectDynamicEntityCollisions()
{
    const int32_t size = static_cast<int32_t>(dynamicEntities.size());

    for(int32_t i = 0; i < size - 1; ++i)
    {
        auto a = dynamicEntities[i];
        if(!a->IsActive)
            continue;

        for(int32_t j = i + 1; j < size; ++j)
        {
			auto b = dynamicEntities[j];
            if(!b->IsActive)
                continue;

            CheckCollision(a, b);
        }
    }
}

void PhysicsEngine::CheckCollision(CollidableEntity* a, CollidableEntity* b)
{
    if((a->CollisionType & b->CollisionMask) == 0 ||
       (a->CollisionMask & b->CollisionType) == 0)
       return;

    CollisionData collision = a->GetCollider()->CollidesWith(
        a->Position, b->GetCollider(), b->Position);

    if(collision.Collision)
    {
        collision.EntityA = a;
        collision.EntityB = b;
        entityCollisions.push_back(collision);
    }
}

void PhysicsEngine::ResolveTileCollisions()
{
    for(auto& c : horizontalTileCollisions)
    {
        c.entity->OnHorizontalTileCollision(c);
    }

    for(auto& c : verticalTileCollisions)
    {
        c.entity->OnVerticalTileCollision(c);
    }

    verticalTileCollisions.clear();
    horizontalTileCollisions.clear();
}

void PhysicsEngine::ResolveEntityCollisions()
{
    for (auto& c : entityCollisions)
    {
        c.EntityB->OnCollision(c.EntityA, c);
        c.Normal = -c.Normal;
        c.EntityA->OnCollision(c.EntityB, c);
    }

    entityCollisions.clear();
}

RayCollisionData PhysicsEngine::RayCastEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask)
{
    RayCollisionData data;
    data.Distance = std::numeric_limits<float>::max();

    for(auto e : staticEntities)
    {
        if((e->CollisionType & mask) == 0)
            continue;

        RayCollisionData newData = e->GetCollider()->RayCast(position, direction, e->Position);
        if(newData.Collision && newData.Distance < data.Distance)
            data = newData;
        
    }

    for(auto e :  dynamicEntities)
    {
        if((e->CollisionType & mask) == 0)
            continue;

        RayCollisionData newData = e->GetCollider()->RayCast(position, direction, e->Position);
        if(newData.Collision && newData.Distance < data.Distance)
            data = newData;
    }

    return data;
}

RayCollisionData PhysicsEngine::RayCastDynamicEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask)
{
    RayCollisionData data;
    data.Distance = std::numeric_limits<float>::max();

    for(auto e :  dynamicEntities)
    {
        if((e->CollisionType & mask) == 0)
            continue;

        RayCollisionData newData = e->GetCollider()->RayCast(position, direction, e->Position);
        if(newData.Collision && newData.Distance < data.Distance)
            data = newData;
    }

    return data;
}

RayCollisionData PhysicsEngine::RayCastStaticEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask)
{
    RayCollisionData data;
    data.Distance = std::numeric_limits<float>::max();

    for(auto e : staticEntities)
    {
        if((e->CollisionType & mask) == 0)
            continue;

        RayCollisionData newData = e->GetCollider()->RayCast(position, direction, e->Position);
        if(newData.Collision && newData.Distance < data.Distance)
            data = newData;
    }

    return data;
}

TileRayCollisionData PhysicsEngine::RayCastTileMap(sf::Vector2f position, sf::Vector2f dir)
{
    TileRayCollisionData data;

    float mx = std::numeric_limits<float>::max();
    if(dir.x != 0.0f)
        mx = (dir.y / dir.x);

    float my = std::numeric_limits<float>::max();
    if(dir.y != 0.0f)
        my = (dir.x / dir.y);

    sf::Vector2f rayDistance(0.0f, 0.0f);
    sf::Vector2f rayStepSize
    (
        sqrt(1 + mx * mx),
        sqrt(1 + my * my)
    );

    sf::Vector2i tileCoords
    (
        collisionLayer->GetScaledCoord(position.x),
        collisionLayer->GetScaledCoord(position.y)
    );

    sf::Vector2i stepDir;
    if(dir.x > 0.0f)
    {
        stepDir.x =  1;
        rayDistance.x = ((tileCoords.x + 1) * E2D::TileMap::TILE_SIZE - position.x) * rayStepSize.x;
    }
    else
    {
        stepDir.x = -1;
        rayDistance.x = (position.x - (tileCoords.x * E2D::TileMap::TILE_SIZE)) * rayStepSize.x;
    }

    if(dir.y > 0.0f)
    {
        stepDir.y =  1;
        rayDistance.y = ((tileCoords.y + 1) * E2D::TileMap::TILE_SIZE - position.y) * rayStepSize.y;
    }
    else
    {
        stepDir.y = -1;
        rayDistance.y = (position.y - (tileCoords.y * E2D::TileMap::TILE_SIZE)) * rayStepSize.y;
    }

    while(!collisionLayer->IsOutOfBounds(tileCoords.x, tileCoords.y))
    {
        if(rayDistance.x < rayDistance.y)
        {
            tileCoords.x += stepDir.x;
            data.Distance = rayDistance.x;
            rayDistance.x += rayStepSize.x * E2D::TileMap::TILE_SIZE;
        }
        else
        {
            tileCoords.y += stepDir.y;
            data.Distance = rayDistance.y;
            rayDistance.y += rayStepSize.y * E2D::TileMap::TILE_SIZE;
        }

        int32_t tileType = collisionLayer->Get(tileCoords.x, tileCoords.y);
        if(tileType != 0)
        {
            data.Collision = true;
            data.TileType = tileType;
            data.TileCoordX = tileCoords.x;
            data.TileCoordY = tileCoords.y;

            return data;
        }    
    }

    return data;
}

TileRayCollisionData PhysicsEngine::RayCastTileMapHorizontal(sf::Vector2f position, int32_t direction)
{
    TileRayCollisionData data;

    int32_t startX = collisionLayer->GetScaledCoord(position.x);

    int32_t x = startX;
    int32_t y = collisionLayer->GetScaledCoord(position.y);

    while(!collisionLayer->IsOutOfBounds(x, y))
    {
        x += direction;

        int32_t tileType = collisionLayer->Get(x, y);
        if(tileType != 0)
        {
            data.Collision = true;
            data.TileType = tileType;
            data.TileCoordX = x;
            data.TileCoordY = y;
            if(direction > 0)
                data.Distance = (x - startX - 1) * E2D::TileMap::TILE_SIZE + 
                    ((startX + 1) * E2D::TileMap::TILE_SIZE - position.x);
            else
                data.Distance = (startX - x - 1) * E2D::TileMap::TILE_SIZE +
                    (position.x - startX * E2D::TileMap::TILE_SIZE);

            return data; 
        }
    }

    return data;
}

TileRayCollisionData PhysicsEngine::RayCastTileMapVertical(sf::Vector2f position, int32_t direction)
{
    TileRayCollisionData data;

    int32_t startY = collisionLayer->GetScaledCoord(position.y);

    int32_t x = collisionLayer->GetScaledCoord(position.x);
    int32_t y = startY;

    while(!collisionLayer->IsOutOfBounds(x, y))
    {
        y += direction;

        int32_t tileType = collisionLayer->Get(x, y);
        if(tileType != 0)
        {
            data.Collision = true;
            data.TileType = tileType;
            data.TileCoordX = x;
            data.TileCoordY = y;
            if(direction > 0)
                data.Distance = (y - startY - 1) * E2D::TileMap::TILE_SIZE + 
                    ((startY + 1) * E2D::TileMap::TILE_SIZE - position.y);
            else
                data.Distance = (startY - y - 1) * E2D::TileMap::TILE_SIZE +
                    (position.y - startY * E2D::TileMap::TILE_SIZE);

            return data; 
        }
    }

    return data;
}

PhysicsEngine PhysicsEngine::SingletonInstance {};