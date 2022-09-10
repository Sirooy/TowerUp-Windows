#pragma once

#include "Consumable.h"

class Heart : public Consumable
{
public:
    Heart(sf::Vector2f position, int32_t amountOfHealing, sf::Color color);

    void OnCollision(Player* player, const CollisionData& data) override;
    void OnHorizontalTileCollision(const TileCollisionData& data) override;
protected:
    static constexpr float WIDTH = 30.0f;
    static constexpr float HEIGHT = 30.0f;
    static constexpr float MAX_ALIVE_TIME = 10.0f;

    int32_t amountOfHealing;
};