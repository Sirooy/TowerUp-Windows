#pragma once

#include "../DynamicEntity.h"
#include "../../Engine2D/Sprite.h"
#include "../../Engine2D/Sound.h"
#include "../EntityTypeEnums.h"
#include <array>


class Player;
class PhysicsEngine;
class BulletGenerator;

class Enemy : public DynamicEntity
{
public:
    static constexpr float DROP_BASE_CHANGE = 0.3f;
    static constexpr float DROP_INCREASE_PER_CURSE = 0.3f;

    Enemy(sf::Vector2f position, float size, float health, int32_t attack);
    Enemy(sf::Vector2f position, float width, float height, float health, int32_t attack);

    virtual void Draw(float tickPercent) override;
    virtual void OnCollision(CollidableEntity* other, const CollisionData& data) override;
    virtual void OnCollision(Bullet* bullet, const CollisionData& data) override;
    virtual void OnCollision(HurtZone* hurtZone, const CollisionData& data) override;
    virtual void TakeDamage(float amount);

    inline float GetHealth() { return health; }
    inline int32_t GetAttack() { return attack; }
    inline float   GetKnockback() { return knockback; }
    inline E2D::Sprite& GetSprite() { return sprite; }

    void SetHealth(float health) { this->health = health; }
    void SetAttack(int32_t attack) { this->attack = attack; }
    void SetKnockback(float knockback) { this->knockback = knockback; }
    void Kill();

protected:
    static constexpr float DROP_FORCE = 500.0f;
    static constexpr std::array<int32_t, 5> DROP_RATIO = 
    {
        0, 0, 0, 0, 1
    }; //80% Moneda, 20% Corazon

    static constexpr std::array<ConsumableType, 4> HEART_DROP_RATIO = 
    {
        ConsumableType::RedHeart, ConsumableType::RedHeart, ConsumableType::RedHeart,
        ConsumableType::GoldenHeart
    }; // 75% RedHeart, 25% GoldenHeart 

    static constexpr std::array<ConsumableType, 10> COIN_DROP_RATIO = 
    {
        ConsumableType::BronceCoin, ConsumableType::BronceCoin, ConsumableType::BronceCoin, ConsumableType::BronceCoin,
        ConsumableType::SilverCoin, ConsumableType::SilverCoin, ConsumableType::SilverCoin,
        ConsumableType::GoldCoin,   ConsumableType::GoldCoin,   ConsumableType::GoldCoin,
    }; //40% Bronce, 30% Silver, 20% Gold

    E2D::Sprite sprite;
    PhysicsEngine& physics;
    Player& player;
    float health;
    int32_t attack;
    float knockback;
    float armor;
    float knockbackResistance;

    void TryDropConsumable();
    bool IsCloseToPlayer(float detectionDistSquared);
    bool HasVisionOfPlayer(float detectionDistSquared);
    bool HasVisionOfPlayer(float detectionDistSquared, sf::Vector2f viewDir);
    bool IsNearWall(float direction, float distance);
    bool IsNearEdge(float halfHeight, float direction, float distance);
    sf::Vector2f GetRandomMapPosition();
    void SetSpriteOrientation(float dir);
    void SetSpriteOrientation(sf::Vector2f viewDir);
    void SetBulletGeneratorBaseData(BulletGenerator* generator);
};
