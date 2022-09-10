#pragma once

#include "Entity.h"
#include "InteractiveElement.h"
#include "../Engine2D/Sprite.h"
#include "SFML/Graphics.hpp"

class World;

class InteractiveEntity : public Entity, public InteractiveElement
{
public:
    InteractiveEntity(sf::Vector2f position, float activeDistanceSquared);

    virtual void Update(float dt) override;
    virtual void Draw(float tickPercentt) override;

    virtual void OnInteract() = 0;

    virtual ~InteractiveEntity() = default;
protected:
    World& world;
    E2D::Sprite sprite;
    sf::RenderStates renderStates;
    sf::Shader* outlineShader;
    float activeDistanceSquared;
};