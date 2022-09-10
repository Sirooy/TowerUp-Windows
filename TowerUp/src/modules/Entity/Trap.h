#pragma once

#include "Entity.h"
#include "InteractiveElement.h"
#include "../Engine2D/Sprite.h"
#include "EntityTypeEnums.h"

class Trap : public Entity, public InteractiveElement
{
public:
    Trap(sf::Vector2f position, TrapDirection direction, float startDelay);

    virtual ~Trap() = default;
protected:
    E2D::Sprite sprite;
    TrapDirection direction;
    float startDelay;
};