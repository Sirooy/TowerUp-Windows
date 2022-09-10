#include "LevelGenerator.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/CollidableEntity.h"
#include "Entity/EntityTypeEnums.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/LightSystem.h"
#include "PathFinding.h"
#include "SFMLUtils.hpp"
#include "Curse.h"
#include <exception>


LevelGenerator::LevelGenerator()
{
    CurrentRoom.collisionLayer.SetCollisionType(CollisionType::TileCollisionType);
    CurrentRoom.collisionLayer.SetCollisionMask(CollisionType::EnemyCollisionType | 
                                                CollisionType::PlayerCollisionType | 
                                                CollisionType::PlayerBulletCollisionType | 
                                                CollisionType::EnemyBulletCollisionType |
                                                CollisionType::ConsumableCollisionType);
    CurrentRoom.collisionLayer.Create(LevelGenerator::MAX_WIDTH, LevelGenerator::MAX_HEIGHT);

    CreateAvailableCurses();
    InitTotalRoomsPerZone();
}

void LevelGenerator::CreateAvailableCurses()
{
    //availableCurses.push_back(new VisualCurse(E2D::FilterType::Blindness));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::GrayScale));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::Rainbow));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::Nausea));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::Negative));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::Pixelation));
    availableCurses.push_back(new VisualCurse(E2D::FilterType::RGB));
    /*availableCurses.push_back(new DoubleLifeEnemiesCurse());
    availableCurses.push_back(new OneHeartCurse());
    availableCurses.push_back(new OneBulletCurse());
    availableCurses.push_back(new MaxDashCooldownCurse());
    availableCurses.push_back(new HalfMoneyCurse());
    availableCurses.push_back(new MoreRecoilCurse());
    availableCurses.push_back(new MoreReloadTimeCurse());*/
}

void LevelGenerator::InitTotalRoomsPerZone()
{
    //Default rooms
    for(int32_t zone = 0; zone < NUM_ZONES; ++zone)
    {
        totalRoomsPerZone[RoomTypeIndex::DefaultRoomIndex][zone] = 
            CalculateTotalRoomsPerZone("resources/rooms/zone"+std::to_string(zone + 1) + "/");
    }

    //Treasure rooms
    for(int32_t zone = 0; zone < NUM_ZONES; ++zone)
    {
        totalRoomsPerZone[RoomTypeIndex::TreasureRoomIndex][zone] = 
            CalculateTotalRoomsPerZone("resources/rooms/treasure/zone" + 
                std::to_string(zone + 1) + "/");
    }

    //Shop rooms
    for(int32_t zone = 0; zone < NUM_ZONES; ++zone)
    {
        totalRoomsPerZone[RoomTypeIndex::ShopRoomIndex][zone] = 
            CalculateTotalRoomsPerZone("resources/rooms/shop/zone" + 
                std::to_string(zone + 1) + "/");
    }

    //Boss rooms
    for(int32_t zone = 0; zone < NUM_ZONES; ++zone)
    {
        totalRoomsPerZone[RoomTypeIndex::BossRoomIndex][zone] = 
            CalculateTotalRoomsPerZone("resources/rooms/boss/zone" + 
                std::to_string(zone + 1) + "/");
    }
}

void LevelGenerator::SetSeed(uint64_t seed)
{
    random.SetSeed(seed);
}

void LevelGenerator::Reset()
{
    completedRooms = 0;
    currentZone = 0;
    currentLevel = 0;

    CurrentRoom.torches.clear();
    CurrentRoom.curses.clear();
    CurrentRoom.doors.clear();
    SetRoomsPerLevel();
}

void LevelGenerator::SetRoomsPerLevel()
{
    roomsPerLevel = random.RangeInt(MIN_ROOMS_PER_LEVEL, MAX_ROOMS_PER_LEVEL) + 1; //+1 Por la sala del tesoro/tienda
    
    if(currentLevel == NUM_LEVELS_PER_ZONE - 1) //Si estamos en el ultimo nivel añadir tambien la sala del boss
        ++roomsPerLevel;
}

void LevelGenerator::CreateHub()
{
    completedRooms = -1;
    LoadRoomFromFile("resources/rooms/hub.xml");
    //currentZone = 2;
    //currentLevel = 1;
}

bool LevelGenerator::CreateNextRoom(int32_t doorId)
{
    DoorData& doorData = CurrentRoom.doors[doorId];
    random.Skip(doorData.ID); //Omitimos una serie de numeros aleatorios dependiendo de la id de la puerta para que se generen salas distintas

    ++completedRooms;

    if(completedRooms == roomsPerLevel)
    {
        ++currentLevel;
        completedRooms = 0;
        SetRoomsPerLevel();

        if(currentLevel == NUM_LEVELS_PER_ZONE)
        {
            currentLevel = 0;
            ++currentZone;

            if(currentZone == NUM_ZONES)
                return false;
        } 
    }

    CurrentRoom.curses = std::move(doorData.Curses);
    CreateRoom(doorData.Type);

    return true;
}

void LevelGenerator::CreateRoom(DoorType doorType)
{
    std::string path;
    switch (doorType)
    {
        case DoorType::Boss:
            path = "resources/rooms/boss/zone" + std::to_string(currentZone + 1) +"/boss" +
                std::to_string(random.RangeInt(1, totalRoomsPerZone[RoomTypeIndex::BossRoomIndex][currentZone])) + ".xml"; 
            break;
        case DoorType::Shop:
            path = "resources/rooms/shop/zone" + std::to_string(currentZone + 1) +"/shop" +
                std::to_string(random.RangeInt(1, totalRoomsPerZone[RoomTypeIndex::ShopRoomIndex][currentZone])) + ".xml";
            break;
        case DoorType::Treasure:
            path = "resources/rooms/treasure/zone" + std::to_string(currentZone + 1) +"/treasure" + 
                std::to_string(random.RangeInt(1, totalRoomsPerZone[RoomTypeIndex::TreasureRoomIndex][currentZone])) + ".xml";  
            break;
    
        default: //DoorType = NewZone o Default
            path = "resources/rooms/zone" + std::to_string(currentZone + 1) + "/map" + 
                std::to_string(random.RangeInt(1, totalRoomsPerZone[RoomTypeIndex::DefaultRoomIndex][currentZone])) + ".xml";
            break;
    }

    LoadRoomFromFile(path);
}

void LevelGenerator::LoadRoomFromFile(const std::string& path)
{
    tinyxml2::XMLDocument document;
    
    if(document.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS)
        throw std::runtime_error("No se ha podido cargar la habitación del fichero: " + path);
    
    tinyxml2::XMLElement* map = document.FirstChildElement("map");

    map->QueryIntAttribute("width", &CurrentRoom.widthInTiles);
    map->QueryIntAttribute("height", &CurrentRoom.heightInTiles);
    CurrentRoom.collisionLayer.SetCurrentWidth(CurrentRoom.widthInTiles);
    CurrentRoom.collisionLayer.SetCurrentHeight(CurrentRoom.heightInTiles);
    CurrentRoom.size = sf::Vector2f
    (
        static_cast<float>(CurrentRoom.widthInTiles * E2D::TileMap::TILE_SIZE), 
        static_cast<float>(CurrentRoom.heightInTiles * E2D::TileMap::TILE_SIZE)
    );

    CurrentRoom.doors.clear();
    CurrentRoom.torches.clear();
    CreateTileMapLayers(map);
    CreateEntities(map);
}

void LevelGenerator::CreateTileMapLayers(tinyxml2::XMLElement* map)
{
    tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
    std::string textureName = map->FirstChildElement("properties")->FirstChildElement("property")->Attribute("value");
    E2D::Texture* texture = E2D::MediaContent::Instance().GetTexture(textureName);

    while(layer)
    {
        std::vector<std::string> tiles = Split(layer->FirstChildElement("data")->GetText(), ",");

        if (tiles.size() > 0)
        {
            int32_t* matrix = new int32_t[tiles.size()];
            for (std::size_t i = 0; i < tiles.size(); ++i)
            {
                matrix[i] = std::stoi(tiles[i]);
            }

            std::string name = layer->Attribute("name");

            if (name == "collision")
            {
                FillCollisionLayer(matrix);
            }
            else if (name == "background")
            {
                CurrentRoom.tileMap.CreateLayer(E2D::TileMap::LayerType::Background, texture,
                    matrix, CurrentRoom.widthInTiles, CurrentRoom.heightInTiles);
            }
            else if (name == "foreground")
            {
                CurrentRoom.tileMap.CreateLayer(E2D::TileMap::LayerType::Foreground, texture,
                    matrix, CurrentRoom.widthInTiles, CurrentRoom.heightInTiles);
            }
            else if (name == "decoration")
            {
                CurrentRoom.tileMap.CreateLayer(E2D::TileMap::LayerType::Decoration, texture,
                    matrix, CurrentRoom.widthInTiles, CurrentRoom.heightInTiles);
            }
            else if (name == "shadow")
            {
                CurrentRoom.tileMap.CreateShadowLayer(matrix, CurrentRoom.widthInTiles, CurrentRoom.heightInTiles);
            }

            layer = layer->NextSiblingElement("layer");
            delete[] matrix;
        }
    }

    PhysicsEngine::Instance().SetCollisionLayer(&CurrentRoom.collisionLayer);
    PathFinding::Instance().UpdateGrid(&CurrentRoom.collisionLayer, CurrentRoom.widthInTiles, CurrentRoom.heightInTiles);
}

void LevelGenerator::CreateEntities(tinyxml2::XMLElement* map)
{
    World& world = World::Instance();

    tinyxml2::XMLElement* object = map->FirstChildElement("objectgroup")->FirstChildElement("object");

    while(object)
    {
        std::string objectName = object->Attribute("name");
        float x = static_cast<float>(atof(object->Attribute("x")));
        float y = static_cast<float>(atof(object->Attribute("y")));
        sf::Vector2f position(x, y);

        if(objectName == "StartPoint")
        {
            world.GetPlayer()->Position = position;
            world.GetPlayer()->LastPosition = position;
        } 
        else if(objectName == "Door")
        {
            CreateDoor(position);
        }
        else if(objectName == "Enemy")
        {
            EnemyType type = static_cast<EnemyType>(std::atoi(object->Attribute("type")));
            world.CreateEnemy(type, position);

        }
        else if(objectName == "Trap")
        {
            TrapType type = static_cast<TrapType>(std::atoi(object->Attribute("type")));
            CreateTrap(type, position, object->FirstChildElement("properties"));
        }
        else if(objectName == "Item")
        {
            ItemType type = static_cast<ItemType>(random.NextInt(static_cast<int32_t>(ItemType::MaxItemTypes)));
            world.CreateItem(type, position);
        } 
        else if(objectName == "VendingMachine")
        {
            VendingMachineType type = static_cast<VendingMachineType>(std::atoi(object->Attribute("type")));
            world.CreateVendingMachine(type, position, random.Next());
        }
        else if(objectName == "Light")
        {
            CreateLight(position, object->FirstChildElement("properties"));
        }
        
        object = object->NextSiblingElement("object");
    }
}

void LevelGenerator::CreateTrap(TrapType type, sf::Vector2f position, tinyxml2::XMLElement* properties)
{
    TrapDirection direction = TrapDirection::Top;
    float delay = 0.0f;

    tinyxml2::XMLElement* property = nullptr;
    if(properties)
        property = properties->FirstChildElement("property");

    while(property)
    {
        std::string propertyName = property->Attribute("name");
        const char* value = property->Attribute("value");

        if(propertyName == "direction")
            direction = static_cast<TrapDirection>(std::atoi(value));
        else if (propertyName == "delay")
            delay = std::atof(value);
        
        property = property->NextSiblingElement("property");
    }

    World::Instance().CreateTrap(type, position, direction, delay);
}

void LevelGenerator::CreateLight(sf::Vector2f position, tinyxml2::XMLElement* properties)
{
    uint8_t red = 255;
    uint8_t green = 128;
    uint8_t blue = 0;
    float radius = E2D::TileMap::TILE_SIZE * 3.0f;

    tinyxml2::XMLElement* property = nullptr;
    if(properties)
        property = properties->FirstChildElement("property");

    while(property)
    {
        std::string propertyName = property->Attribute("name");
        const char* value = property->Attribute("value");

        if(propertyName == "color-r")
            red = static_cast<uint8_t>(std::atoi(value));
        else if(propertyName == "color-g")
            green = static_cast<uint8_t>(std::atoi(value));
        else if(propertyName == "color-b")
            blue = static_cast<uint8_t>(std::atoi(value));
        else if(propertyName == "radius")
            radius = std::atof(value);
        
        property = property->NextSiblingElement("property");
    }

    E2D::Light* light = E2D::LightSystem::Instance().CreateLight(position, red, green, blue, radius);
    CurrentRoom.torches.emplace_back(light);
}

void LevelGenerator::CreateDoor(sf::Vector2f position)
{
    int32_t id = static_cast<int32_t>(CurrentRoom.doors.size());
    int32_t numCurses = random.RangeInt(0, currentZone + 1);
    DoorType type { DoorType::Default };

    int32_t nextCompletedRooms = completedRooms + 1;
    int32_t nextLevel = currentLevel;
    if(nextCompletedRooms == roomsPerLevel)
        ++nextLevel;

    //La siguiente puerta pasará al siguiente nivel
    if(nextLevel != currentLevel) 
        type = DoorType::NewLevel;
    //Si la siguiente sala es la ultima del ultimo nivel
    else if(nextCompletedRooms == roomsPerLevel - 1 && currentLevel == NUM_LEVELS_PER_ZONE - 1) 
        type = DoorType::Boss;
    else //Le restamos la sala del tesoro/tienda y la sala del boss antes de calcular la mitad
    {
        int32_t halfRooms = roomsPerLevel - 1;
        if(currentLevel == NUM_LEVELS_PER_ZONE - 1) //Si estamos en el ultimo nivel le restamos la sala del boss
            --halfRooms;
        halfRooms /= 2;
        
        //Generamos una tienda en mitad del nivel
        if(nextCompletedRooms == halfRooms)
        {
            //Si hay un numero de puertas pares genera sala del tesoro, si no, tienda
            type = (CurrentRoom.doors.size() % 2 == 0) ? DoorType::Treasure : DoorType::Shop;
            numCurses = 0; 
        }
    }
    
    DoorData& doorData = CurrentRoom.doors.emplace_back(id, type, GenerateRoomCurses(numCurses));
    World::Instance().CreateDoor(type, position, id, doorData.Curses);
}

std::vector<Curse*> LevelGenerator::GenerateRoomCurses(int32_t numCurses)
{
    std::vector<int32_t> indices;
    indices.reserve(availableCurses.size());
    for(std::size_t i = 0; i < availableCurses.size(); ++i)
        indices.push_back(i);

    std::vector<Curse*> curses;
    curses.reserve(numCurses);
    for(int32_t i = 0;i < numCurses; ++i)
    {
        int32_t index = random.NextInt(indices.size());
        curses.push_back(availableCurses[indices[index]]);

        indices.erase(indices.begin() + index); //Borramos el indice para que no salga una maldicion repetida
    }

    return curses;
}


void LevelGenerator::FillCollisionLayer(int32_t* matrix)
{
    for(int y = 0;y < CurrentRoom.heightInTiles; ++y)
    {
        for(int x = 0; x < CurrentRoom.widthInTiles; ++x)
        {
            CurrentRoom.collisionLayer.Set(x, y, matrix[x + y * CurrentRoom.widthInTiles]);
        }
    }
}

//Visto en https://stackoverflow.com/questions/2802188/file-count-in-a-directory-using-c
int32_t LevelGenerator::CalculateTotalRoomsPerZone(const std::string& path) //Para saber el numero de mapas que hay por zona
{
    int rooms = 0;

    auto it = std::filesystem::directory_iterator(path);
    for (auto& entry : it)
    {
        if (entry.is_regular_file())
            ++rooms;    
    }
    
    return rooms;
}

LevelGenerator::~LevelGenerator()
{
    for(auto c : availableCurses)
        delete c;
    availableCurses.clear();
}

std::vector<std::string> Split(const std::string& str, const std::string& separator)
{
    std::vector<std::string> ret;
    
    std::size_t current = 0;
    std::size_t index = str.find(separator);
    while(index != std::string::npos)
    {
        ret.emplace_back(str.substr(current, index - current));
        current = index + separator.length();
        index = str.find(separator, current);
    }
    ret.emplace_back(str.substr(current, index));
    
    return ret;
}