#include "Font.h"

namespace E2D
{

bool Font::LoadFromFile(const std::string& filename) 
{ 
    return font.loadFromFile(filename); 
}

const sf::Font::Info& Font::GetInfo() const 
{ 
    return font.getInfo(); 
}

const sf::Glyph& Font::GetGlyph(uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness) const 
{ 
    return font.getGlyph(codePoint, characterSize, bold, outlineThickness); 
}

float Font::GetKerning(uint32_t first, uint32_t second, unsigned int characterSize) const 
{
    return font.getKerning(first, second, characterSize);
}

float Font::GetLineSpacing(unsigned int characterSize) const
{
    return font.getLineSpacing(characterSize);
}

float Font::GetUnderlinePosition(unsigned int characterSize) const
{
    return font.getUnderlinePosition(characterSize);
}

float Font::GetUnderlineThickness(unsigned int characterSize) const
{
    return font.getUnderlineThickness(characterSize);
}

const sf::Texture& Font::GetTexture(unsigned int characterSize) const
{
    return font.getTexture(characterSize);
}

};