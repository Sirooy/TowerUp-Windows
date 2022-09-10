#include "GenericEntity.h"
#include "../Engine2D/Renderer.h"

GenericEntity::GenericEntity(sf::Vector2f position) : 
    Entity(position), InteractiveElement() { }

void GenericEntity::Draw([[maybe_unused]]float tickPercent)
{
    E2D::Renderer::Instance().Draw(sprite);
}