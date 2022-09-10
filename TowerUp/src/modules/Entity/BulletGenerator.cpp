#include "BulletGenerator.h"
#include "World.h"
#include "../Random.h"


//! BASE GENERATOR

BulletGenerator::BulletGenerator() : world(World::Instance()) { }

BulletGenerator::BulletGenerator(std::initializer_list<BulletPointGenerator*> list) : world(World::Instance())
{
    PointGenerators = std::vector<BulletPointGenerator*>(list);    
}

void BulletGenerator::GenerateBullet(sf::Vector2f position, sf::Vector2f direction, bool executeCallbacks)
{
    int32_t inputIndex = 0;
    int32_t outputIndex = 1;
    auto input = &points[inputIndex];
    auto output = &points[outputIndex];
    input->clear();
    output->clear();

    input->emplace_back(position, direction);
    for (std::size_t i = 0; i < PointGenerators.size(); ++i)
    {
        PointGenerators[i]->GeneratePoints(*input, *output);
        input->clear();

        //Swap indices
        inputIndex ^= 1; 
        outputIndex ^= 1;
        
        input = &points[inputIndex];
        output = &points[outputIndex];
    }

    //Swap indices
    inputIndex ^= 1;
    outputIndex ^= 1;

    output = &points[outputIndex];
    for (std::size_t i = 0; i < output->size(); ++i)
    {
        auto& point = (*output)[i];
        world.CreateBullet(BulletType, this, point.Position, point.Direction, executeCallbacks);
    }
}

/*
void BulletGenerator::Copy(BulletGenerator* other)
{
    this->BulletType = other->BulletType;
    this->BulletDamage = other->BulletDamage;
    this->BulletPlayerDamage = other->BulletPlayerDamage;
    this->BulletFriction = other->BulletFriction;
    this->BulletSize = other->BulletSize;
    this->BulletSpeed = other->BulletSpeed;
    this->BulletKnockBack = other->BulletKnockBack;
    this->BulletMaxAliveTime = other->BulletMaxAliveTime; 
    this->BulletBaseAcceleration = other->BulletBaseAcceleration;
    this->BulletTexture = other->BulletTexture;
    this->BulletTexturePadding = other->BulletTexturePadding;
    this->BulletColor = other->BulletColor;
    this->BulletAnimationFrames[0] = other->BulletAnimationFrames[0];
    this->BulletAnimationFrames[1] = other->BulletAnimationFrames[1];
    this->BulletAnimationDurations[0] = other->BulletAnimationDurations[0];
    this->BulletAnimationDurations[1] = other->BulletAnimationDurations[1];
    this->BulletCollisionType = other->BulletCollisionType;
    this->BulletCollisionMask = other->BulletCollisionMask;
    this->OnCollisionWithEnemyCallbacks = other->OnCollisionWithEnemyCallbacks;
    this->OnCollisionWithTileCallbacks = other->OnCollisionWithTileCallbacks;
    this->OnReachMaxAliveTimeCallbacks = other->OnReachMaxAliveTimeCallbacks;
}
*/

//! DOUBLE BULLET GENERATOR
DoubleBulletPointGenerator DoubleBulletPointGenerator::GlobalInstance {};

void DoubleBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        sf::Vector2f normal = sf::cross(point.Direction);
        output.emplace_back(point.Position + normal * SEPARATION, point.Direction);
        output.emplace_back(point.Position - normal * SEPARATION, point.Direction);
    }
}

//! ANGLED DOUBLE BULLET GENERATOR
AngledDoubleBulletPointGenerator AngledDoubleBulletPointGenerator::GlobalInstance{};

void AngledDoubleBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        output.emplace_back(point.Position, sf::rotate(point.Direction, -ARC_ANGLE));
        output.emplace_back(point.Position, sf::rotate(point.Direction,  ARC_ANGLE));
    }
}


//! TRIPLE BULLET GENERATOR
TripleBulletPointGenerator TripleBulletPointGenerator::GlobalInstance{};

void TripleBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        sf::Vector2f normal = sf::cross(point.Direction);
        output.emplace_back(point.Position + normal, sf::rotate(point.Direction, -ARC_ANGLE));
        output.emplace_back(point.Position + normal, point.Direction);
        output.emplace_back(point.Position - normal, sf::rotate(point.Direction, ARC_ANGLE));
    }
}

//! SHOTGUN BULLET GENERATOR
ShotGunBulletPointGenerator ShotGunBulletPointGenerator::GlobalInstance{};

void ShotGunBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    int32_t pellets = Random::GlobalInstance.RangeInt(MIN_PELLETS, MAX_PELLETS);
    
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        for (int32_t i = 0; i < pellets; ++i)
        {
            float angle = Random::GlobalInstance.NextFloat(MAX_ARC_ANGLE);
            float angleDir = (Random::GlobalInstance.RangeInt(0, 1) == 1.0) ? 1.0f : -1.0f;
            output.emplace_back(point.Position, sf::rotate(point.Direction, angle * angleDir));
        }
    }

    /*
    float bulletSpeed = this->BulletSpeed;
    float bulletSize = this->BulletSize;
    for(int32_t i = 0;i < pellets; ++i)
    {
        float angle = Random::GlobalInstance.NextFloat(MAX_ARC_ANGLE);
        float angleDir = (Random::GlobalInstance.RangeInt(0, 1) == 1.0) ? 1.0f : -1.0f;
        sf::Vector2f rotatedDir = sf::rotate(direction, angle * angleDir);
        BulletSpeed = bulletSpeed * Random::GlobalInstance.NextFloat(MIN_BULLET_SPEED_PERCENT, MAX_BULLET_SPEED_PERCENT);
        BulletSize = bulletSize * Random::GlobalInstance.NextFloat(MIN_BULLET_SIZE_PERCENT, MAX_BULLET_SIZE_PERCENT);
        world.CreateBullet(BulletType, this, position, rotatedDir, executeCallbacks);
    }
    this->BulletSpeed = bulletSpeed;
    this->BulletSize = bulletSize;
    */
}

//! CROSS BULLET GENERATOR
CrossBulletPointGenerator CrossBulletPointGenerator::GlobalInstance{};

void CrossBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        sf::Vector2f normal = sf::cross(point.Direction);
        output.emplace_back(point.Position,  point.Direction);
        output.emplace_back(point.Position,  normal);
        output.emplace_back(point.Position, -point.Direction);
        output.emplace_back(point.Position, -normal);
    }
}

//! QUADRUPLE BULLET GENERATOR
QuadrupleBulletPointGenerator QuadrupleBulletPointGenerator::GlobalInstance{};

void QuadrupleBulletPointGenerator::GeneratePoints(std::vector<BulletPoint>& input, std::vector<BulletPoint>& output)
{
    float iaCos = std::cos(INNER_ARC_ANGLE);
    float iaSin = std::sin(INNER_ARC_ANGLE);
    float miaCos = std::cos(-INNER_ARC_ANGLE);
    float miaSin = std::sin(-INNER_ARC_ANGLE);
    float oaCos = std::cos(OUTER_ARC_ANGLE);
    float oaSin = std::sin(OUTER_ARC_ANGLE);
    float moaCos = std::cos(-OUTER_ARC_ANGLE);
    float moaSin = std::sin(-OUTER_ARC_ANGLE);

    for (std::size_t i = 0; i < input.size(); ++i)
    {
        auto& point = input[i];

        output.emplace_back(point.Position, sf::rotate(point.Direction, miaCos, miaSin));
        output.emplace_back(point.Position, sf::rotate(point.Direction, iaCos, iaSin));
        output.emplace_back(point.Position, sf::rotate(point.Direction, moaCos, moaSin));
        output.emplace_back(point.Position, sf::rotate(point.Direction, oaCos, oaSin));
    }
}
