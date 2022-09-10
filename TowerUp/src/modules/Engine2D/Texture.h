#pragma once

#include <filesystem>
#include "SFML/Graphics/Texture.hpp"

namespace E2D
{

class Texture
{
public:
    Texture()  = default;

    bool Create(unsigned int width, unsigned int height);
    bool LoadFromFile(const std::string& filename, const sf::IntRect& area = sf::IntRect());
    sf::Vector2u GetSize() const;

    void SetSmooth(bool smooth);
    bool IsSmooth() const;
    void SetSrgb(bool sRgb);
    bool IsSrgb() const;
    void SetRepeated(bool repeated);
    bool IsRepeated() const;

    static unsigned int GetMaximumSize();

    ~Texture() = default;

private:
    friend class Sprite;
    friend class TileMap;

    sf::Texture texture;
};

} // namespace E2D


