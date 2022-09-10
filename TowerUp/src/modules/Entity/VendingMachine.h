#pragma once

#include "InteractiveEntity.h"
#include "../Random.h"
#include "../StateMachine.h"
#include "EntityTypeEnums.h"
#include "../Engine2D/Sound.h"

class Player;

class VendingMachine : public InteractiveEntity
{
public:
    VendingMachine(sf::Vector2f position, uint64_t seed, float activeDistanceSquared, int32_t spriteRectID);

    void Update(float dt) override;
    void Draw(float tickPercent) override;
    void OnInteract() override;
    void ResetInterpolatedPosition() override { lastScaleFactor = scaleFactor; }

protected:
    static constexpr float USAGE_SCALE_FACTOR = 0.08f;
    static constexpr float USAGE_SCALE_SPEED = 20.0f;
    static constexpr float USAGE_SPEED = 2.0f;
    static constexpr float WIDTH = 100.0f;
    static constexpr float HEIGHT = 132.0f;

    enum State
    {
        Default = 0,
        UsedWithResources,
        UsedWithoutResources
    };

    enum SoundIndex
    {
        CoinInSound = 0,
        SuccessSound,
        ErrorSound,
        WorkingSound
    };

    Player& player;
    StateMachine<VendingMachine, 3> stateMachine;
    Random random;
    sf::Vector2f scaleFactor;
    sf::Vector2f lastScaleFactor;
    float elapsedTime;
    E2D::Sound sounds[4];

    virtual bool PlayerHasEnoughResources() = 0;
    virtual void RemoveResources() = 0;
    virtual void OnSuccessfulUse() = 0;

    void DefaultState(float dt);
    void UsedWithResourcesState(float dt);
    void UsedWithoutResourcesState(float dt);
    void CreateSounds();
};

class ItemVendingMachine : public VendingMachine
{
public:
    ItemVendingMachine(sf::Vector2f position, uint64_t seed);

protected:
    static constexpr float ACTIVE_DISTANCE_SQUARED = VendingMachine::WIDTH * 0.5f * VendingMachine::WIDTH * 0.5f;
    static constexpr int32_t MIN_ITEMS = 1;
    static constexpr int32_t MAX_ITEMS = 3;
    static constexpr int32_t PRICE = 150.0f;

    sf::Vector2f itemPosition;
    int32_t itemsLeft;

    bool PlayerHasEnoughResources() override;
    void RemoveResources() override;
    void OnSuccessfulUse() override;
};

class HeartVendingMachine : public VendingMachine
{
public:
    HeartVendingMachine(sf::Vector2f position, uint64_t seed);

protected:
    static constexpr float ACTIVE_DISTANCE_SQUARED = VendingMachine::WIDTH * 0.5f * VendingMachine::WIDTH * 0.5f;
    static constexpr int32_t PRICE = 25.0f;
    static constexpr std::size_t PROBABILITIES_SIZE = 5;
    static constexpr ConsumableType PROBABILITIES[PROBABILITIES_SIZE] = 
    { 
        ConsumableType::RedHeart, ConsumableType::RedHeart, ConsumableType::RedHeart, 
        ConsumableType::GoldenHeart, ConsumableType::GoldenHeart
    }; //Red = 60%, Gold = 40%

    bool PlayerHasEnoughResources() override;
    void RemoveResources() override;
    void OnSuccessfulUse() override;
};

class CoinVendingMachine : public VendingMachine
{
public:
    CoinVendingMachine(sf::Vector2f position, uint64_t seed);

protected:
    static constexpr float ACTIVE_DISTANCE_SQUARED = VendingMachine::WIDTH * 0.5f * VendingMachine::WIDTH * 0.5f;
    static constexpr int32_t PRICE = 1;
    static constexpr std::size_t PROBABILITIES_SIZE = 10;
    static constexpr ConsumableType PROBABILITIES[PROBABILITIES_SIZE] = 
    { 
        ConsumableType::BronceCoin, ConsumableType::BronceCoin, ConsumableType::BronceCoin, ConsumableType::BronceCoin, ConsumableType::BronceCoin,
        ConsumableType::SilverCoin, ConsumableType::SilverCoin, ConsumableType::SilverCoin,
        ConsumableType::GoldCoin,   ConsumableType::GoldCoin
    }; //Bronce = 50%, Silver = 30%, Gold = 20%

    bool PlayerHasEnoughResources() override;
    void RemoveResources() override;
    void OnSuccessfulUse() override;
};