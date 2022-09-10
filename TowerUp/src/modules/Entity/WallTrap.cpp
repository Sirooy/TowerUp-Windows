#include "WallTrap.h"

#include "../Physics/PhysicsEngine.h"
#include "../Engine2D/TileMap.h"
#include "../SFMLUtils.hpp"
#include "../Engine2D/Renderer.h"


WallTrap::WallTrap(sf::Vector2f position, TrapDirection direction, float startDelay) : 
    Trap(position, direction, startDelay) { }

void WallTrap::Draw([[maybe_unused]] float tickPercent)
{
    E2D::Renderer::Instance().Draw(sprite);
}

//Se lanza un rayo en direccion contraria a la que apunta la trampa para pegarla a esa pared
void WallTrap::SetTrapIntoWall(const sf::IntRect& rect, TrapDirection direction)
{
    auto& physics = PhysicsEngine::Instance();

    float rotation = static_cast<int32_t>(direction) * sf::PI_OVER_2;
    float halfHeight = static_cast<float>(rect.height) * 0.5f;
    
    sprite.setOrigin(rect.width * 0.5f, rect.height * 0.5f);
    sprite.setRotation(sf::degrees(rotation));
    
    sf::Vector2f rayDirection(0.0f, 0.0f);
    sf::Vector2f positionOffset(0.0f, 0.0f);
    switch (direction)
    {
        case TrapDirection::Top:    
            rayDirection.y =  1.0f; //Apunta hacia arriba por lo que se busca la pared de abajo
            positionOffset.y = -halfHeight - E2D::TileMap::TILE_SIZE * 0.5f;
            break; 
        case TrapDirection::Right:  
            rayDirection.x = -1.0f; //Lo mismo pero con izquierda y derecha
            positionOffset.x =  halfHeight + E2D::TileMap::TILE_SIZE * 0.5f;
            break; 
        case TrapDirection::Bottom: 
            rayDirection.y = -1.0f;
            positionOffset.y =  halfHeight + E2D::TileMap::TILE_SIZE * 0.5f;
            break;
        case TrapDirection::Left:   
            rayDirection.x =  1.0f;
            positionOffset.x = -halfHeight - E2D::TileMap::TILE_SIZE * 0.5f;
            break;
    }

    TileRayCollisionData data = physics.RayCastTileMap(Position, rayDirection);
    if(data.Collision)
    {
        //Centrar la trampa en el tile y luego aplicarle un offset dependiendo de la dirección
        Position = sf::Vector2f
        (
            data.TileCoordX * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f, 
            data.TileCoordY * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f
        );
        Position += positionOffset;
        sprite.setPosition(Position);
    }
}