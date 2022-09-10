#include "BossHealthBar.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"

BossHealthBar::BossHealthBar(float maxHealth)
{
    this->maxHealth = maxHealth;

    E2D::MediaContent& media = E2D::MediaContent::Instance();

    container.SetTexture(*media.GetTexture("Hud"));
    healthBar.SetTexture(*media.GetTexture("Hud"));
    
    sf::IntRect containerRect = media.GetAnimationFrames("HudBossBarContainer")->front();
    sf::IntRect healthBarRect = media.GetAnimationFrames("HudBossBarHealthBar")->front();
    container.SetTextureRect(containerRect);
    healthBar.SetTextureRect(healthBarRect);
    container.setOrigin(containerRect.width * 0.5f, containerRect.height * 0.5f);
    healthBar.setOrigin(healthBarRect.width * 0.5f, healthBarRect.height * 0.5f);
    healthBarWidth = healthBarRect.width - HEALTH_BAR_OFFSET; //Donde acaba la transparencia y empieza la barra de vida
    height = static_cast<float>(containerRect.height);
}

void BossHealthBar::SetPosition(sf::Vector2f position)
{
    container.setPosition(position);
    healthBar.setPosition(position);
}

void BossHealthBar::SetPosition(float x, float y)
{
    container.setPosition(x, y);
    healthBar.setPosition(x, y);
}

void BossHealthBar::SetCurrentHealth(float currentHealth)
{
    if(currentHealth > maxHealth)
        currentHealth = maxHealth;

    sf::IntRect newRect = healthBar.GetTextureRect();
    float percent = currentHealth / maxHealth;
    newRect.width = std::max(0, static_cast<int32_t>(healthBarWidth * percent) + HEALTH_BAR_OFFSET);

    healthBar.SetTextureRect(newRect);
}

void BossHealthBar::SetSize(float size)
{
    float factor = size / height;
    container.SetScale(factor, factor);
    healthBar.SetScale(factor, factor);
}

void BossHealthBar::Draw(E2D::Renderer& renderer)
{
    renderer.Draw(container);
    renderer.Draw(healthBar);
}