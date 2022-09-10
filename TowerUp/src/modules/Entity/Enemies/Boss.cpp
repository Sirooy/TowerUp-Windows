#include "Boss.h"
#include "../World.h"
#include "../../Hud.h"

Boss::Boss(sf::Vector2f position, float size, float health, int32_t attack) : 
    Enemy(position, size, health, attack), healthBar(health) 
{
    World::Instance().GetHud()->AddBossHealthBar(&healthBar);
}

Boss::Boss(sf::Vector2f position, float width, float height, float health, int32_t attack) : 
    Enemy(position, width, height, health, attack), healthBar(health) 
{ 
    World::Instance().GetHud()->AddBossHealthBar(&healthBar);
}

void Boss::TakeDamage(float amount)
{
    Enemy::TakeDamage(amount);
    healthBar.SetCurrentHealth(health);
}

Boss::~Boss()
{
    World::Instance().GetHud()->RemoveBossHealthBar(&healthBar);
}
