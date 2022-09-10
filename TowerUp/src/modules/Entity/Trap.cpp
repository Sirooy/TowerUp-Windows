#include "Trap.h"
#include "../Engine2D/MediaContent.h"

Trap::Trap(sf::Vector2f position, TrapDirection direction, float startDelay) : 
    Entity(position), direction(direction), startDelay(startDelay)
{ 
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Traps"));
}
