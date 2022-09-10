#pragma once

#include <cstdint>
#include "Engine2D/TileMap.h"

class CollisionLayer
{
public:
    CollisionLayer();

    void SetCollisionType(uint32_t collisionType);
    void SetCollisionMask(uint32_t collisionMask);
    void Create(int32_t width, int32_t height);
    bool Collides(int32_t x, int32_t y) const;

    inline void SetCurrentWidth(int32_t width)
    {
        this->currentWidth = width;
    }

    inline void SetCurrentHeight(int32_t height)
    {
        this->currentHeight = height;
    }

    inline void Set(int32_t x, int32_t y, int32_t value)
    {
        arr[y][x] = value;
    }

    inline int32_t& Get(int32_t x, int32_t y)
    {
        return arr[y][x];
    }

    inline const int32_t& Get(int32_t x, int32_t y) const
    {
        return arr[y][x];
    } 

    inline int32_t** GetArray()
    {
        return arr;
    }

    inline int32_t GetScaledCoord(float coord) const
    {
        return static_cast<int32_t>(coord) / E2D::TileMap::TILE_SIZE;
    }

    inline bool IsOutOfBounds(int32_t x, int32_t y) const
    {
        return (x < 0 || x > currentWidth - 1 ||
                y < 0 || y > currentHeight - 1);
    }

    inline constexpr int32_t GetMaxWidth() const
    {
        return width;
    }

    inline constexpr int32_t GetMaxHeight() const
    {
        return height;
    }

    inline constexpr int32_t GetCurrentWidth() const
    {
        return currentWidth;
    }

    inline constexpr int32_t GetCurrentHeight() const
    {
        return currentHeight;
    }

    ~CollisionLayer();

    int32_t CollisionType, CollisionMask;
private:
    int32_t** arr;
    int32_t width, height;
    int32_t currentWidth, currentHeight;
};