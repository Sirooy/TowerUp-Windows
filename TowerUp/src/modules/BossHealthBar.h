#pragma once

#include "Engine2D/Sprite.h"

class BossHealthBar
{
public:
    BossHealthBar(float maxHealth);

    void SetCurrentHealth(float dt);
    void SetSize(float size);
    void Draw(E2D::Renderer& renderer);

    void SetPosition(sf::Vector2f position);
    void SetPosition(float x, float y);
    inline sf::Vector2f GetPosition() const { return container.getPosition(); }
    inline float GetHeight() const { return container.GetTextureRect().height; }
protected:
    static constexpr int32_t HEALTH_BAR_OFFSET = 52;
    E2D::Sprite container;
    E2D::Sprite healthBar;

    float maxHealth { 0.0f };
    float height { 0.0f };

    int32_t healthBarWidth { 0 };
public:
};