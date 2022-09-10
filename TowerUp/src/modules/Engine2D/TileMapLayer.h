#pragma once

#include "SFML/Graphics.hpp"

namespace E2D
{

class TileMapLayer : public sf::Transformable
{
public:
    TileMapLayer();
    void Create(sf::Texture* texture, int32_t* tiles, int32_t width, int32_t height);
    void CreateShadowLayer(int32_t* tiles, int32_t width, int32_t height);

private:
    friend class Renderer;
    friend class LightSystem;

    sf::VertexArray m_vertices;
    sf::Texture* m_texture;
};

} // namespace E2D
