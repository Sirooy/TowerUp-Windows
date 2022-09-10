#pragma once

#include "SFML/System/Vector2.hpp"
#include "Engine2D/TileMap.h"
#include <unordered_set>
#include <cstdint>
#include <vector>
#include <queue>

struct Node
{
    Node();

    std::vector<Node*> Neighbours;
    Node* Parent;
    int32_t ID;
    int32_t X, Y;
    int32_t F, G, H;
    sf::Vector2f WorldPosition;
    bool IsInOpenList;
    bool IsVisited;
    bool IsSolid;
};

struct CompareNode
{
    bool operator()(const Node* a, const Node* b);
};

class CollisionLayer;

class PathFinding
{
public:
    

    void CreateGrid(int32_t width, int32_t height);
    void UpdateGrid(CollisionLayer* collisionLayer, int32_t width, int32_t height);
    void UpdateNodeNeighbours(int32_t width, int32_t height);
    void SetNodeNeighbours(Node* n);
    bool RequestPath(std::vector<Node*>& path, uint32_t id, sf::Vector2f from, sf::Vector2f to);
    void SetMaxRequests(int32_t maxRequests);
    void ResetRequests();

    
    inline Node* GetNode(int32_t x, int32_t y) { return &grid[y][x]; }
    inline Node* GetNode(float x, float y)
    {
        int32_t intX = static_cast<int32_t>(x) / E2D::TileMap::TILE_SIZE;
        int32_t intY = static_cast<int32_t>(y) / E2D::TileMap::TILE_SIZE;

        return &grid[intY][intX];
    }

    inline static PathFinding& Instance()
    {
        return SingletonInstance;
    }
    ~PathFinding();

protected:
    std::unordered_set<uint32_t> requestSet;
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> queue;
    Node** grid;
    int32_t maxWidth;
    int32_t maxHeight;
    int32_t width;
    int32_t height;
    int32_t maxRequests;
    int32_t currentRequests;

    PathFinding();

    void Solve(std::vector<Node*>& path, Node* start, Node* end);
    int32_t CalculateHeuristic(Node* a, Node* b);
    void Reset();

    static PathFinding SingletonInstance;
};