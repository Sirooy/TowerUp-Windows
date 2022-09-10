#pragma once

#include <cstdint>
#include <SFML/System/Vector2.hpp>

class CollidableEntity;
class DynamicEntity;

struct CollisionData
{
    sf::Vector2f Normal { 0.0f, 0.0f };
    CollidableEntity* EntityA { nullptr };
    CollidableEntity* EntityB { nullptr };
    float Penetration { 0.0f };
    bool Collision { false };
};

struct TileCollisionData
{
    sf::Vector2f Normal { 0.0f, 0.0f };
    DynamicEntity* entity { nullptr };
    int32_t TileType { 0 };
    float Penetration { -1.0f };
    bool Collision { false };
};

struct RayCollisionData
{
    CollidableEntity* entity { nullptr };
    float Distance { 0.0f };
    bool Collision { false };
};

struct TileRayCollisionData
{
    int32_t TileType { 0 };
    int32_t TileCoordX { 0 };
    int32_t TileCoordY { 0 };
    float Distance { 0.0f };
    bool Collision { false };
};