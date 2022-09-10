#include "CollisionLayer.h"

CollisionLayer::CollisionLayer() : 
    CollisionType(0), CollisionMask(0), arr(nullptr), width(0), height(0), currentWidth(0), currentHeight(0) { }

void CollisionLayer::SetCollisionType(uint32_t collisionType)
{
    this->CollisionType = collisionType;
}

void CollisionLayer::SetCollisionMask(uint32_t collisionMask)
{
    this->CollisionMask = collisionMask;
}

void CollisionLayer::Create(int32_t width, int32_t height)
{
    if(arr)
    {
        delete[] arr[0];
        delete[] arr;
    }

    this->width = width;
    this->height = height;
    this->currentWidth = width;
    this->currentHeight = height;
    
    int32_t* memory = new int32_t[width * height] { };
    arr = new int32_t*[height];
    
    for(int32_t i = 0;i < height; ++i)
    {
        arr[i] = &memory[i * width];
    }
}

bool CollisionLayer::Collides(int32_t x, int32_t y) const
{
    if(x < 0 || x > currentWidth - 1 ||
       y < 0 || y > currentHeight - 1)
       return true;
       
    return arr[y][x] != 0;
}

CollisionLayer::~CollisionLayer()
{
    if(arr)
    {
        delete[] arr[0];
        delete[] arr;
    }
}