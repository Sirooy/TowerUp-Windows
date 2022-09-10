#include "TileMap.h"
#include "Renderer.h"
#include "Texture.h"

namespace E2D
{

TileMap::TileMap() : layers() { }

void TileMap::Draw(LayerType type, const sf::RenderStates& states)
{
    Renderer::Instance().Draw(layers[type], states);
}

void TileMap::CreateLayer(LayerType type, Texture* texture, int32_t* tiles, int32_t width, int32_t height)
{
    layers[type].Create(&texture->texture, tiles, width, height);
}

void TileMap::CreateShadowLayer(int32_t* tiles, int32_t width, int32_t height)
{
    layers[LayerType::Shadow].CreateShadowLayer(tiles, width, height);
}

} // namespace E2D