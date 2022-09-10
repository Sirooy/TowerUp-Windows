#include "Items.h"
#include "World.h"
#include "Player.h"
#include "DesertEagle.h"
#include "BulletGenerator.h"
#include "ShootMode.h"
#include "Consumable.h"
#include "Enemies/Enemy.h"
#include "../Physics/PhysicsEngine.h"
#include "../Engine2D/MediaContent.h"
#include "../Random.h"
#include "../SFMLUtils.hpp"
#include "Bullet.h"


CannibalismItem::CannibalismItem(sf::Vector2f position) : BaseItem(position, "Canibalismo", 0, ItemRarity::Lengendary) { }

void CannibalismItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->SetMaxHealth(2);
    deagle.SetBulletDamage(player->GetDeagle().GetBulletDamage() * 2.0f);
    deagle.AddAttackSpeed(0.025f);
    deagle.AddBulletKnockBack(50.0f);
    deagle.RemoveAccuracy(0.05f);
    deagle.SetLightColor(255, 0, 0);
}

GoldenAppleItem::GoldenAppleItem(sf::Vector2f position) : BaseItem(position, "Manzana dorada", 1, ItemRarity::Common) { }

void GoldenAppleItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddMaxHealth(2);
    World::Instance().GetPlayer()->GetDeagle().RemoveRecoil(0.1f);
    World::Instance().GetPlayer()->GetDeagle().AddBulletDamage(5.0f);
}


MushroomItem::MushroomItem(sf::Vector2f position) : BaseItem(position, "Seta comun", 2, ItemRarity::Common) { }

void MushroomItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddMaxHealth(2);
    World::Instance().GetPlayer()->AddCurrentHealth(4);
}

AidKitItem::AidKitItem(sf::Vector2f position) : BaseItem(position, "Ultimo auxilio", 3, ItemRarity::Common) { }

void AidKitItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddMaxHealth(4);
    World::Instance().GetPlayer()->AddCurrentHealth(19); 
}

QuickHoodItem::QuickHoodItem(sf::Vector2f position) : BaseItem(position, "Capucha veloz", 4, ItemRarity::Uncommon) { }

void QuickHoodItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddDashPower(125.0f);
    World::Instance().GetPlayer()->AddMoveSpeed(100.0f);
}

QuillItem::QuillItem(sf::Vector2f position) : BaseItem(position, "Pluma liviana", 5, ItemRarity::Uncommon) { }

void QuillItem::OnPickUp()
{
    World::Instance().GetPlayer()->RemoveDashCooldown(0.2f);
    World::Instance().GetPlayer()->AddJumpMaxTime(0.2f);
    World::Instance().GetPlayer()->AddDashPower(25.0f);
}

JumpBootsItem::JumpBootsItem(sf::Vector2f position) : BaseItem(position, "Botas de salto", 7, ItemRarity::Common) { }

void JumpBootsItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddJumpMaxTime(0.25f);
    player->AddDoubleJumps(2);
    player->RemoveDashCooldown(0.1f);
}

FlyingHelmetItem::FlyingHelmetItem(sf::Vector2f position) : BaseItem(position, "Casco volador", 8, ItemRarity::Common) { }

void FlyingHelmetItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddJumpMaxTime(0.4f);
    World::Instance().GetPlayer()->AddJumpPower(50.0f);
}

WarAxItem::WarAxItem(sf::Vector2f position) : BaseItem(position, "Hacha de guerra", 9, ItemRarity::Rare) { }

void WarAxItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletDamage(deagle.GetBulletDamage() * 1.25f);
    deagle.AddBulletSpeed(50.0f);
    deagle.AddBulletKnockBack(50.0f);
    deagle.AddBulletSize(2.0f);
}

SoyMilkItem::SoyMilkItem(sf::Vector2f position) : BaseItem(position, "Leche de soja", 6, ItemRarity::Lengendary) { }

void SoyMilkItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();

    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetShootMode(new AutomaticShotMode());
    deagle.SetAttackSpeed(deagle.GetAttackSpeed() * 0.3f);
    deagle.SetBulletDamage(deagle.GetBulletDamage() * 0.4f);
    deagle.SetBulletSize(4.0f);
    deagle.AddMaxAmmo(16);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("DefaultBulletAlive"), media.GetAnimationFrames("DefaultBulletDead"));
    deagle.SetLightColor(200, 200, 200);
}

DoubleDeagleItem::DoubleDeagleItem(sf::Vector2f position) : BaseItem(position, "Deagles dobles", 10, ItemRarity::Mythical) { }

void DoubleDeagleItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddBulletPointGenerator(DoubleBulletPointGenerator::GetGlobalInstance());
    deagle.AddMaxAmmo(2);
    deagle.AddRecoil(50.0f);
}

ShotGunItem::ShotGunItem(sf::Vector2f position) : BaseItem(position, "Escopeta", 11, ItemRarity::Mythical) { }

void ShotGunItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddBulletPointGenerator(ShotGunBulletPointGenerator::GetGlobalInstance());
    deagle.SetBulletDamage(deagle.GetBulletDamage() * 0.4f);
    deagle.AddRecoil(100.0f);
    deagle.SetBulletSize(deagle.GetBulletSize() * 0.8f);
}

CannonItem::CannonItem(sf::Vector2f position) : BaseItem(position, "Cañon", 12, ItemRarity::Lengendary) { }

void CannonItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetShootMode(new ChargedShotMode());
    deagle.RemoveAttackSpeed(0.1f);
    deagle.AddBulletSize(4.0f);
    deagle.AddRecoil(200.0f);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("DefaultBlackBulletAlive"), media.GetAnimationFrames("DefaultBlackBulletDead"));
    deagle.SetBulletAnimationDurations(0.5f, 0.5f);
    deagle.SetBulletTexturePadding(3.0f);
}

BurstShotItem::BurstShotItem(sf::Vector2f position) : BaseItem(position, "Disparo en rafaga", 14, ItemRarity::Lengendary) { }

void BurstShotItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetShootMode(new BurstShotMode());
    deagle.RemoveAttackSpeed(0.15f);
    deagle.AddRecoil(50.0f);
}

SantoryuItem::SantoryuItem(sf::Vector2f position) : BaseItem(position, "Santoryu", 13, ItemRarity::Epic) { }

void SantoryuItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddBulletPointGenerator(TripleBulletPointGenerator::GetGlobalInstance());
    deagle.RemoveAttackSpeed(0.1f);
    deagle.AddRecoil(50.0f);
}

SandClockItem::SandClockItem(sf::Vector2f position) : BaseItem(position, "Reloj de invulnerabilidad", 20, ItemRarity::Mythical) { }

void SandClockItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddInvulnerabilityTime(3.0f);
}


MoreAmmoItem::MoreAmmoItem(sf::Vector2f position) : BaseItem(position, "Cargador XXL", 26, ItemRarity::Common) { }

void MoreAmmoItem::OnPickUp()
{
    World::Instance().GetPlayer()->GetDeagle().AddMaxAmmo(6);
    World::Instance().GetPlayer()->GetDeagle().RemoveReloadTime(0.15f);
}


DiamondItem::DiamondItem(sf::Vector2f position) : BaseItem(position, "Diamante", 21, ItemRarity::Rare) { }

void DiamondItem::OnPickUp()
{
    World::Instance().GetPlayer()->AddMoney(500);
}

WizardHatItem::WizardHatItem(sf::Vector2f position) : BaseItem(position, "Gorro de mago", 18, ItemRarity::Mythical) { }

void WizardHatItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulleType(BulletType::RubberBullet);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("SmallSpellBulletAlive"), media.GetAnimationFrames("SmallSpellBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.1);
    deagle.SetBulletTexturePadding(12.0f);
    deagle.AddMaxAmmo(2);
    deagle.RemoveAttackSpeed(0.05f);
    deagle.AddBulletMaxAliveTime(6.0f);
    deagle.SetLightColor(255, 0, 255);
}

BigEyeItem::BigEyeItem(sf::Vector2f position) : BaseItem(position, "Ojo grande", 28, ItemRarity::Rare) { }

void BigEyeItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddAccuracy(1.0f);
    deagle.AddBulletDamage(10.0f);
    deagle.AddBulletSpeed(100.0f);
    deagle.AddBulletMaxAliveTime(4.0f);
}

CanOfSodaItem::CanOfSodaItem(sf::Vector2f position) : BaseItem(position, "Lata de refresco", 16, ItemRarity::Lengendary) { }

void CanOfSodaItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->AddMoveSpeed(200.0f);
    player->AddJumpPower(100.0f);
    deagle.AddBulletSpeed(150.0f);
    deagle.RemoveAccuracy(0.1f);
    deagle.AddAttackSpeed(0.1f);
}

PaladinShieldItem::PaladinShieldItem(sf::Vector2f position) : BaseItem(position, "Escudo de paladin", 19, ItemRarity::Uncommon) { }

void PaladinShieldItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddMaxHealth(2);
    player->GetDeagle().AddBulletDamage(10.0f);
    player->GetDeagle().AddMaxAmmo(2);
}

MartinItem::MartinItem(sf::Vector2f position) : BaseItem(position, "Martin", 25, ItemRarity::Mythical) { }

void MartinItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->AddMaxHealth(4);
    player->AddCurrentHealth(8);
    player->AddInvulnerabilityTime(0.5f);
    player->AddMoney(50);
    player->AddDoubleJumps(1);
    player->AddDoubleJumpPower(100.0f);
    player->AddMoveSpeed(50.0f);
    player->AddDashPower(100.0f);

    deagle.AddBulletDamage(25.0f);
    deagle.AddMaxAmmo(4);
    deagle.AddAccuracy(0.1f);
    deagle.AddAttackSpeed(0.1f);
    deagle.AddBulletKnockBack(50.0f);
    deagle.AddBulletMaxAliveTime(2.0f);
    deagle.AddBulletSpeed(50.0f);
    deagle.RemoveReloadTime(0.1f);
    deagle.RemoveRecoil(50.0f);
    deagle.AddBulletSize(2.0f);
}

RedBalloonItem::RedBalloonItem(sf::Vector2f position) : BaseItem(position, "Globo rojo", 17, ItemRarity::Common) { }

void RedBalloonItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddJumpMaxTime(0.1f);
    player->AddDoubleJumps(1);
}

BlueSoulItem::BlueSoulItem(sf::Vector2f position) : BaseItem(position, "Alma azul", 15, ItemRarity::Mythical) { }

void BlueSoulItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->AddMoveSpeed(50.0f);
    player->AddDoubleJumps(1);

    deagle.AddBulletDamage(15.0f);
    deagle.AddBulletSize(2.0f);
    deagle.AddBulletKnockBack(20.0f);
    deagle.AddBulletSpeed(50.0f);
    deagle.AddBulletPointGenerator(CrossBulletPointGenerator::GetGlobalInstance());
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("KamekamehaBulletAlive"), media.GetAnimationFrames("KamekamehaBulletDead"));
    deagle.SetBulletAnimationDurations(0.2f, 0.2f);
    deagle.SetBulletTexturePadding(23.0f);
    deagle.SetLightColor(50, 50, 255);
}

HealthRingItem::HealthRingItem(sf::Vector2f position) : BaseItem(position, "Banda de vida", 23, ItemRarity::Uncommon) { }

void HealthRingItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddMaxHealth(4);
    player->AddCurrentHealth(6);
    player->AddInvulnerabilityTime(0.5f);
}

SwiftGlovesItem::SwiftGlovesItem(sf::Vector2f position) : BaseItem(position, "Guantes veloces", 27, ItemRarity::Rare) { }

void SwiftGlovesItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    deagle.RemoveAccuracy(0.2f);
    deagle.AddAttackSpeed(0.5f);
    deagle.RemoveReloadTime(0.25f);
    deagle.AddBulletDamage(5.0f);
}

WarHornItem::WarHornItem(sf::Vector2f position) : BaseItem(position, "Cuerno de guerra", 29, ItemRarity::Common) { }

void WarHornItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    deagle.AddBulletDamage(10.0f);
    deagle.AddMaxAmmo(1);
    deagle.RemoveReloadTime(0.1f);
}

SolarCapeItem::SolarCapeItem(sf::Vector2f position) : BaseItem(position, "Capa solar", 22, ItemRarity::Mythical) { }

void SolarCapeItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->RemoveDashCooldown(0.2f);
    player->AddDashPower(100.0f);
    deagle.AddBulletDamage(20.0f);
    deagle.AddBulletSpeed(100.0f);
    deagle.AddBulletSize(3.0f);
    deagle.RemoveReloadTime(0.1f);
    deagle.AddAttackSpeed(0.05f);
    deagle.SetBulletTexturePadding(12.0f);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("FireBallBulletAlive"), media.GetAnimationFrames("FireBallBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.25f);
    deagle.SetLightColor(255, 128, 0);
}

FireWandItem::FireWandItem(sf::Vector2f position) : BaseItem(position, "Varita de fuego", 24, ItemRarity::Lengendary) { }

void FireWandItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->RemoveDashCooldown(0.2f);
    player->AddDashPower(100.0f);
    deagle.AddBulletDamage(10.0f);
    deagle.AddBulletSpeed(50.0f);
    deagle.AddBulletKnockBack(150.0f);
    deagle.AddMaxAmmo(4);
    deagle.AddRecoil(100.0f);
    deagle.SetBulletTexturePadding(8.0f);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("BigWaveBulletAlive"), media.GetAnimationFrames("BigWaveBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.25f);
    deagle.SetLightColor(255, 128, 0);
}

GhostFruitItem::GhostFruitItem(sf::Vector2f position) : BaseItem(position, "Fruta fantasma", 30, ItemRarity::Uncommon) { }

void GhostFruitItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    sf::Color color = deagle.GetBulletColor();
    color.a = 255 / 2;

    deagle.RemoveBulletCollisionMask(CollisionType::TileCollisionType);
    deagle.SetBulletColor(color);
}

HeavyBulletsItem::HeavyBulletsItem(sf::Vector2f position) : BaseItem(position, "Balas pesadas", 31, ItemRarity::Rare) { }

void HeavyBulletsItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletBaseAcceleration(PhysicsEngine::BaseGravity * 0.25f);
    deagle.SetBulletDamage(deagle.GetBulletDamage() * 2.0f);
    deagle.AddRecoil(50.0f);
    deagle.AddBulletSpeed(150.0f);
    deagle.AddBulletSize(2.0f);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("DefaultBlackBulletAlive"), media.GetAnimationFrames("DefaultBlackBulletDead"));
    deagle.SetBulletAnimationDurations(0.5f, 0.5f);
    deagle.SetBulletTexturePadding(3.0f);
}

VampirismItem::VampirismItem(sf::Vector2f position) : BaseItem(position, "Vampirismo", 32, ItemRarity::Mythical) { }

void VampirismItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("SmallWaveBulletAlive"), media.GetAnimationFrames("SmallWaveBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.25f);
    deagle.SetBulletTexturePadding(0.0f);
    deagle.RemoveBulletDamage(5.0f);
    deagle.SetLightColor(255, 0, 0);

    deagle.AddBulletOnCollisionWithEnemyCallback([]([[maybe_unused]] Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.1f)
            World::Instance().GetPlayer()->AddCurrentHealth(1);
    });
}

SoapItem::SoapItem(sf::Vector2f position) : BaseItem(position, "Jabon", 33, ItemRarity::Uncommon) { }

void SoapItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletBaseAcceleration(PhysicsEngine::BaseGravity * -0.25f);
    deagle.AddBulletSpeed(250.0f);
    deagle.SetBulletFriction(0.1f);
    deagle.AddBulletSize(1.0f);
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("BubbleBulletAlive"), media.GetAnimationFrames("BubbleBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.5f);
    deagle.SetBulletTexturePadding(0.0f);
    deagle.AddAccuracy(0.05f);
    deagle.RemoveRecoil(100.0f);
    deagle.AddAttackSpeed(0.025f);
    deagle.AddBulletMaxAliveTime(2.5f);
    deagle.SetShootMode(new AutomaticShotMode());
    deagle.SetLightColor(80, 160, 255);
}

GoldenDeagleItem::GoldenDeagleItem(sf::Vector2f position) : BaseItem(position, "Deagle Dorada", 34, ItemRarity::Mythical) { }

void GoldenDeagleItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("CoinBulletAlive"), media.GetAnimationFrames("CoinBulletDead"));
    deagle.SetBulletAnimationDurations(0.5f, 0.5f);
    deagle.SetBulletTexturePadding(3.0f);
    deagle.AddAttackSpeed(0.01f);
    deagle.GetSprite().SetColor(sf::Color::Yellow);
    deagle.RemoveBulletDamage(10.0f);
    deagle.RemoveBulletMaxAliveTime(3.0f);
    deagle.SetLightColor(255, 255, 0);

    deagle.AddBulletOnCollisionWithEnemyCallback([](Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.15f)
        {
            constexpr ConsumableType Types[] = { ConsumableType::BronceCoin, ConsumableType::SilverCoin, ConsumableType::GoldCoin };
            Consumable* c = World::Instance().CreateConsumable(Types[Random::GlobalInstance.NextInt(3)], b->Position);

            float throwAngle = Random::GlobalInstance.NextFloat(sf::PI_OVER_4, sf::PI - sf::PI_OVER_4);
            sf::Vector2f direction = sf::Vector2f(std::cos(throwAngle), -std::sin(throwAngle));
            c->Velocity = direction * 500.0f;
        }
    });
}

RocketsItem::RocketsItem(sf::Vector2f position) : BaseItem(position, "Cohetes", 35, ItemRarity::Mythical) { }

void RocketsItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("RocketBulletAlive"), media.GetAnimationFrames("RocketBulletDead"));
    deagle.SetBulletAnimationDurations(0.5f, 0.1f);
    deagle.SetBulletTexturePadding(4.0f);
    deagle.RemoveAccuracy(0.08f);
    deagle.RemoveAttackSpeed(0.15f);
    deagle.AddBulletSpeed(150.0f);
    deagle.AddBulletKnockBack(200.0f);
    deagle.SetLightColor(255, 128, 0);

    deagle.AddBulletOnCollisionWithEnemyCallback([](Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        World::Instance().CreateExplosion(b->Position, b->Size * 4.0f, b->Damage * 2.0f, 0, CollisionType::EnemyCollisionType);
    });

    deagle.AddBulletOnCollisionWithTileCallback([](Bullet* b, [[maybe_unused]] const TileCollisionData& data) 
    {
        World::Instance().CreateExplosion(b->Position, b->Size * 4.0f, b->Damage * 2.0f, 0, CollisionType::EnemyCollisionType);
    });

}

WatermelonItem::WatermelonItem(sf::Vector2f position) : BaseItem(position, "Sandia", 36, ItemRarity::Mythical) { }

void WatermelonItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("WatermelonBulletAlive"), media.GetAnimationFrames("WatermelonBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.1f);
    deagle.SetBulletTexturePadding(2.0f);
    deagle.AddBulletDamage(5.0f);
    deagle.AddBulletSize(4.0f);
    deagle.AddBulletKnockBack(100.0f);
    deagle.SetLightColor(0, 255, 0);

    deagle.AddBulletOnCollisionWithTileCallback([](Bullet* b, [[maybe_unused]] const TileCollisionData& data) 
    {
        auto& world = World::Instance();
        auto& generator = world.GetPlayer()->GetDeagle().GetGenerator();

        float bulletSize = generator.BulletSize;
        float bulletDamage = generator.BulletDamage;
        generator.BulletSize *= 0.5f;
        generator.BulletDamage *= 0.5f;

        int32_t numBullets = Random::GlobalInstance.RangeInt(1, 3);
        for(int32_t i = 0;i < numBullets; ++i)
        {
            float angle = Random::GlobalInstance.NextFloat(sf::PI_OVER_4) * Random::GlobalInstance.NextFloatSign();
            sf::Vector2f dir = sf::rotate(data.Normal, angle);
            world.CreateBullet(generator.BulletType, &generator, b->Position + data.Normal * b->Size, dir, false);
        }

        generator.BulletSize = bulletSize;
        generator.BulletDamage = bulletDamage;
    });

    deagle.AddBulletOnCollisionWithEnemyCallback([](Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        auto& world = World::Instance();
        auto& generator = world.GetPlayer()->GetDeagle().GetGenerator();

        float bulletSize = generator.BulletSize;
        float bulletDamage = generator.BulletDamage;
        generator.BulletSize *= 0.5f;
        generator.BulletDamage *= 0.5f;

        int32_t numBullets = Random::GlobalInstance.RangeInt(1, 3);
        for(int32_t i = 0;i < numBullets; ++i)
        {
            float angle = Random::GlobalInstance.NextFloat(sf::PI_OVER_4) * Random::GlobalInstance.NextFloatSign();
            sf::Vector2f dir = sf::rotate(data.Normal, angle);
            world.CreateBullet(generator.BulletType, &generator, b->Position + data.Normal * b->Size, dir, false);
        }

        generator.BulletSize = bulletSize;
        generator.BulletDamage = bulletDamage;
    });
}

MiniGunItem::MiniGunItem(sf::Vector2f position) : BaseItem(position, "Minigun", 38, ItemRarity::Mythical) { }

void MiniGunItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddMaxAmmo(50);
    deagle.SetShootMode(new MiniGunShotMode());
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("DefaultFireShotBulletAlive"), media.GetAnimationFrames("DefaultFireShotBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.1f);
    deagle.SetBulletTexturePadding(10.0f);
    deagle.AddAttackSpeed(0.05f);
    deagle.SetLightColor(255, 255, 255);
}

TntItem::TntItem(sf::Vector2f position) : BaseItem(position, "TNT", 39, ItemRarity::Uncommon) { }

void TntItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddOnTakeDamageCallback([](Player* player, [[maybe_unused]]int32_t amount) 
    {
        World::Instance().CreateExplosion(player->Position, 350.0f, 
            player->GetDeagle().GetBulletDamage() * 3.0f, 0, CollisionType::EnemyCollisionType);
    });
}

BloodyShieldItem::BloodyShieldItem(sf::Vector2f position) : BaseItem(position, "Escudo ensangrentado", 40, ItemRarity::Rare) { }

void BloodyShieldItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddOnTakeDamageCallback([](Player* player, [[maybe_unused]]int32_t amount) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.25f)
            player->AddCurrentHealth(amount + 1);
    });
}

BloodyCoinItem::BloodyCoinItem(sf::Vector2f position) : BaseItem(position, "Moneda ensangrentada", 41, ItemRarity::Rare) { }

void BloodyCoinItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddOnTakeDamageCallback([](Player* player, [[maybe_unused]]int32_t amount) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.5f)
        {
            constexpr ConsumableType Types[] = { ConsumableType::BronceCoin, ConsumableType::SilverCoin, ConsumableType::GoldCoin };
            Consumable* c = World::Instance().CreateConsumable(Types[Random::GlobalInstance.NextInt(3)], 
                player->Position - sf::Vector2f(0.0f, Player::HEIGHT));

            float throwAngle = Random::GlobalInstance.NextFloat(sf::PI_OVER_4, sf::PI - sf::PI_OVER_4);
            sf::Vector2f direction = sf::Vector2f(std::cos(throwAngle), -std::sin(throwAngle));
            c->Velocity = direction * 500.0f;
        }
    });
}

RareCandyItem::RareCandyItem(sf::Vector2f position) : BaseItem(position, "Caramelo raro", 42, ItemRarity::Rare) { }

void RareCandyItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    DesertEagle& deagle = player->GetDeagle();
    player->AddMaxHealth(2);
    player->AddCurrentHealth(2);
    player->AddInvulnerabilityTime(0.5f);
    player->AddMoney(25);
    player->AddDoubleJumps(1);
    player->AddDashPower(50.0f);

    deagle.AddBulletDamage(10.0f);
    deagle.AddMaxAmmo(2);
    deagle.AddAccuracy(0.05f);
    deagle.AddAttackSpeed(0.01f);
    deagle.AddBulletKnockBack(25.0f);
    deagle.AddBulletMaxAliveTime(1.0f);
    deagle.RemoveReloadTime(0.05f);
    deagle.RemoveRecoil(25.0f);
}

PineAppleItem::PineAppleItem(sf::Vector2f position) : BaseItem(position, "Pina", 43, ItemRarity::Mythical) { }

void PineAppleItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("PineAppleBulletAlive"), media.GetAnimationFrames("PineAppleBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.1f);
    deagle.SetBulletTexturePadding(2.0f);
    deagle.AddBulletDamage(5.0f);
    deagle.AddBulletSize(4.0f);
    deagle.SetBulletMaxAliveTime(0.75f);
    deagle.SetLightColor(255, 200, 0);


    deagle.AddBulletOnReachMaxAliveTimeCallback([](Bullet* b) 
    {
        auto& world = World::Instance();
        auto& generator = world.GetPlayer()->GetDeagle().GetGenerator();

        float bulletSize = generator.BulletSize;
        float bulletDamage = generator.BulletDamage;
        generator.BulletSize *= 0.5f;
        generator.BulletDamage *= 0.5f;

        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f( 1.0f,  0.0f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f(-1.0f,  0.0f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f( 0.0f,  1.0f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f( 0.0f, -1.0f), false);

        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f( 0.70710678118f,  0.70710678118f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f(-0.70710678118f,  0.70710678118f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f( 0.70710678118f, -0.70710678118f), false);
        world.CreateBullet(generator.BulletType, &generator, b->Position, sf::Vector2f(-0.70710678118f, -0.70710678118f), false);

        generator.BulletSize = bulletSize;
        generator.BulletDamage = bulletDamage;
    });

}

FaultyBombItem::FaultyBombItem(sf::Vector2f position) : BaseItem(position, "Bomba defectuosa", 44, ItemRarity::Uncommon) { }

void FaultyBombItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();

    deagle.AddBulletOnCollisionWithEnemyCallback([](Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.05f)
        {
            World::Instance().CreateExplosion(b->Position, 1500.0f, b->Damage * 15.0f, 0, CollisionType::EnemyCollisionType);
        }
    });
}

FrozenTunaItem::FrozenTunaItem(sf::Vector2f position) : BaseItem(position, "Atun congelado", 45, ItemRarity::Mythical) { }

void FrozenTunaItem::OnPickUp()
{
    auto& media = E2D::MediaContent::Instance();
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.SetBulletAnimationFrames(media.GetAnimationFrames("IceSpikeBulletAlive"), media.GetAnimationFrames("IceSpikeBulletDead"));
    deagle.SetBulletAnimationDurations(0.25f, 0.25f);
    deagle.SetBulletTexturePadding(12.0f);
    deagle.AddBulletDamage(5.0f);
    deagle.SetLightColor(0, 255, 255);

    deagle.AddBulletOnCollisionWithEnemyCallback([](Bullet* b, [[maybe_unused]] Enemy* e, [[maybe_unused]] const CollisionData& data) 
    {
        if(Random::GlobalInstance.NextFloat() <= 0.4f)
            World::Instance().CreateIceBlizzard(b->Position, b->Size * 10.0f, b->Damage * 0.5f, 0, 
                CollisionType::EnemyCollisionType);
    });
}

QuadShotItem::QuadShotItem(sf::Vector2f position) : BaseItem(position, "Disparo cuadruple", 46, ItemRarity::Lengendary) { }

void QuadShotItem::OnPickUp()
{
    DesertEagle& deagle = World::Instance().GetPlayer()->GetDeagle();
    deagle.AddBulletPointGenerator(QuadrupleBulletPointGenerator::GetGlobalInstance());
    deagle.RemoveAttackSpeed(0.1f);
    deagle.AddRecoil(50.0f);
}

HappyRockItem::HappyRockItem(sf::Vector2f position) : BaseItem(position, "Piedra feliz", 47, ItemRarity::Common) { }

void HappyRockItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->AddCurrentHealth(2);
    player->AddOnTakeDamageCallback([](Player* player, int32_t amount)
    {
        if (player->GetCurrentHealth() == 0 && Random::GlobalInstance.NextFloat() <= 0.05f)
        {
            player->AddDoubleJumps(5);
            player->AddMaxHealth(20);
            player->AddCurrentHealth(20);
            player->AddMoveSpeed(100.0f);
            player->AddJumpMaxTime(2.0f);
            player->AddInvulnerabilityTime(1.0f);
            player->RemoveDashCooldown(1.0f);

            auto& deagle = player->GetDeagle();
            deagle.AddAccuracy(1.0f);
            deagle.AddBulletSpeed(100.0f);
            deagle.AddBulletDamage(50.0f);
            deagle.AddAttackSpeed(0.5f);
            deagle.AddBulletKnockBack(100.0f);
            deagle.RemoveReloadTime(1.0f);
        }
    });
}

CursedHeartItem::CursedHeartItem(sf::Vector2f position) : BaseItem(position, "Corazon maldito", 48, ItemRarity::Lengendary) { }

void CursedHeartItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->RemoveMaxHealth(2);
    player->GetDeagle().RemoveBulletDamage(20.0f);
    player->AddOnTakeDamageCallback([](Player* player, int32_t amount) 
    {
        player->GetDeagle().AddBulletDamage(7.5f * amount);
    });
}

AstronautHelmetItem::AstronautHelmetItem(sf::Vector2f position) : BaseItem(position, "Casco de astronauta", 49, ItemRarity::Uncommon) { }

void AstronautHelmetItem::OnPickUp()
{
    Player* player = World::Instance().GetPlayer();
    player->SetBaseGravity(player->GetBaseGravity() * 0.5f);
}
