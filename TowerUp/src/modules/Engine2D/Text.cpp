#include "Text.h"

namespace E2D
{

Text::Text(const std::string& string, const sf::Font& font, unsigned int characterSize) : text(string, font, characterSize), textString(string) { 

}

void Text::SetString(const std::string& string) { 
    text.setString(string); textString = string; 
}

void Text::SetFont(const sf::Font& font) { 
    text.setFont(font); 
}

void Text::SetFont(const Font& font)
{
    text.setFont(font.font);
}

void Text::SetCharacterSize(unsigned int size){ 
    text.setCharacterSize(size); 
}

void Text::SetLineSpacing(float spacingFactor) { 
    text.setLineSpacing(spacingFactor); 
}

void Text::SetPosition(const sf::Vector2f& pos){
    text.setPosition(pos);
}

void Text::SetPosition(float x, float y)
{
    text.setPosition(x, y);
}

void Text::SetOrigin(const sf::Vector2f& origin)
{
    text.setOrigin(origin);
}

void Text::SetOrigin(float x, float y)
{
    text.setOrigin(x, y);
}

void Text::SetScale(const sf::Vector2f& factors)
{
    text.setScale(factors);
}

void Text::SetScale(float factorX, float factorY)
{
    text.setScale(factorX, factorY);
}

void Text::SetRotation(float angle)
{
    text.setRotation(angle);
}

void Text::SetLetterSpacing(float spacingFactor) { 
    text.setLetterSpacing(spacingFactor); 
}

void Text::SetStyle(uint32_t style) { 
    text.setStyle(style); 
}

void Text::SetFillColor(const sf::Color& color) { 
    text.setFillColor(color); 
}

void Text::SetOutlineColor(const sf::Color& color) { 
    text.setOutlineColor(color); 
}

void Text::SetOutlineThickness(float thickness) { 
    text.setOutlineThickness(thickness); 
}

void Text::SetAlpha(uint8_t alpha)
{
    sf::Color fillColor = text.getFillColor();
    sf::Color outlineColor = text.getOutlineColor();
    fillColor.a = alpha;
    outlineColor.a = alpha;
    text.setFillColor(fillColor);
    text.setOutlineColor(outlineColor);
}

void Text::InvertAlpha()
{
    sf::Color fillColor = text.getFillColor();
    sf::Color outlineColor = text.getOutlineColor();
    fillColor.a = 255 - fillColor.a;
    outlineColor.a = 255 - outlineColor.a;
    text.setFillColor(fillColor);
    text.setOutlineColor(outlineColor);
}

bool Text::FadeOut(float time, float dt)
{
    float percent = dt / time;
    uint8_t amount = static_cast<uint8_t>(percent * 255.0f);
    uint8_t alpha = text.getFillColor().a;
    
    if(amount >= alpha)
    {   
        SetAlpha(0);
        return true;
    }
    else
        SetAlpha(alpha - amount);

    return false;
}

bool Text::FadeIn(float time, float dt)
{
    float percent = dt / time;
    uint8_t amount = static_cast<uint8_t>(percent * 255.0f);
    uint8_t alpha = text.getFillColor().a;
    
    if(amount + alpha >= 255)
    {
        SetAlpha(255);
        return true;
    }
    else
        SetAlpha(amount + alpha);

    return false;
}


sf::Vector2f Text::GetPosition() const
{
    return text.getPosition();
}

const std::string& Text::GetString() const { 
    return textString; 
}

const sf::Font* Text::GetFont() const { 
    return text.getFont(); 
}

unsigned int Text::GetCharacterSize() const { 
    return text.getCharacterSize(); 
}

float Text::GetLetterSpacing() const { 
    return text.getLetterSpacing(); 
}

float Text::GetLineSpacing() const { 
    return text.getLineSpacing(); 
}

uint32_t Text::GetStyle() const { 
    return text.getStyle(); 
}

const sf::Color& Text::GetFillColor() const { 
    return text.getFillColor(); 
}

const sf::Color& Text::GetOutlineColor() const { 
    return text.getOutlineColor(); 
}

float Text::GetOutlineThickness() const { 
    return text.getOutlineThickness(); 
}

sf::Vector2f Text::FindCharacterPos(std::size_t index) const { 
    return text.findCharacterPos(index); 
}

sf::FloatRect Text::GetLocalBounds() const { 
    return text.getLocalBounds(); 
}

sf::FloatRect Text::GetGlobalBounds() const { 
    return text.getGlobalBounds(); 
}

} // namespace E2D
