#include "TileMapLayer.h"
#include "TileMap.h"
#include <iostream>


namespace E2D
{

TileMapLayer::TileMapLayer() : m_vertices(), m_texture(nullptr)
{
m_vertices.setPrimitiveType(sf::Quads);
}

//Visto en las diapositivas de clase y en la documentación de sfml: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
void TileMapLayer::Create(sf::Texture* texture, int32_t* tiles, int32_t width, int32_t height)
{
    constexpr std::size_t VERTICES_PER_TILE = 4;

    m_texture = texture;
    m_vertices.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * VERTICES_PER_TILE);
    int32_t totalTiles = 0;
    int32_t textureWidthInTiles = static_cast<int32_t>(m_texture->getSize().x) / TileMap::TILE_SIZE;

    for (int32_t i = 0; i < width; ++i)
    {
        for (int32_t j = 0; j < height; ++j)
        {
            int tileNumber = tiles[(i + j * width)] - 1; //-1 por que en Tiled el primer tile empieza en 1
            if (tileNumber == -1)
                continue;

            int tu = tileNumber % textureWidthInTiles;
            int tv = tileNumber / textureWidthInTiles;

            sf::Vertex* quad = &m_vertices[totalTiles * VERTICES_PER_TILE];

            quad[0].position = sf::Vector2f(i * TileMap::TILE_SIZE, j * TileMap::TILE_SIZE);
            quad[1].position = sf::Vector2f((i + 1) * TileMap::TILE_SIZE, j * TileMap::TILE_SIZE);
            quad[2].position = sf::Vector2f((i + 1) * TileMap::TILE_SIZE, (j + 1) * TileMap::TILE_SIZE);
            quad[3].position = sf::Vector2f(i * TileMap::TILE_SIZE, (j + 1) * TileMap::TILE_SIZE);

            quad[0].texCoords = sf::Vector2f(tu * TileMap::TILE_SIZE, tv * TileMap::TILE_SIZE);
            quad[1].texCoords = sf::Vector2f((tu + 1) * TileMap::TILE_SIZE, tv * TileMap::TILE_SIZE);
            quad[2].texCoords = sf::Vector2f((tu + 1) * TileMap::TILE_SIZE, (tv + 1) * TileMap::TILE_SIZE);
            quad[3].texCoords = sf::Vector2f(tu * TileMap::TILE_SIZE, (tv + 1) * TileMap::TILE_SIZE);
            ++totalTiles; // Si no es -1 entonces contamos un tile mas
        }
    }
    m_vertices.resize(totalTiles * VERTICES_PER_TILE);
}

void TileMapLayer::CreateShadowLayer(int32_t* tiles, int32_t width, int32_t height)
{
    constexpr std::size_t VERTICES_PER_BLOCK = 2 * 3; // 2 triangles * 3 vertices

    m_vertices.setPrimitiveType(sf::Triangles);
    std::vector<sf::FloatRect> blocks;

    int32_t x = 0;
    int32_t y = 0;

    //Search for rows
    for(int32_t y = 0; y < height; ++y)
    {
        for (int32_t x = 0; x < width; ++x)
        {
            if (tiles[x + y * width] == 0)
                continue;

            int32_t startX = x;
            int32_t tileCount = 1;
            while (x + 1 < width && tiles[(x + 1) + y * width] != 0) //While it's a row
            {
                ++x;
                ++tileCount;
            }

            if (x != startX || //It's a row Or Its not a column
                (y == 0 || y + 1 == height) ||
                (tiles[x + (y + 1) * width] == 0 && tiles[x + (y - 1) * width] == 0))
            {
                blocks.emplace_back(startX * TileMap::TILE_SIZE, y * TileMap::TILE_SIZE,
                    tileCount * TileMap::TILE_SIZE, TileMap::TILE_SIZE);
            }
        }
    }

    //Search for columns
    for (int32_t x = 0; x < width; ++x)
    {
        for (int32_t y = 0; y < height; ++y)
        {
            if (tiles[x + y * width] == 0)
                continue;

            int32_t startY = y;
            int32_t tileCount = 1;

            while (y + 1 < height && tiles[x + (y + 1) * width] != 0) //While it's a column
            {
                ++y;
                ++tileCount;
            }

            if (y != startY) //Is a column
            {
                blocks.emplace_back(x * TileMap::TILE_SIZE, startY * TileMap::TILE_SIZE,
                     TileMap::TILE_SIZE, tileCount * TileMap::TILE_SIZE);
            }
        }
    }
    
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(blocks.size() * VERTICES_PER_BLOCK);
    for (std::size_t i = 0; i < blocks.size(); ++i)
    {
        const auto& current = blocks[i];

        sf::Vertex* triangles = &m_vertices[i * VERTICES_PER_BLOCK];

        sf::Vector2f topLeft = sf::Vector2f(current.left, current.top);
        sf::Vector2f topRight = sf::Vector2f(current.left + current.width, current.top);
        sf::Vector2f bottomLeft = sf::Vector2f(current.left, current.top + current.height);
        sf::Vector2f bottomRight = sf::Vector2f(current.left + current.width, current.top + current.height);

        triangles[0] = topLeft;
        triangles[1] = topRight;
        triangles[2] = bottomRight;

        triangles[3] = topLeft;
        triangles[4] = bottomRight;
        triangles[5] = bottomLeft;
    }
}

} // namespace E2D
