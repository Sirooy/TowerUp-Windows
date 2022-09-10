#pragma once

#include "Entity.h"
#include "InteractiveElement.h"
#include "../Engine2D/Sprite.h"

class GenericEntity : public Entity, public InteractiveElement
{
public:
    GenericEntity(sf::Vector2f position);

    virtual void Draw(float tickPercent) override;

    virtual ~GenericEntity() = default;
protected:
    E2D::Sprite sprite;
};