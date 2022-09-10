#include "Entity.h"

Entity::Entity(sf::Vector2f position) : Position(position), LastPosition(position), ID(CurrentID++), IsDead(false) { }

uint32_t Entity::CurrentID = 0;