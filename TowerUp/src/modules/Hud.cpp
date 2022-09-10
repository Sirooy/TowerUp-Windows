#include "Hud.h"
#include "Engine2D/Sprite.h"
#include "Engine2D/Renderer.h"
#include "Entity/World.h"
#include <iostream>


Hud::Hud()
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

    healthBar.SetPosition(sf::Vector2f(8.0f, 8.0f));
    ammoBar.SetPosition(sf::Vector2f(8.0f, screenSize.y - 8.0f));
    items.SetPosition(sf::Vector2f(screenSize.x - ItemBar::MAX_WIDTH - 8.0f, 8.0f));
    moneyBar.SetPosition(healthBar.GetPosition() + sf::Vector2f(0.0f, healthBar.GetHeight() + 8.0f));
    moneyBar.SetSize(32.0f);
}

void Hud::AddBossHealthBar(BossHealthBar* bossHealthBar)
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

    if(bossesHealthBars.size() > 0)
    {
        sf::Vector2f position = bossesHealthBars.back()->GetPosition();
        position.y -= bossHealthBar->GetHeight();
        bossHealthBar->SetPosition(position);
    }
    else
        bossHealthBar->SetPosition(screenSize.x * 0.5f, screenSize.y - bossHealthBar->GetHeight() * 0.5f);

    bossesHealthBars.push_back(bossHealthBar);
}

void Hud::RemoveBossHealthBar(BossHealthBar* bossHealthBar)
{
    auto it = std::find(bossesHealthBars.begin(), bossesHealthBars.end(), bossHealthBar);

    if(it != bossesHealthBars.end())
        bossesHealthBars.erase(it);
}

void Hud::Update(float dt)
{
    ammoBar.Update(dt);
    moneyBar.Update(dt);
    items.Update(dt);
}

void Hud::Draw(float tickPercent)
{
    E2D::Renderer& renderer = E2D::Renderer::Instance();

    healthBar.Draw(renderer);
    ammoBar.Draw(renderer);
    moneyBar.Draw(renderer);
    items.Draw(renderer, tickPercent);
    
    for(std::size_t i = 0; i < bossesHealthBars.size(); ++i)
        bossesHealthBars[i]->Draw(renderer);
}