#pragma once

#include "Enemy.h"
#include "../../BossHealthBar.h"

class BossHealthBar;

class Boss : public Enemy
{
public:
    Boss(sf::Vector2f position, float size, float health, int32_t attack);
    Boss(sf::Vector2f position, float width, float height, float health, int32_t attack);

    void TakeDamage(float amount) override;

    ~Boss();
protected:
    BossHealthBar healthBar;
};