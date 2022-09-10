#pragma once

#include "Engine2D/Sprite.h"
#include "Engine2D/Text.h"
#include "Animation.h"

class MoneyBar
{
public:
    MoneyBar();

    void SetMoney(int32_t money);
    void Update(float dt);
    void Draw(E2D::Renderer& renderer);

    void SetPosition(sf::Vector2f position);
    void SetSize(float size);
    sf::Vector2f GetPosition();
    float GetHeight();

protected:
    static constexpr float TEXT_SEPARATION = 8.0f;

    E2D::Sprite coinSprite;
    E2D::Text moneyText;
    Animation coinAnimation;
    sf::Vector2f position;
    int32_t money;
    float height;
};