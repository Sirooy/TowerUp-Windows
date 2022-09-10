#pragma once

#include "SFML/Window.hpp"
#include "Engine2D/Sprite.h"
#include "Engine2D/Text.h"

class Button
{
public:
    Button();

    void SetDefaultRect(const sf::IntRect& defaultRect);
    void SetHoverRect(const sf::IntRect& hoverRect);
    void SetSize(float width, float height);
    void SetText(const std::string& string);
    void SetTextStyle(sf::Text::Style style);
    void SetTextColor(const sf::Color& color);
    void SetTextCharacterSize(uint32_t size);
    void SetTexture(E2D::Texture* texture);
    void SetTextFont(E2D::Font* font);
    void SetPosition(sf::Vector2f pos);
    void SetPosition(float x, float y);
    void Update(const sf::Event& event);
    void Draw();

    bool IsClicked() const { return isClicked; }
    bool MouseIsOver() const { return mouseIsOver; }

    
protected:
    E2D::Renderer& renderer;
    E2D::Sprite sprite;
    E2D::Text text;
    sf::IntRect defaultRect;
    sf::IntRect hoverRect;
    float width;
    float height;
    bool isClicked;
    bool mouseIsOver;

    void RecalculateButtonSize();
    void CenterText();
};