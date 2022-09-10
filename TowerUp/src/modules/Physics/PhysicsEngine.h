#pragma once

#include <vector>
#include "CollisionData.h"

class CollidableEntity;
class DynamicEntity;
class CollisionLayer;

class PhysicsEngine
{
public:
    inline static sf::Vector2f BaseGravity = sf::Vector2f(0.0f, 3000.0f);

    void AddStaticEntity(CollidableEntity* staticEntity);
    void AddDynamicEntity(DynamicEntity* dynamicEntity);
    void SetCollisionLayer(CollisionLayer* collisionLayer);
    void RemoveStaticEntity(CollidableEntity* staticEntity);
    void RemoveDynamicEntity(DynamicEntity* entity);
    RayCollisionData RayCastEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask);
    RayCollisionData RayCastDynamicEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask);
    RayCollisionData RayCastStaticEntities(sf::Vector2f position, sf::Vector2f direction, uint32_t mask);
    TileRayCollisionData RayCastTileMap(sf::Vector2f position, sf::Vector2f direction);
    TileRayCollisionData RayCastTileMapHorizontal(sf::Vector2f position, int32_t direction);
    TileRayCollisionData RayCastTileMapVertical(sf::Vector2f position, int32_t direction);
    void ClearEntities();

    void UpdatePhysics(float dt);

    inline CollisionLayer* GetCollisionLayer() { return collisionLayer; }
    inline static PhysicsEngine& Instance() { return SingletonInstance; }

protected:
    std::vector<CollidableEntity*> staticEntities {};
    std::vector<DynamicEntity*> dynamicEntities {};
    std::vector<CollisionData> entityCollisions {};
    std::vector<TileCollisionData> horizontalTileCollisions {};
    std::vector<TileCollisionData> verticalTileCollisions {};
    CollisionLayer* collisionLayer;

    PhysicsEngine();
    void Integrate(float dt);
    void DetectTileCollisions();
    void DetectStaticEntityCollisions();
    void DetectDynamicEntityCollisions();
    void CheckCollision(CollidableEntity* a, CollidableEntity* b);

    void ResolveTileCollisions();
    void ResolveEntityCollisions();

    static PhysicsEngine SingletonInstance;
};