#pragma once
#include "SFML/Graphics/Text.hpp"
#include "Font.h"

namespace E2D
{

class Text
{
public:
    enum Style : uint32_t
    {
        Regular = sf::Text::Style::Regular,
        Bold = sf::Text::Style::Bold,
        Italic = sf::Text::Style::Italic,
        StrikeThrough = sf::Text::Style::StrikeThrough,
        Underlined = sf::Text::Style::Underlined
    };

    Text() = default;
    Text(const std::string& string, const sf::Font& font, unsigned int characterSize = 30);

    void SetString(const std::string& string);
    void SetFont(const sf::Font& font);
    void SetFont(const Font& font);
    void SetCharacterSize(unsigned int size);
    void SetLineSpacing(float spacingFactor);
    void SetPosition(const sf::Vector2f& pos);
    void SetPosition(float x, float y);
    void SetOrigin(const sf::Vector2f& origin);
    void SetOrigin(float x, float y);
    void SetScale(const sf::Vector2f& factors);
    void SetScale(float factorX, float factorY);
    void SetRotation(float angle);
    void SetLetterSpacing(float spacingFactor);
    void SetStyle(uint32_t style);
    void SetFillColor(const sf::Color& color);
    void SetOutlineColor(const sf::Color& color);
    void SetOutlineThickness(float thickness);
    void SetAlpha(uint8_t alpha);
    void InvertAlpha();
    bool FadeOut(float time, float dt);
    bool FadeIn(float time, float dt);
    sf::Vector2f GetPosition() const;
    const std::string& GetString() const;
    const sf::Font* GetFont() const;
    unsigned int GetCharacterSize() const;
    float GetLetterSpacing() const;
    float GetLineSpacing() const;
    uint32_t GetStyle() const;
    const sf::Color& GetFillColor() const;
    const sf::Color& GetOutlineColor() const;
    float GetOutlineThickness() const;
    sf::Vector2f FindCharacterPos(std::size_t index) const;
    sf::FloatRect GetLocalBounds() const;
    sf::FloatRect GetGlobalBounds() const;
    inline uint8_t GetAlpha() const { return text.getFillColor().a; }
    inline bool IsFadedOut() const { return text.getFillColor().a == 0; }
    inline bool IsFadedIn() const { return text.getFillColor().a == 255; }

private:
    friend class Renderer;

    sf::Text text;
    std::string textString;
};

} // namespace E2D