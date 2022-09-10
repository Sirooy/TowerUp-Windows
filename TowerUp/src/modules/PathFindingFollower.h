#pragma once

#include <cstdint>

#include "PathFinding.h"

class PathFindingFollower
{
public:
    PathFindingFollower(float updateTime);

    void RequestNewPath(uint32_t entityID, sf::Vector2f fromPosition, sf::Vector2f toPosition);
    bool HasReachedDestination();
    bool HasNotReachedDestination();
    sf::Vector2f GetPathNextDirection(sf::Vector2f position, float speed, float dt);
    sf::Vector2f TryGetPathNextDirection(sf::Vector2f position, float speed, float dt);
protected:
    PathFinding& pathfinding;
    float requestTime;
    sf::Clock requestClock {};
    std::vector<Node*> path {};
    uint32_t currentPathNode { 0 };
};