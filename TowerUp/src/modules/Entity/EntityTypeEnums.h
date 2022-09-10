#pragma once

#include <cstdint>

enum class DoorType
{
    Default = 0,
    Shop,
    Treasure,
    Boss,
    NewLevel,
    MaxDoorTypes
};

enum class EnemyType : int32_t
{
    FlyingEye = 0,
    RedFlyingEye = 1,
    GreenFlyingEye = 2,
    BlueFlyingEye = 3,
    PossessedCircularSaw = 4,
    FastPossessedCircularSaw = 5,
    DoubleDamagePossessedCircularSaw = 6,
    BatEye = 7,
    RedBatEye = 8,
    GreenBatEye = 9,
    BlueBatEye = 10,
    RogueKnight = 11, //BOSS
    AxeSkeleton = 12,
    MaxEnemyTypes
};

enum class BulletType : int32_t
{
    Default,
    RubberBullet,
    MaxBulletTypes
};

enum class ConsumableType : int32_t
{
    RedHeart,
    GoldenHeart,
    BronceCoin,
    SilverCoin,
    GoldCoin,
    MaxConsumableTypes
};

enum class VendingMachineType : int32_t
{
    ItemVendingMachine,
    HeartVendingMachine,
    CoinVendingMachine,
    MaxVendingMachineTypes
};


enum class ItemType : int32_t
{
    CannibalismItem,
    GoldenAppleItem,
    MushroomItem,
    AidKitItem,
    QuickHoodItem,
    QuillItem,
    JumpBootsItem,
    FlyingHelmetItem,
    WarAxItem,
    SoyMilkItem,
    DoubleDeagleItem,
    ShotGunItem,
    CannonItem,
    BurstShotItem,
    SantoryuItem,
    SandClockItem,
    MoreAmmoItem,
    DiamondItem,
    WizardHatItem,
    BigEyeItem,
    CanOfSodaItem,
    PaladinShieldItem,
    MartinItem,
    RedBalloonItem,
    BlueSoulItem,
    HealthRingItem,
    SwiftGlovesItem,
    WarHornItem,
    SolarCapeItem,
    FireWandItem,
    GhostFruitItem,
    HeavyBulletsItem,
    VampirismItem,
    SoapItem,
    GoldenDeagleItem,
    RocketsItem,
    WatermelonItem,
    MiniGunItem,
    TntItem,
    BloodyShieldItem,
    BloodyCoinItem,
    RareCandyItem,
    PineAppleItem,
    FaultyBombItem,
    FrozenTunaItem,
    QuadShotItem,
    HappyRockItem,
    CursedHeartItem,
    AstronautHelmetItem,
    MaxItemTypes
};

enum class GenericEntityType : int32_t
{
    DefaultExplosion,
    MaxGenericEntityTypes
};

enum class TrapType : int32_t
{
    Zone1SmallCircularSaw        = 0,
    Zone1BigCircularSaw          = 1,
    Zone2SmallCircularSaw        = 2,
    Zone2BigCircularSaw          = 3,
    Zone3SmallCircularSaw        = 4,
    Zone3BigCircularSaw          = 5, 
    Zone1ProximitySpike          = 6,
    Zone2ProximitySpike          = 7,
    Zone3ProximitySpike          = 8,
    Zone1DefaultShooter          = 9,
    Zone1FastShooter             = 10,
    Zone1BigShooter              = 11,
    Zone2DefaultShooter          = 12,
    Zone2TripleShooter           = 13,
    Zone2FastShooter             = 14,
    Zone3DefaultShooter          = 15,
    Zone3TripleShooter           = 16,
    Zone3DoubleShooter           = 17
};

enum class TrapDirection : int32_t
{
    Top = 0,
    Right = 1,
    Bottom = 2,
    Left = 3
};