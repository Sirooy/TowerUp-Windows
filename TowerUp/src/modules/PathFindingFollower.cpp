#include "PathFindingFollower.h"
#include "SFMLUtils.hpp"

PathFindingFollower::PathFindingFollower(float requestTime) : pathfinding(PathFinding::Instance()),
    requestTime(requestTime)
{

}

void PathFindingFollower::RequestNewPath(uint32_t entityID, sf::Vector2f fromPosition, sf::Vector2f toPosition)
{
    if(requestClock.getElapsedTime().asSeconds() > requestTime)
    {
        requestClock.restart();
        if(pathfinding.RequestPath(path, entityID, fromPosition, toPosition))
        {
            currentPathNode = 0;

            if(path.size() > 1)
            {
                Node* n1 = path[0];
                Node* n2 = path[1];

                sf::Vector2f n1Pos = n1->WorldPosition;
                sf::Vector2f n2Pos = n2->WorldPosition;

                if(sf::lengthSquared(fromPosition - n2Pos) < sf::lengthSquared(n1Pos - n2Pos))
                    ++currentPathNode;
            }
        }
    }
}

bool PathFindingFollower::HasReachedDestination()
{
    return currentPathNode >= path.size(); // ==?
}

bool PathFindingFollower::HasNotReachedDestination()
{
    return currentPathNode < path.size();
}

sf::Vector2f PathFindingFollower::GetPathNextDirection(sf::Vector2f position, float speed, float dt)
{
    sf::Vector2f targetPos = path[currentPathNode]->WorldPosition;
    sf::Vector2f dir = targetPos - position;
    float length = sf::length(dir);

    if(length < speed * dt)
        ++currentPathNode;
    
    if(length != 0.0f)
        dir /= length;

    return dir;
}

sf::Vector2f PathFindingFollower::TryGetPathNextDirection(sf::Vector2f position, float speed, float dt)
{
    if(currentPathNode < path.size())
    {
        sf::Vector2f targetPos = path[currentPathNode]->WorldPosition;
        sf::Vector2f dir = targetPos - position;
        float length = sf::length(dir);

        if(length < speed * dt)
            ++currentPathNode;

        if(length != 0.0f)
            dir /= length;

        return dir;
    }
    else
        return sf::Vector2f(0.0f, 0.0f);
}