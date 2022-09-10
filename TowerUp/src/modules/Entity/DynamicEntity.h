#pragma once

#include "CollidableEntity.h"
#include "InteractiveElement.h"

//TODO: Agregar un vector de velocidad maxima?
class DynamicEntity : public CollidableEntity, public InteractiveElement
{
public:
    sf::Vector2f Force{ 0.0f, 0.0f }; 
    sf::Vector2f Velocity{ 0.0f, 0.0f };
    sf::Vector2f LastVelocity{ 0.0f, 0.0f };
    sf::Vector2f BaseAcceleration{ 0.0f, 0.0f };
    float Friction { 0.0f };
    float Bounciness { 0.0f };

    DynamicEntity(sf::Vector2f position, Collider* collider);
    DynamicEntity(sf::Vector2f position, float radius);
    DynamicEntity(sf::Vector2f position, float width, float height);

    virtual void OnHorizontalTileCollision(const TileCollisionData& data);
    virtual void OnVerticalTileCollision(const TileCollisionData& data);

    inline float GetMass() const { return mass; }
    inline float GetInverseMass() const { return inverseMass; }
    void SetMass(float newMass);

    virtual ~DynamicEntity() = default;
protected:
    float mass { 10.0f };
    float inverseMass { 1.0f / 10.0f };
};