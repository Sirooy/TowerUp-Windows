#include "CollidableEntity.h"
#include "../Physics/AABB.h"
#include "../Physics/Circle.h"

CollidableEntity::CollidableEntity(sf::Vector2f position, Collider* collider) : 
    Entity(position), CollisionType(0), CollisionMask(0), IsActive{true}, collider(collider) { }
CollidableEntity::CollidableEntity(sf::Vector2f position, float radius) : 
    Entity(position), CollisionType(0), CollisionMask(0), IsActive{true}, collider(new Circle(radius)) { }
CollidableEntity::CollidableEntity(sf::Vector2f position, float width, float height) : 
    Entity(position), CollisionType(0), CollisionMask(0), IsActive{true}, collider(new AABB(width, height)) { }

void CollidableEntity::AddCollisionType(uint32_t type)
{
    CollisionType |= type;
}

void CollidableEntity::AddCollisionMask(uint32_t mask)
{
    CollisionMask |= mask;
}

void CollidableEntity::RemoveCollisionType(uint32_t type)
{
    CollisionType &= (~type);
}

void CollidableEntity::RemoveCollisionMask(uint32_t mask)
{
    CollisionMask &= (~mask);
}


//Metodos vacios para que las subclases no esten obligadas a hacer override
void CollidableEntity::OnCollision([[maybe_unused]] Player* other, [[maybe_unused]] const CollisionData& data) { }
void CollidableEntity::OnCollision([[maybe_unused]] Bullet* other, [[maybe_unused]] const CollisionData& data) { }
void CollidableEntity::OnCollision([[maybe_unused]] Enemy* other, [[maybe_unused]] const CollisionData& data) { }
void CollidableEntity::OnCollision([[maybe_unused]] HurtZone* other, [[maybe_unused]] const CollisionData& data) { }
void CollidableEntity::OnCollision([[maybe_unused]] Consumable* other, [[maybe_unused]] const CollisionData& data) { }

CollidableEntity::~CollidableEntity()
{
    delete collider;
}