#pragma once

#include "Engine2D/Sprite.h"
#include <iostream>
#include <vector>

class HealthBar
{
public:
    HealthBar();

    void SetMaxHealth(int32_t maxHealth);
    void SetCurrentHealth(int32_t currentHealth);
    void Draw(E2D::Renderer& renderer);

    void SetPosition(sf::Vector2f position);
    sf::Vector2f GetPosition();
    float GetHeight();
protected:
    static constexpr float HEART_SEPARATION = 8.0f;

    enum HeartRectsIndices
    {
        Full  = 0,
        Half  = 1,
        Empty = 2
    };

    std::vector<E2D::Sprite> hearts;
    std::vector<sf::IntRect>* rects; 
    E2D::Texture* texture;
    sf::Vector2f position;
    int32_t heartWidth;
    float height;

};
