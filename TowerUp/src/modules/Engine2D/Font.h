#pragma once


#include "SFML/Graphics/Font.hpp"
#include <filesystem>

namespace E2D
{

class Font
{

public:
    Font() = default;

    bool LoadFromFile(const std::string& filename);
    const sf::Font::Info& GetInfo() const;
    const sf::Glyph& GetGlyph(uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness = 0) const;
    float GetKerning(uint32_t first, uint32_t second, unsigned int characterSize) const;
    float GetLineSpacing(unsigned int characterSize) const;
    float GetUnderlinePosition(unsigned int characterSize) const;
    float GetUnderlineThickness(unsigned int characterSize) const;
    const sf::Texture& GetTexture(unsigned int characterSize) const;

private:
    friend class Text;

    sf::Font font;
};

}
