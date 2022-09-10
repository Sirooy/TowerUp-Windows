#pragma once

#include "tinyxml2.h"
#include "Entity/Door.h"
#include "Random.h"
#include "Room.h"
#include <vector>

class Curse;

class LevelGenerator
{
public:
    static constexpr uint32_t MAX_WIDTH = 128;
    static constexpr uint32_t MAX_HEIGHT = 128; 
    static constexpr int32_t  MIN_ROOMS_PER_LEVEL = 3;
    static constexpr int32_t  MAX_ROOMS_PER_LEVEL = 6;
    static constexpr int32_t  NUM_LEVELS_PER_ZONE = 2;
    static constexpr int32_t  NUM_ZONES = 3;

    LevelGenerator();

    void CreateHub();
    bool CreateNextRoom(int32_t doorId);
    void SetSeed(uint64_t seed);
    void Reset();

    ~LevelGenerator();

    Room CurrentRoom {};
private:
    std::vector<Curse*> availableCurses {}; 
    Random random {};
   
    enum RoomTypeIndex
    {
        DefaultRoomIndex = 0,
        TreasureRoomIndex = 1,
        ShopRoomIndex = 2,
        BossRoomIndex = 3,
        TotalRoomIndices
    };

    //Gestion de la generacion procedural
    int completedRooms {0};
    int currentZone {0};
    int currentLevel {0};
    int roomsPerLevel {0};//Actualizado al pasar de nivel
    int totalRoomsPerZone[TotalRoomIndices][NUM_ZONES] {}; //Indica la cantidad de salas entre las que elegir dependiendo de la zona

    
    void CreateAvailableCurses();
    void InitTotalRoomsPerZone();
    void SetRoomsPerLevel();
    void LoadRoomFromFile(const std::string& path);
    void CreateTileMapLayers(tinyxml2::XMLElement* map);
    void CreateEntities(tinyxml2::XMLElement* map);
    void FillCollisionLayer(int32_t* tiles);
    void CreateRoom(DoorType doorType);
    void CreateDoor(sf::Vector2f position);
    void CreateTrap(TrapType type, sf::Vector2f position, tinyxml2::XMLElement* properties);
    void CreateLight(sf::Vector2f position, tinyxml2::XMLElement* properties);
    std::vector<Curse*> GenerateRoomCurses(int32_t numCurses);

    int32_t CalculateTotalRoomsPerZone(const std::string& path);
};

std::vector<std::string> Split(const std::string& str, const std::string& separator);
