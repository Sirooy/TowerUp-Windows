#pragma once

#include "Entity.h"

struct CollisionData;
struct TileCollisionData;
class Collider;
class Player;
class Bullet;
class Enemy;
class HurtZone;
class Consumable;

enum CollisionType : uint32_t
{
    NoneType                  = 0,
    TileCollisionType         = 1 << 1,
    PlayerCollisionType       = 1 << 2,
    EnemyCollisionType        = 1 << 3,
    PlayerBulletCollisionType = 1 << 4,
    EnemyBulletCollisionType  = 1 << 5,
    HurtZoneCollisionType     = 1 << 6,
    ConsumableCollisionType   = 1 << 7,
    ExplosionCollisionType    = 1 << 8
};

class CollidableEntity : public Entity
{
public:
    uint32_t CollisionType; //La entidad es de tipo X
    uint32_t CollisionMask; //La entidad colisiona con tipos X
    bool IsActive;

    CollidableEntity(sf::Vector2f position, Collider* collider);
    CollidableEntity(sf::Vector2f position, float radius);
    CollidableEntity(sf::Vector2f position, float width, float height);

    inline Collider* GetCollider() const { return collider; }
    void AddCollisionType(uint32_t type);
    void AddCollisionMask(uint32_t mask);
    void RemoveCollisionType(uint32_t type);
    void RemoveCollisionMask(uint32_t mask);

    virtual void OnCollision(CollidableEntity* other, const CollisionData& data) = 0;
    virtual void OnCollision(Player* other, const CollisionData& data);
    virtual void OnCollision(Bullet* other, const CollisionData& data);
    virtual void OnCollision(Enemy* other, const CollisionData& data);
    virtual void OnCollision(HurtZone* other, const CollisionData& data);
    virtual void OnCollision(Consumable* other, const CollisionData& data);

    virtual ~CollidableEntity();

protected:
    Collider* collider;
};