#include "World.h"
#include "Player.h"
#include "Enemies/Enemy.h"
#include "Bullet.h"
#include "BulletGenerator.h"
#include "Door.h"
#include "Consumable.h"
#include "InteractiveEntity.h"
#include "../Physics/PhysicsEngine.h"
#include "../LevelGenerator.h"
#include "../Hud.h"
#include "VendingMachine.h"
#include "BaseItem.h"
#include "Trap.h"
#include "GenericEntity.h"
#include "Explosion.h"
#include "IceBlizzard.h"

#include <iostream>

World::World() : physics(PhysicsEngine::Instance())
{
    
}

Hud* World::CreateHud()
{
    hud = new Hud();

    return hud;
}

Player* World::CreatePlayer()
{
    player = new Player();
    physics.AddDynamicEntity(player);

    return player;
}

Door* World::CreateDoor(DoorType type, sf::Vector2f position, int32_t id, const std::vector<Curse*>& curses)
{
    Door* door = new Door(position, id, type, curses);

    doors.push_back(door);

    return door;
}


Enemy* World::CreateEnemy(EnemyType type, sf::Vector2f position)
{
    Enemy* enemy = entityFactory.CreateEnemy(type, position);

    enemies.push_back(enemy);
    physics.AddDynamicEntity(enemy);

    //Aplicar eventos

    return enemy;
}

Bullet* World::CreateBullet(BulletType type, BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks)
{
    Bullet* bullet = entityFactory.CreateBullet(type, generator, position, direction, executeCallbacks);

    bullets.push_back(bullet);
    physics.AddDynamicEntity(bullet);

    //Aplicar eventos

    return bullet;
}

Consumable* World::CreateConsumable(ConsumableType type, sf::Vector2f position)
{
    Consumable* consumable = entityFactory.CreateConsumable(type, position);

    consumables.push_back(consumable);
    physics.AddDynamicEntity(consumable);

    //Aplicar eventos

    return consumable;
}

VendingMachine* World::CreateVendingMachine(VendingMachineType type, sf::Vector2f position, uint64_t seed)
{
    VendingMachine* vendingMachine = entityFactory.CreateVendingMachine(type, position, seed);

    interactiveEntities.push_back(vendingMachine);

    return vendingMachine;
}

Trap* World::CreateTrap(TrapType type, sf::Vector2f position, TrapDirection direction, float startDelay)
{
    Trap* trap = entityFactory.CreateTrap(type, position, direction, startDelay);

    traps.push_back(trap);

    return trap;
}

BaseItem* World::CreateItem(ItemType type, sf::Vector2f position)
{
    BaseItem* item = entityFactory.CreateItem(type, position);

    interactiveEntities.push_back(item);

    return item;
}

GenericEntity* World::CreateExplosion(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t mask)
{
    GenericEntity* entity = new Explosion(position, radius, damage, playerDamage, mask);

    genericEntities.push_back(entity);

    return entity;
}

GenericEntity* World::CreateIceBlizzard(sf::Vector2f position, float radius, float damage, int32_t playerDamage, uint32_t mask)
{
    GenericEntity* entity = new IceBlizzard(position, radius, damage, playerDamage, mask);

    genericEntities.push_back(entity);

    return entity;
}

void World::RemoveDeadEnemies()
{
    size_t i = 0;
    while(i < enemies.size())
    {
        Enemy* e = enemies[i];
        if(e->IsDead)
        {
            std::swap(enemies[i], enemies.back());
            physics.RemoveDynamicEntity(e);
            delete e;
            enemies.pop_back();
        }
        else
            ++i;
    }
}

void World::RemoveDeadBullets()
{
    size_t i = 0;
    while(i < bullets.size())
    {
        Bullet* b = bullets[i];
        if(b->IsDead)
        {
            
            std::swap(bullets[i], bullets.back());
            physics.RemoveDynamicEntity(b);
            delete b;
            bullets.pop_back();
        }
        else
            ++i;
    }
}

void World::RemoveDeadConsumables()
{
    size_t i = 0;
    while(i < consumables.size())
    {
        Consumable* c = consumables[i];
        if(c->IsDead)
        {
            std::swap(consumables[i], consumables.back());
            physics.RemoveDynamicEntity(c);
            delete c;
            consumables.pop_back();
        }
        else
            ++i;
    }
}

void World::RemoveDeadInteractiveEntities()
{
    size_t i = 0;
    while(i < interactiveEntities.size())
    {
        InteractiveEntity* ie = interactiveEntities[i];
        if(ie->IsDead)
        {
            std::swap(interactiveEntities[i], interactiveEntities.back());
            delete ie;
            interactiveEntities.pop_back();
        }
        else
            ++i;
    }
}

void World::RemoveDeadGenericEntities()
{
    size_t i = 0;
    while(i < genericEntities.size())
    {
        GenericEntity* ge = genericEntities[i];
        if(ge->IsDead)
        {
            std::swap(genericEntities[i], genericEntities.back());
            delete ge;
            genericEntities.pop_back();
        }
        else
            ++i;
    }
}

void World::ClearEntities()
{
    ClearRoomEntities();

    if(player)
    {
        delete player;
        player = nullptr;
    }

    if(hud)
    {
        delete hud;
        hud = nullptr;
    }
}

void World::ClearRoomEntities()
{
    for(auto b : bullets)
        delete b;
    bullets.clear();

    for(auto e : enemies)
        delete e;
    enemies.clear();

    for(auto t : traps)
        delete t;
    traps.clear();

    for(auto c : consumables)
        delete c;
    consumables.clear();

    for(auto i : interactiveEntities)
        delete i;
    interactiveEntities.clear();

    for(auto g : genericEntities)
        delete g;
    genericEntities.clear();

    for(auto d : doors)
        delete d;
    doors.clear();
}

void World::SetSelectedDoor(int32_t doorId)
{
    selectedDoor = doorId;
}

void World::ResetSelectedDoor()
{
    selectedDoor = -1;
}

void World::SetGlobalDropProbability(float probabilty)
{
    globalDropProbability = probabilty;
}

void World::SetCheatsOn(bool value)
{
    cheatsOn = value;
}

World World::SingletonInstance {};