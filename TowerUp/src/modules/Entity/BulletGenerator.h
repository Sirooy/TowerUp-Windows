#pragma once

#include "SFML/System/Vector2.hpp"
#include "../Engine2D/Texture.h"
#include "../SFMLUtils.hpp"
#include "EntityTypeEnums.h"
#include <cstdint>
#include <vector>
#include <functional>

class World;
class Bullet;
class Enemy;
struct CollisionData;
struct TileCollisionData;

struct BulletPoint
{
    sf::Vector2f Position{};
    sf::Vector2f Direction{};

    BulletPoint(sf::Vector2f position, sf::Vector2f direction) : Position(position), Direction(direction)  { }
};

class BulletPointGenerator
{
public:
    virtual void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) = 0;
};

class BulletGenerator
{
public:
    BulletGenerator();
    BulletGenerator(std::initializer_list<BulletPointGenerator*> list);

    World& world;
    
    BulletType BulletType { BulletType::Default };
    float BulletDamage { 0.0f };
    int32_t BulletPlayerDamage { 0 };
    float BulletFriction { 1.0f };
    float BulletSize { 0.0f }; //Radio
    float BulletSpeed { 0.0f };
    float BulletKnockBack { 0.0f };
    float BulletMaxAliveTime { 0.0f };
    float BulletTexturePadding { 0.0f };
    sf::Vector2f BulletBaseAcceleration {0.0f, 0.0f};
    E2D::Texture* BulletTexture { nullptr };
    sf::Color BulletColor { sf::Color::White };
    std::vector<sf::IntRect>* BulletAnimationFrames[2] { };
    float BulletAnimationDurations[2] { };
    uint32_t BulletCollisionType { 0 };
    uint32_t BulletCollisionMask { 0 };
    std::vector<BulletPointGenerator*> PointGenerators { };
    std::vector<std::function<void(Bullet*, Enemy*, const CollisionData&)>> OnCollisionWithEnemyCallbacks { };
    std::vector<std::function<void(Bullet*, const TileCollisionData&)>> OnCollisionWithTileCallbacks { };
    std::vector<std::function<void(Bullet*)>> OnReachMaxAliveTimeCallbacks { };
    //Sonido de choque y generacion

    void GenerateBullet(sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks = true);
protected:
    std::vector<BulletPoint> points[2] { };
};

class DoubleBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static DoubleBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance;  }
protected:
    static constexpr float SEPARATION = 8.0f;

    static DoubleBulletPointGenerator GlobalInstance;
};

class AngledDoubleBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static AngledDoubleBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance; }
protected:
    static constexpr float ARC_ANGLE = 45.0_ToRadians;

    static AngledDoubleBulletPointGenerator GlobalInstance;
};

class TripleBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static TripleBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance; }
protected:
    static constexpr float ARC_ANGLE = 30.0_ToRadians;

    static TripleBulletPointGenerator GlobalInstance;
};

class ShotGunBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static ShotGunBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance; }
protected:
    static constexpr int32_t MAX_PELLETS = 6;
    static constexpr int32_t MIN_PELLETS = 3;
    static constexpr float MAX_ARC_ANGLE = 10.0_ToRadians;
    static constexpr float MIN_BULLET_SPEED_PERCENT = 0.8f;
    static constexpr float MAX_BULLET_SPEED_PERCENT = 1.0f;
    static constexpr float MIN_BULLET_SIZE_PERCENT = 0.8f;
    static constexpr float MAX_BULLET_SIZE_PERCENT = 1.2f;

    static ShotGunBulletPointGenerator GlobalInstance;
};

class CrossBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static CrossBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance; }
protected:
    static CrossBulletPointGenerator GlobalInstance;
};

class QuadrupleBulletPointGenerator : public BulletPointGenerator
{
public:
    void GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output) override;

    static QuadrupleBulletPointGenerator* GetGlobalInstance() { return &GlobalInstance; }
protected:
    static constexpr float INNER_ARC_ANGLE = 10.0_ToRadians;
    static constexpr float OUTER_ARC_ANGLE = 30.0_ToRadians;
    static QuadrupleBulletPointGenerator GlobalInstance;
};