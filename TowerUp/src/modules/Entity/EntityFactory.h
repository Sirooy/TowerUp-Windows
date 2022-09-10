#pragma once

#include <unordered_map>
#include "SFML/System/Vector2.hpp"
#include "EntityTypeEnums.h"
#include "functional"

class Enemy;
class Consumable;
class Bullet;
class BulletGenerator;
class BaseItem;
class VendingMachine;
class Trap;

class EntityFactory
{
public:
    EntityFactory();
    Enemy* CreateEnemy(EnemyType type, sf::Vector2f position);
    Bullet* CreateBullet(BulletType type, BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks = true);
    Consumable* CreateConsumable(ConsumableType type, sf::Vector2f position);
    VendingMachine* CreateVendingMachine(VendingMachineType type, sf::Vector2f position, uint64_t seed);
    Trap* CreateTrap(TrapType type, sf::Vector2f position, TrapDirection direction, float startDelay);
    BaseItem* CreateItem(ItemType type, sf::Vector2f position);

protected:
    std::unordered_map<EnemyType, Enemy* (EntityFactory::*)(sf::Vector2f)> createEnemyMethods;
    std::unordered_map<BulletType, Bullet* (EntityFactory::*)(BulletGenerator*, sf::Vector2f, sf::Vector2f, bool)> createBulletMethods;
    std::unordered_map<ConsumableType, Consumable* (EntityFactory::*)(sf::Vector2f)> createConsumableMethods;
    std::unordered_map<VendingMachineType, VendingMachine* (EntityFactory::*)(sf::Vector2f, uint64_t)> createVendingMachineMethods;
    std::unordered_map<TrapType, Trap* (EntityFactory::*)(sf::Vector2f, TrapDirection, float)> createTrapMethods;
    std::unordered_map<ItemType, std::function<BaseItem*(sf::Vector2f)>> createItemsFunctions;

    Bullet* CreateDefaultBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks = true);
    Bullet* CreateRubberBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks = true);

    Enemy* CreateFlyingEye(sf::Vector2f position);
    Enemy* CreateRedFlyingEye(sf::Vector2f position);
    Enemy* CreateGreenFlyingEye(sf::Vector2f position);
    Enemy* CreateBlueFlyingEye(sf::Vector2f position);
    Enemy* CreatePossessedCircularSaw(sf::Vector2f position);
    Enemy* CreateFastPossessedCircularSaw(sf::Vector2f position);
    Enemy* CreateDoubleDamagePossessedCircularSaw(sf::Vector2f position);
    Enemy* CreateBatEye(sf::Vector2f position);
    Enemy* CreateRedBatEye(sf::Vector2f position);
    Enemy* CreateGreenBatEye(sf::Vector2f position);
    Enemy* CreateBlueBatEye(sf::Vector2f position);
    Enemy* CreateAxeSkeleton(sf::Vector2f position);
    Enemy* CreateRogueKnight(sf::Vector2f position);

    Consumable* CreateRedHeart(sf::Vector2f position);
    Consumable* CreateGoldenHeart(sf::Vector2f position);
    Consumable* CreateBronceCoin(sf::Vector2f position);
    Consumable* CreateSilverCoin(sf::Vector2f position);
    Consumable* CreateGoldCoin(sf::Vector2f position);
    
    VendingMachine* CreateItemVendingMachine(sf::Vector2f position, uint64_t seed);
    VendingMachine* CreateHeartVendingMachine(sf::Vector2f position, uint64_t seed);
    VendingMachine* CreateCoinVendingMachine(sf::Vector2f position, uint64_t seed);

    Trap* CreateZone1SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone1BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone2SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone2BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone3SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone3BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay);
    
    Trap* CreateZone1ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay);
    Trap* CreateZone2ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay);
    Trap* CreateZone3ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay);

    Trap* CreateZone1DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone1FastShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone1BigShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone2DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone2FastShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone2TripleShooter(sf::Vector2f position, TrapDirection direction, float startDelay);

    Trap* CreateZone3DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone3TripleShooter(sf::Vector2f position, TrapDirection direction, float startDelay);
    Trap* CreateZone3DoubleShooter(sf::Vector2f position, TrapDirection direction, float startDelay);

    void AddItemCreationFunctions();
};