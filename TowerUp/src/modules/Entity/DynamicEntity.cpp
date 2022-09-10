#include "DynamicEntity.h"
#include "../Physics/CollisionData.h"

DynamicEntity::DynamicEntity(sf::Vector2f position, Collider* collider) : 
    CollidableEntity(position, collider) { }
DynamicEntity::DynamicEntity(sf::Vector2f position, float radius) : 
    CollidableEntity(position, radius) { }
DynamicEntity::DynamicEntity(sf::Vector2f position, float width, float height) : 
    CollidableEntity(position, width, height) { }

void DynamicEntity::SetMass(float mass)
{
    this->mass = mass;

	if (mass != 0.0f)
		this->inverseMass = 1.0f / mass;
	else
		this->inverseMass  = 0.0f;
}

void DynamicEntity::OnHorizontalTileCollision([[maybe_unused]] const TileCollisionData& data) 
{ 
    Velocity.x = 0;
    Position += data.Normal * data.Penetration;
}

void DynamicEntity::OnVerticalTileCollision([[maybe_unused]] const TileCollisionData& data) 
{
    Velocity.y = 0;
    Position += data.Normal * data.Penetration;
}

