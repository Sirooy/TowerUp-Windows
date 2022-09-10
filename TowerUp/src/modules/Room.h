#pragma once

#include "CollisionLayer.h"
#include "Engine2D/TileMap.h"
#include "Engine2D/MediaContent.h"
#include "Entity/Door.h"
#include "Torch.h"

class Curse;

enum class RoomType : uint32_t
{
    Default = 0,
    Hub,
    Shop,
    Treasure,
    Boss
};

struct DoorData
{
    int32_t ID { 0 };
    DoorType Type { DoorType::Default };
    std::vector<Curse*> Curses {};

    DoorData(int32_t id, DoorType type, std::vector<Curse*>&& curses) : ID(id), Type(type), Curses(curses) {} 
};

class Room
{
public:
    Room();

    inline CollisionLayer* GetCollisionLayer() { return &collisionLayer; }
    inline int32_t GetWidthInTiles() { return widthInTiles; }
    inline int32_t GetHeightInTiles() { return heightInTiles; }
    inline sf::Vector2f GetSize() { return size; }

    void ApplyCurses();
    void RemoveCurses();
    void Draw(E2D::TileMap::LayerType type, const sf::RenderStates& states = sf::RenderStates::Default);
    void UpdateTorches(float dt);
    void DrawTorches();
    int32_t GetNumCurses() { return static_cast<int32_t>(curses.size()); }
    E2D::TileMapLayer& GetTileMapLayer(E2D::TileMap::LayerType type) { return tileMap.GetLayer(type); }

protected:
    friend class LevelGenerator;

    E2D::TileMap tileMap;
    CollisionLayer collisionLayer;
    std::vector<Curse*> curses;
    std::vector<DoorData> doors;
    std::vector<Torch> torches;

    sf::Vector2f size;
    int32_t widthInTiles;
    int32_t heightInTiles;
    RoomType type;
};