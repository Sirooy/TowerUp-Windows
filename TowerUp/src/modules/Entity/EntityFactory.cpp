#include "EntityFactory.h"
#include "VendingMachine.h"
#include "Enemies/Enemy.h"
#include "Enemies/FlyingEye.h"
#include "Enemies/PossessedCircularSaw.h"
#include "Enemies/BatEye.h"
#include "Enemies/AxeSkeleton.h"
#include "Enemies/RogueKnight.h"
#include "Bullet.h"
#include "BulletGenerator.h"
#include "Heart.h"
#include "Coin.h"
#include "Items.h"
#include "ShooterTrap.h"
#include "CircularSawTrap.h"
#include "ProximitySpikeTrap.h"

EntityFactory::EntityFactory()
{
    //ENEMIES
    createEnemyMethods.emplace(std::make_pair(EnemyType::FlyingEye, &EntityFactory::CreateFlyingEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::RedFlyingEye, &EntityFactory::CreateRedFlyingEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::GreenFlyingEye, &EntityFactory::CreateGreenFlyingEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::BlueFlyingEye, &EntityFactory::CreateBlueFlyingEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::PossessedCircularSaw, &EntityFactory::CreatePossessedCircularSaw));
    createEnemyMethods.emplace(std::make_pair(EnemyType::FastPossessedCircularSaw, &EntityFactory::CreateFastPossessedCircularSaw));
    createEnemyMethods.emplace(std::make_pair(EnemyType::DoubleDamagePossessedCircularSaw, &EntityFactory::CreateDoubleDamagePossessedCircularSaw));
    createEnemyMethods.emplace(std::make_pair(EnemyType::BatEye, &EntityFactory::CreateBatEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::RedBatEye, &EntityFactory::CreateRedBatEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::GreenBatEye, &EntityFactory::CreateGreenBatEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::BlueBatEye, &EntityFactory::CreateBlueBatEye));
    createEnemyMethods.emplace(std::make_pair(EnemyType::AxeSkeleton, &EntityFactory::CreateAxeSkeleton));
    createEnemyMethods.emplace(std::make_pair(EnemyType::RogueKnight, &EntityFactory::CreateRogueKnight));

    //BULLETS
    createBulletMethods.emplace(std::make_pair(BulletType::Default, &EntityFactory::CreateDefaultBullet));
    createBulletMethods.emplace(std::make_pair(BulletType::RubberBullet, &EntityFactory::CreateRubberBullet));

    //CONSUMABLES
    createConsumableMethods.emplace(std::make_pair(ConsumableType::RedHeart, 
        &EntityFactory::CreateRedHeart));
    createConsumableMethods.emplace(std::make_pair(ConsumableType::GoldenHeart, 
        &EntityFactory::CreateGoldenHeart));
    createConsumableMethods.emplace(std::make_pair(ConsumableType::BronceCoin, 
        &EntityFactory::CreateBronceCoin));
    createConsumableMethods.emplace(std::make_pair(ConsumableType::SilverCoin, 
        &EntityFactory::CreateSilverCoin));
    createConsumableMethods.emplace(std::make_pair(ConsumableType::GoldCoin, 
        &EntityFactory::CreateGoldCoin));

    //VENDING MACHINES
    createVendingMachineMethods.emplace(std::make_pair(VendingMachineType::ItemVendingMachine, 
        &EntityFactory::CreateItemVendingMachine));
    createVendingMachineMethods.emplace(std::make_pair(VendingMachineType::HeartVendingMachine, 
        &EntityFactory::CreateHeartVendingMachine));
    createVendingMachineMethods.emplace(std::make_pair(VendingMachineType::CoinVendingMachine, 
        &EntityFactory::CreateCoinVendingMachine));

    //TRAPS
    createTrapMethods.emplace(std::make_pair(TrapType::Zone1SmallCircularSaw,
        &EntityFactory::CreateZone1SmallCircularSaw));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone1BigCircularSaw,
        &EntityFactory::CreateZone1BigCircularSaw));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2SmallCircularSaw,
        &EntityFactory::CreateZone2SmallCircularSaw));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2BigCircularSaw,
        &EntityFactory::CreateZone2BigCircularSaw));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3SmallCircularSaw,
        &EntityFactory::CreateZone3SmallCircularSaw));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3BigCircularSaw,
        &EntityFactory::CreateZone3BigCircularSaw));

    createTrapMethods.emplace(std::make_pair(TrapType::Zone1ProximitySpike,
        &EntityFactory::CreateZone1ProximitySpike));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2ProximitySpike,
        &EntityFactory::CreateZone2ProximitySpike));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3ProximitySpike,
        &EntityFactory::CreateZone3ProximitySpike));

    createTrapMethods.emplace(std::make_pair(TrapType::Zone1DefaultShooter,
        &EntityFactory::CreateZone1DefaultShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone1FastShooter,
        &EntityFactory::CreateZone1FastShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone1BigShooter,
        &EntityFactory::CreateZone1BigShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2DefaultShooter,
        &EntityFactory::CreateZone2DefaultShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2FastShooter,
        &EntityFactory::CreateZone2FastShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone2TripleShooter,
        &EntityFactory::CreateZone2TripleShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3DefaultShooter,
        &EntityFactory::CreateZone3DefaultShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3TripleShooter,
        &EntityFactory::CreateZone3TripleShooter));
    createTrapMethods.emplace(std::make_pair(TrapType::Zone3DoubleShooter,
        &EntityFactory::CreateZone3DoubleShooter));

    //ITEMS
    AddItemCreationFunctions();
}


//!BASE METHODS
Enemy* EntityFactory::CreateEnemy(EnemyType type, sf::Vector2f position)
{
    return (this->*createEnemyMethods.at(type))(position);
}

Bullet* EntityFactory::CreateBullet(BulletType type, BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks)
{
    return (this->*createBulletMethods.at(type))(generator, position, direction, executeCallbacks);
}

Consumable* EntityFactory::CreateConsumable(ConsumableType type, sf::Vector2f position)
{
    return (this->*createConsumableMethods.at(type))(position);
}

VendingMachine* EntityFactory::CreateVendingMachine(VendingMachineType type, sf::Vector2f position, uint64_t seed)
{
    return (this->*createVendingMachineMethods.at(type))(position, seed);
}

Trap* EntityFactory::CreateTrap(TrapType type, sf::Vector2f position, TrapDirection direction, float startDelay)
{
    return (this->*createTrapMethods.at(type))(position, direction, startDelay);
}

BaseItem* EntityFactory::CreateItem(ItemType type, sf::Vector2f position)
{
    return (createItemsFunctions.at(type)(position));
}

//!ENEMIES
//Flying eye
Enemy* EntityFactory::CreateFlyingEye(sf::Vector2f position)
{
    return new FlyingEye(position, 2.0f);
}

Enemy* EntityFactory::CreateRedFlyingEye(sf::Vector2f position)
{
    FlyingEye* enemy = new FlyingEye(position, 3.0f);
    enemy->SetHealth(enemy->GetHealth() * 2.0f);
    enemy->GetSprite().SetColor(sf::Color(255, 75, 75));
    enemy->SetSpeed(enemy->GetSpeed() * 0.6f);
    enemy->SetAttack(2);

    return enemy;
}

Enemy* EntityFactory::CreateGreenFlyingEye(sf::Vector2f position)
{
    FlyingEye* enemy = new FlyingEye(position, 2.0f);
    enemy->GetSprite().SetColor(sf::Color(100, 255, 100));
    enemy->SetSpeed(enemy->GetSpeed() * 1.25f);

    return enemy;
}

Enemy* EntityFactory::CreateBlueFlyingEye(sf::Vector2f position)
{
    FlyingEye* enemy = new FlyingEye(position, 1.0f);
    enemy->SetHealth(10.0f);
    enemy->GetSprite().SetColor(sf::Color(75, 75, 255));
    enemy->SetSpeed(enemy->GetSpeed() * 1.25f);

    return enemy;
}

//Possessed Circular Saw
Enemy* EntityFactory::CreatePossessedCircularSaw(sf::Vector2f position)
{
    PossessedCircularSaw* circularSaw = new PossessedCircularSaw(position);


    return circularSaw;
}

Enemy* EntityFactory::CreateFastPossessedCircularSaw(sf::Vector2f position)
{
    PossessedCircularSaw* circularSaw = new PossessedCircularSaw(position, 
        PossessedCircularSaw::BASE_HEALTH, 500.0f, 600.0f);

    return circularSaw;
}

Enemy* EntityFactory::CreateDoubleDamagePossessedCircularSaw(sf::Vector2f position)
{
    PossessedCircularSaw* circularSaw = new PossessedCircularSaw(position);
    circularSaw->SetAttack(2);

    return circularSaw;
}

//BatEye
Enemy* EntityFactory::CreateBatEye(sf::Vector2f position)
{
    return new BatEye(position, 2.5f);
}

Enemy* EntityFactory::CreateRedBatEye(sf::Vector2f position)
{
    BatEye* enemy = new BatEye(position, 3.0f);
    enemy->SetHealth(enemy->GetHealth() * 2.0f);
    enemy->GetSprite().SetColor(sf::Color(255, 100, 100));
    enemy->SetPatrollingSpeed(enemy->GetPatrollingSpeed() * 0.5f);
    enemy->SetFollowSpeed(enemy->GetFollowSpeed() * 0.5f);
    enemy->SetMaxNumShoots(2);

    BulletGenerator& generator = enemy->GetGenerator();
    generator.BulletColor = sf::Color(255, 150, 150);
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *=  0.5f;
    generator.BulletKnockBack *= 1.25f;
    generator.BulletSize *= 1.75f;
    generator.PointGenerators.push_back(TripleBulletPointGenerator::GetGlobalInstance());

    return enemy;
}

Enemy* EntityFactory::CreateGreenBatEye(sf::Vector2f position)
{
    BatEye* enemy = new BatEye(position, 3.0f);
    enemy->SetHealth(enemy->GetHealth() * 0.6f);
    enemy->GetSprite().SetColor(sf::Color(100, 255, 100));
    enemy->SetPatrollingSpeed(enemy->GetPatrollingSpeed() * 1.25f);
    enemy->SetFollowSpeed(enemy->GetFollowSpeed() * 1.25f);
    enemy->SetMaxNumShoots(3);

    BulletGenerator& generator = enemy->GetGenerator();
    generator.BulletColor = sf::Color(50, 255, 50);
    generator.BulletSpeed *=  0.5f;
    generator.BulletType = BulletType::RubberBullet;
    generator.BulletMaxAliveTime = 20.0f;

    return enemy;
}

Enemy* EntityFactory::CreateBlueBatEye(sf::Vector2f position)
{
    BatEye* enemy = new BatEye(position, 2.0f);
    enemy->SetHealth(10.0f);
    enemy->GetSprite().SetColor(sf::Color(100, 100, 255));
    enemy->SetPatrollingSpeed(enemy->GetPatrollingSpeed() * 1.15f);
    enemy->SetFollowSpeed(enemy->GetFollowSpeed() * 1.15f);

    auto& media = E2D::MediaContent::Instance();

    BulletGenerator& generator = enemy->GetGenerator();
    generator.BulletSpeed *=  0.75f;
    generator.BulletAnimationFrames[0] = media.GetAnimationFrames("SmallSpellBulletAlive");
    generator.BulletAnimationDurations[0] = 0.25f;
    generator.BulletTexturePadding = 12.0f;

    return enemy;
}

Enemy* EntityFactory::CreateAxeSkeleton(sf::Vector2f position)
{
    return new AxeSkeleton(position);
}

Enemy* EntityFactory::CreateRogueKnight(sf::Vector2f position)
{
    return new RogueKnight(position);
}

//!BULLETS
Bullet* EntityFactory::CreateDefaultBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks)
{
    return new Bullet(generator, position, direction, executeCallbacks);
}

Bullet* EntityFactory::CreateRubberBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks)
{
    return new RubberBullet(generator, position, direction, executeCallbacks);
}

//!CONSUMABLES
Consumable* EntityFactory::CreateRedHeart(sf::Vector2f position)
{
    return new Heart(position, 1, sf::Color(255, 0, 0));
}

Consumable* EntityFactory::CreateGoldenHeart(sf::Vector2f position)
{
    return new Heart(position, 2, sf::Color(255, 215, 0));
}

Consumable* EntityFactory::CreateBronceCoin(sf::Vector2f position)
{
    return new Coin(position, 10, sf::Color(205, 127, 50));
}

Consumable* EntityFactory::CreateSilverCoin(sf::Vector2f position)
{
    return new Coin(position, 25, sf::Color(240, 240, 240));
}

Consumable* EntityFactory::CreateGoldCoin(sf::Vector2f position)
{
    return new Coin(position, 50, sf::Color(255, 215, 0));
}

//!VENDING MACHINES
VendingMachine* EntityFactory::CreateItemVendingMachine(sf::Vector2f position, uint64_t seed)
{
    return new ItemVendingMachine(position, seed);
}

VendingMachine* EntityFactory::CreateHeartVendingMachine(sf::Vector2f position, uint64_t seed)
{
    return new HeartVendingMachine(position, seed);
}

VendingMachine* EntityFactory::CreateCoinVendingMachine(sf::Vector2f position, uint64_t seed)
{
    return new CoinVendingMachine(position, seed);
}

//!TRAPS

Trap* EntityFactory::CreateZone1SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone1TrapCircularSaw", 1.0f);
    trap->SetDamage(1);
    trap->SetSpeed(450.0f);

    return trap;
}

Trap* EntityFactory::CreateZone1BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone1TrapCircularSaw", 2.0f);
    trap->SetDamage(1);
    trap->SetSpeed(450.0f);

    return trap;
}

Trap* EntityFactory::CreateZone2SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone2TrapCircularSaw", 1.0f);
    trap->SetDamage(1);
    trap->SetSpeed(600.0f);

    return trap;
}

Trap* EntityFactory::CreateZone2BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone2TrapCircularSaw", 2.0f);
    trap->SetDamage(1);
    trap->SetSpeed(600.0f);

    return trap;
}

Trap* EntityFactory::CreateZone3SmallCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone3TrapCircularSaw", 1.0f);
    trap->SetDamage(2);
    trap->SetSpeed(600.0f);

    return trap;
}

Trap* EntityFactory::CreateZone3BigCircularSaw(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    CircularSawTrap* trap = new CircularSawTrap(position, direction, startDelay, "Zone3TrapCircularSaw", 2.0f);
    trap->SetDamage(2);
    trap->SetSpeed(600.0f);

    return trap;
}

Trap* EntityFactory::CreateZone1ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay)
{
    return new ProximitySpikeTrap(position, direction, "Zone1TrapProximitySpike", 1.0f, 1.0f, 1);
}

Trap* EntityFactory::CreateZone2ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay)
{
    return new ProximitySpikeTrap(position, direction, "Zone2TrapProximitySpike", 0.5f, 2.0f, 1);
}

Trap* EntityFactory::CreateZone3ProximitySpike(sf::Vector2f position, TrapDirection direction, [[maybe_unused]] float startDelay)
{
    return new ProximitySpikeTrap(position, direction, "Zone3TrapProximitySpike", 0.5f, 2.0f, 2);
}

Trap* EntityFactory::CreateZone1DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 3.0f);

    return trap;
}

Trap* EntityFactory::CreateZone1FastShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 2.0f);

    return trap;
}

Trap* EntityFactory::CreateZone1BigShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 4.0f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *= 0.5f;
    generator.BulletSize *= 2.0f;
    generator.BulletColor = sf::Color(255, 64, 0);

    return trap;
}

Trap* EntityFactory::CreateZone2DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 2.5f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *= 1.5f;
    generator.BulletColor = sf::Color(255, 128, 64);

    return trap;
}

Trap* EntityFactory::CreateZone2FastShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 1.5f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 1;
    generator.BulletSpeed *= 1.5f;
    generator.BulletColor = sf::Color(255, 128, 64);

    return trap;
}

Trap* EntityFactory::CreateZone2TripleShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 3.5f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletColor = sf::Color(128, 128, 255);
    generator.PointGenerators.push_back(TripleBulletPointGenerator::GetGlobalInstance());

    return trap;
}

Trap* EntityFactory::CreateZone3DefaultShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 1.5f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *= 2.0f;
    generator.BulletColor = sf::Color(255, 255, 0);

    return trap;
}

Trap* EntityFactory::CreateZone3TripleShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 2.0f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *= 1.5f;
    generator.BulletColor = sf::Color(128, 128, 255);
    generator.PointGenerators.push_back(TripleBulletPointGenerator::GetGlobalInstance());

    return trap;
}

Trap* EntityFactory::CreateZone3DoubleShooter(sf::Vector2f position, TrapDirection direction, float startDelay)
{
    ShooterTrap* trap = new ShooterTrap(position, direction, startDelay, "TrapShooter", 2.5f);

    BulletGenerator& generator = trap->GetGenerator();
    generator.BulletPlayerDamage = 2;
    generator.BulletSpeed *= 1.5f;
    generator.BulletColor = sf::Color(128, 255, 128);
    generator.BulletType = BulletType::RubberBullet;
    generator.PointGenerators.push_back(AngledDoubleBulletPointGenerator::GetGlobalInstance());

    return trap;
}


//! ITEMS

void EntityFactory::AddItemCreationFunctions()
{
    createItemsFunctions.emplace(ItemType::CannibalismItem, 
        [](sf::Vector2f position) { return new CannibalismItem(position); });
    createItemsFunctions.emplace(ItemType::GoldenAppleItem, 
        [](sf::Vector2f position) { return new GoldenAppleItem(position); });
    createItemsFunctions.emplace(ItemType::MushroomItem, 
        [](sf::Vector2f position) { return new MushroomItem(position); });
    createItemsFunctions.emplace(ItemType::AidKitItem, 
        [](sf::Vector2f position) { return new AidKitItem(position); });
    createItemsFunctions.emplace(ItemType::QuickHoodItem, 
        [](sf::Vector2f position) { return new QuickHoodItem(position); });
    createItemsFunctions.emplace(ItemType::QuillItem, 
        [](sf::Vector2f position) { return new QuillItem(position); });
    createItemsFunctions.emplace(ItemType::JumpBootsItem, 
        [](sf::Vector2f position) { return new JumpBootsItem(position); });
    createItemsFunctions.emplace(ItemType::FlyingHelmetItem, 
        [](sf::Vector2f position) { return new FlyingHelmetItem(position); });
    createItemsFunctions.emplace(ItemType::WarAxItem, 
        [](sf::Vector2f position) { return new WarAxItem(position); });
    createItemsFunctions.emplace(ItemType::SoyMilkItem, 
        [](sf::Vector2f position) { return new SoyMilkItem(position); });
    createItemsFunctions.emplace(ItemType::DoubleDeagleItem, 
        [](sf::Vector2f position) { return new DoubleDeagleItem(position); });
    createItemsFunctions.emplace(ItemType::ShotGunItem, 
        [](sf::Vector2f position) { return new ShotGunItem(position); });
    createItemsFunctions.emplace(ItemType::CannonItem, 
        [](sf::Vector2f position) { return new CannonItem(position); });
    createItemsFunctions.emplace(ItemType::BurstShotItem, 
        [](sf::Vector2f position) { return new BurstShotItem(position); });
    createItemsFunctions.emplace(ItemType::SantoryuItem, 
        [](sf::Vector2f position) { return new SantoryuItem(position); });
    createItemsFunctions.emplace(ItemType::SandClockItem, 
        [](sf::Vector2f position) { return new SandClockItem(position); });
    createItemsFunctions.emplace(ItemType::MoreAmmoItem, 
        [](sf::Vector2f position) { return new MoreAmmoItem(position); });
        createItemsFunctions.emplace(ItemType::DiamondItem, 
    [](sf::Vector2f position) { return new DiamondItem(position); });
    createItemsFunctions.emplace(ItemType::WizardHatItem, 
        [](sf::Vector2f position) { return new WizardHatItem(position); });
    createItemsFunctions.emplace(ItemType::BigEyeItem, 
        [](sf::Vector2f position) { return new BigEyeItem(position); });
    createItemsFunctions.emplace(ItemType::CanOfSodaItem, 
        [](sf::Vector2f position) { return new CanOfSodaItem(position); });
    createItemsFunctions.emplace(ItemType::PaladinShieldItem, 
        [](sf::Vector2f position) { return new PaladinShieldItem(position); });
    createItemsFunctions.emplace(ItemType::MartinItem, 
        [](sf::Vector2f position) { return new MartinItem(position); });
    createItemsFunctions.emplace(ItemType::RedBalloonItem, 
        [](sf::Vector2f position) { return new RedBalloonItem(position); });
    createItemsFunctions.emplace(ItemType::BlueSoulItem, 
        [](sf::Vector2f position) { return new BlueSoulItem(position); });
    createItemsFunctions.emplace(ItemType::HealthRingItem, 
        [](sf::Vector2f position) { return new HealthRingItem(position); });
    createItemsFunctions.emplace(ItemType::SwiftGlovesItem, 
        [](sf::Vector2f position) { return new SwiftGlovesItem(position); });
    createItemsFunctions.emplace(ItemType::WarHornItem, 
        [](sf::Vector2f position) { return new WarHornItem(position); });
    createItemsFunctions.emplace(ItemType::SolarCapeItem, 
        [](sf::Vector2f position) { return new SolarCapeItem(position); });
    createItemsFunctions.emplace(ItemType::FireWandItem, 
        [](sf::Vector2f position) { return new FireWandItem(position); });
    createItemsFunctions.emplace(ItemType::GhostFruitItem, 
        [](sf::Vector2f position) { return new GhostFruitItem(position); });
    createItemsFunctions.emplace(ItemType::HeavyBulletsItem, 
        [](sf::Vector2f position) { return new HeavyBulletsItem(position); });
    createItemsFunctions.emplace(ItemType::VampirismItem, 
        [](sf::Vector2f position) { return new VampirismItem(position); });
    createItemsFunctions.emplace(ItemType::SoapItem, 
        [](sf::Vector2f position) { return new SoapItem(position); });
    createItemsFunctions.emplace(ItemType::GoldenDeagleItem, 
        [](sf::Vector2f position) { return new GoldenDeagleItem(position); });
    createItemsFunctions.emplace(ItemType::RocketsItem, 
        [](sf::Vector2f position) { return new RocketsItem(position); });
    createItemsFunctions.emplace(ItemType::WatermelonItem, 
        [](sf::Vector2f position) { return new WatermelonItem(position); });
    createItemsFunctions.emplace(ItemType::MiniGunItem, 
        [](sf::Vector2f position) { return new MiniGunItem(position); });
    createItemsFunctions.emplace(ItemType::TntItem, 
        [](sf::Vector2f position) { return new TntItem(position); });
    createItemsFunctions.emplace(ItemType::BloodyShieldItem, 
        [](sf::Vector2f position) { return new BloodyShieldItem(position); });
    createItemsFunctions.emplace(ItemType::BloodyCoinItem, 
        [](sf::Vector2f position) { return new BloodyCoinItem(position); });
    createItemsFunctions.emplace(ItemType::RareCandyItem, 
        [](sf::Vector2f position) { return new RareCandyItem(position); });
    createItemsFunctions.emplace(ItemType::PineAppleItem, 
        [](sf::Vector2f position) { return new PineAppleItem(position); });
    createItemsFunctions.emplace(ItemType::FaultyBombItem, 
        [](sf::Vector2f position) { return new FaultyBombItem(position); });
    createItemsFunctions.emplace(ItemType::FrozenTunaItem, 
        [](sf::Vector2f position) { return new FrozenTunaItem(position); });
    createItemsFunctions.emplace(ItemType::QuadShotItem, 
        [](sf::Vector2f position) { return new QuadShotItem(position); });
    createItemsFunctions.emplace(ItemType::HappyRockItem, 
        [](sf::Vector2f position) { return new HappyRockItem(position); });
    createItemsFunctions.emplace(ItemType::CursedHeartItem, 
        [](sf::Vector2f position) { return new CursedHeartItem(position); });
    createItemsFunctions.emplace(ItemType::AstronautHelmetItem, 
        [](sf::Vector2f position) { return new AstronautHelmetItem(position); });
}        
