#pragma once

#include "Trap.h"

class WallTrap : public Trap
{
public:
    WallTrap(sf::Vector2f position, TrapDirection direction, float startDelay);

    void Draw(float tickPercent) override;

protected:
    float height;

    void SetTrapIntoWall(const sf::IntRect& rect, TrapDirection direction);
};