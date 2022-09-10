#pragma once

#include "TileMapLayer.h"

namespace E2D
{

class Texture;

class TileMap
{
public:
    static constexpr int32_t TILE_SIZE = 64;

    enum LayerType : uint32_t
    {
        Background = 0,
        Foreground = 1,
        Decoration = 2,
        Shadow     = 3
    };

    TileMap();
    void Draw(LayerType type, const sf::RenderStates& states = sf::RenderStates::Default);
    void CreateLayer(LayerType type, Texture* texture, int32_t* tiles, int32_t width, int32_t height);
    void CreateShadowLayer(int32_t* tiles, int32_t width, int32_t height);
    TileMapLayer& GetLayer(LayerType type) { return layers[type]; }
    
private:
    static constexpr uint32_t NUM_LAYERS = 4;

    TileMapLayer layers[NUM_LAYERS];
};

} // namespace E2D