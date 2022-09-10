#include "PathFinding.h"
#include "CollisionLayer.h"

#include <cmath>

//NODES
Node::Node() : Neighbours{}, Parent{ nullptr }, ID{ 0 }, X{ 0 }, Y{ 0 }, 
    F{ 0 }, G{ 0 }, H{ 0 }, WorldPosition{0.0f, 0.0f}, IsInOpenList {false}, IsVisited{ false }, IsSolid{ false } { }

bool CompareNode::operator()(const Node* a, const Node* b)
{
    return a->F > b->F;
}


//PathFinding
PathFinding::PathFinding() : requestSet(), queue(), grid(nullptr), maxWidth(0), maxHeight(0), 
    width(0), height(0), maxRequests(0), currentRequests(0) { }

void PathFinding::CreateGrid(int32_t width, int32_t height)
{
    this->maxWidth = width;
    this->maxHeight = height;
    this->width = width;
    this->height = height;

    if(grid)
    {
        delete[] grid[0];
        delete[] grid;
    }

    Node* memory = new Node[width * height];
    grid = new Node*[height];
    for(int32_t i = 0;i < height; ++i)
    {
        grid[i] = &memory[i * width];
    }

    int32_t currentID = 0;
    for(int32_t y = 0;y < height; ++y)
    {
        for(int32_t x = 0;x < width; ++x)
        {
            Node& node = grid[y][x];
            node.ID = currentID++;
            node.X = x;
            node.Y = y;
            node.WorldPosition = sf::Vector2f(x * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f, 
                                              y * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f); 
        }
    }

    UpdateNodeNeighbours(width, height);
}

void PathFinding::UpdateGrid(CollisionLayer* collisionLayer, int32_t width, int32_t height)
{
    this->width = width;
    this->height = height;

    for(int32_t y = 0; y < height; ++y)
    {
        for(int32_t x = 0; x < width; ++x)
        {
            grid[y][x].IsSolid = collisionLayer->Get(x, y) != 0;
        }
    }

    for(int32_t y = 0; y < height; ++y)
    {
        for(int32_t x = 0; x < width; ++x)
        {
            SetNodeNeighbours(&grid[y][x]);
        }
    }
}

void PathFinding::UpdateNodeNeighbours(int32_t width, int32_t height)
{
    for(int32_t y = 0; y < height; ++y)
    {
        for(int32_t x = 0; x < width; ++x)
        {
            SetNodeNeighbours(&grid[y][x]);
        }
    }
}

void PathFinding::SetNodeNeighbours(Node* n)
{
    n->Neighbours.clear();

    if(n->IsSolid)
        return;

    for(int32_t y = n->Y - 1; y < n->Y + 2; ++y)
    {
        if(y < 0 || y > height - 1)
            continue;

        for(int32_t x = n->X - 1; x < n->X + 2; ++x)
        {
            if(x < 0 || x > width - 1)
                continue;

            Node* neighbour = &grid[y][x];
            
            if(neighbour == n || neighbour->IsSolid)
                continue;

            int32_t xDiff = n->X - neighbour->X;
            int32_t yDiff = n->Y - neighbour->Y;

            if(xDiff != 0 && yDiff != 0) //It's a diagonal node
            {
                if(!grid[y + yDiff][x].IsSolid && //Only add the node if the surrounding nodes are not solid, so the AI can't cut corners
                   !grid[y][x + xDiff].IsSolid) 
                   n->Neighbours.push_back(neighbour);
            }
            else
                n->Neighbours.push_back(neighbour);
        }
    }
}

void PathFinding::Solve(std::vector<Node*>& path, Node* start, Node* end)
{
    Reset();

    start->G = 0;
    Node* current = nullptr;
    queue.push(start);
    while(!queue.empty() && current != end)
    {
        current = queue.top();
        queue.pop(); 
        while(!queue.empty() && current->IsVisited) //The node has been added twice or more to the queue and needs to be skipped
        {
            current = queue.top();
            queue.pop();
        } 
        current->IsVisited = true;

        for(auto other : current->Neighbours)
        {
            if(other->IsVisited) //Skip visited nodes
                continue;

            int32_t g = current->G;
            if(current->X != other->X && current->Y != other->Y) //Is diagonal neighbour
                g += 14;
            else
                g += 10;

            if(g < other->G) //Update the node
            {
                if(!other->IsInOpenList) //It's a new node
                {
                    other->H = CalculateHeuristic(other, end);
                    other->IsInOpenList = true;
                }

                other->G = g;
                other->Parent = current;
                other->F = other->G + other->H;
                queue.push(other);  //Add the node even if it's already in the queue
            }
        }        
    }

    path.clear();
    while(end != nullptr)
    {
        path.push_back(end);
        end = end->Parent;
    }
}

void PathFinding::Reset()
{
    while(!queue.empty()) queue.pop();

    for(int32_t y = 0; y < height; ++y)
    {
        for(int32_t x = 0;x < width; ++x)
        {
            Node& current = grid[y][x];
            current.IsVisited = false;
            current.IsInOpenList = false;
            current.Parent = nullptr;
            current.G = std::numeric_limits<int32_t>::max();
            current.F = 0;
            current.H = 0;
        }
    }
}

bool PathFinding::RequestPath(std::vector<Node*>& path, uint32_t id, sf::Vector2f from, sf::Vector2f to)
{
    int32_t requestsLeft = maxRequests - currentRequests;
    if(requestsLeft > 0)
    {
        
        if(requestsLeft > static_cast<int32_t>(requestSet.size())) 
        {
            ++currentRequests;
            //Se invierten los nodos para que a la hora de construir el camino sea desde from a to
            Solve(path, GetNode(to.x, to.y), GetNode(from.x, from.y)); 

            return true;
        }
        else //Priorizar las peticiones que no se ejecutaron en el Update anterior
        {
            auto it = requestSet.find(id);
            if(it != requestSet.end())
            {
                ++currentRequests;
                requestSet.erase(it);
                Solve(path, GetNode(to.x, to.y), GetNode(from.x, from.y));

                return true;
            }
            else
            {
                if(static_cast<int32_t>(requestSet.size()) < maxRequests)
                    requestSet.insert(id);
            }
        }
        
    }
    else
        if(static_cast<int32_t>(requestSet.size()) < maxRequests)
            requestSet.insert(id);

    return false;
}

void PathFinding::ResetRequests()
{
    if(currentRequests < maxRequests)
        requestSet.clear();
    currentRequests = 0;
}

void PathFinding::SetMaxRequests(int32_t maxRequests)
{
    this->maxRequests = maxRequests;
}

int32_t PathFinding::CalculateHeuristic(Node* a, Node* b)
{
    return (std::abs(a->X - b->X) + std::abs(a->Y - b->Y)) * 10;
}

PathFinding::~PathFinding()
{
    if(grid)
    {
        delete[] grid[0];
        delete[] grid;
    }
}

PathFinding PathFinding::SingletonInstance {}; 