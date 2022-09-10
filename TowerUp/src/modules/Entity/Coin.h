#pragma once

#include "Consumable.h"

class Coin : public Consumable
{
public:
    Coin(sf::Vector2f position, int32_t value, sf::Color color);

    void OnCollision(Player* player, const CollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;
    void OnHorizontalTileCollision(const TileCollisionData& data) override;

public:
    static constexpr float WIDTH = 30.0f;
    static constexpr float HEIGHT = 30.0f;
    static constexpr float MAX_ALIVE_TIME = 10.0f;

    int32_t value;

};