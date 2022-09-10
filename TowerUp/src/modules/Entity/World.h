#pragma once
#include <vector>
#include "Door.h"
#include "EntityFactory.h"

class Hud;
class Enemy;
class Bullet;
class Consumable;
class InteractiveEntity;
class PhysicsEngine;
class BulletGenerator;
class GenericEntity;
class Player;
class Door;
class Trap;

using CreateEnemyCallback_t = void (*)(Enemy*);
using CreateBulletCallback_t = void (*)(Bullet*);

class World
{
public:
    inline Hud* GetHud() { return hud; }
    inline Player* GetPlayer() { return player; }
    inline std::vector<Enemy*>& GetEnemies() { return enemies; }
    inline std::vector<Bullet*>& GetBullets() { return bullets; }
    inline std::vector<Consumable*>& GetConsumables() { return consumables; }
    inline std::vector<InteractiveEntity*>& GetInteractiveEntities() { return interactiveEntities; }
    inline std::vector<Door*>& GetDoors() { return doors; }
    inline std::vector<Trap*>& GetTraps() { return traps; }
    inline std::vector<GenericEntity*>& GetGenericEntities() { return genericEntities; }

    Hud* CreateHud();
    Player* CreatePlayer();
    Door* CreateDoor(DoorType type, sf::Vector2f position, int32_t id, const std::vector<Curse*>& curses);
    Enemy* CreateEnemy(EnemyType type, sf::Vector2f position);
    Bullet* CreateBullet(BulletType type, BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks = true);
    Consumable* CreateConsumable(ConsumableType type, sf::Vector2f position);
    VendingMachine* CreateVendingMachine(VendingMachineType type, sf::Vector2f position, uint64_t seed);
    Trap* CreateTrap(TrapType type, sf::Vector2f position, TrapDirection direction, float startDelay);
    BaseItem* CreateItem(ItemType type, sf::Vector2f position);

    GenericEntity* CreateExplosion(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t mask);
    GenericEntity* CreateIceBlizzard(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t mask);

    void RemoveDeadEnemies();
    void RemoveDeadBullets();
    void RemoveDeadConsumables();
    void RemoveDeadInteractiveEntities();
    void RemoveDeadGenericEntities();
    void ClearRoomEntities();
    void ClearEntities();

    void SetSelectedDoor(int32_t doorId);
    void ResetSelectedDoor();
    void SetGlobalDropProbability(float probabilty);
    void SetCheatsOn(bool value);
    inline bool DoorIsSelected() { return selectedDoor != -1; }
    inline int32_t GetSelectedDoor() { return selectedDoor; }
    inline bool DoorsAreOpen() { return doors[0]->IsOpen(); }
    inline float GetGlobalDropProbability() { return globalDropProbability; }
    inline bool CheatsOn() { return cheatsOn; }

    inline static World& Instance(){ return SingletonInstance; }
protected:
    PhysicsEngine& physics;
    std::vector<Enemy*> enemies {};
    std::vector<Bullet*> bullets {};
    std::vector<Consumable*> consumables {};
    std::vector<InteractiveEntity*> interactiveEntities {};
    std::vector<Trap*> traps {};
    std::vector<Door*> doors {};
    std::vector<GenericEntity*> genericEntities {};
    EntityFactory entityFactory {};
    Player* player { nullptr };
    Hud* hud;
    int32_t selectedDoor { -1 };
    float globalDropProbability { 0.0f };
    bool cheatsOn { false };
    
    World();

    static World SingletonInstance; 
};