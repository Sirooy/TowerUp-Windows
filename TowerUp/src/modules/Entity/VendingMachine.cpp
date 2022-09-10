#include "VendingMachine.h"
#include "../Engine2D/MediaContent.h"
#include "../Engine2D/Renderer.h"
#include "../SFMLUtils.hpp"
#include "Consumable.h"
#include "World.h"
#include "Player.h"
#include <cmath>

#include <iostream> //BORRAR

//BASE VENDING MACHINE
VendingMachine::VendingMachine(sf::Vector2f position, uint64_t seed, float activeDistanceSquared, int32_t spriteRectID) : 
    InteractiveEntity(position, activeDistanceSquared), player(*World::Instance().GetPlayer()),
    stateMachine(), random(seed), scaleFactor(1.0f, 1.0f), lastScaleFactor(1.0f, 1.0f), 
    elapsedTime(0.0f)
{ 
    stateMachine.SetState(State::UsedWithoutResources, &VendingMachine::UsedWithoutResourcesState);
    stateMachine.SetState(State::UsedWithResources, &VendingMachine::UsedWithResourcesState);
    stateMachine.SetState(State::Default, &VendingMachine::DefaultState);

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("VendingMachine"));
    sprite.SetTextureRect(sf::IntRect(WIDTH * spriteRectID, 0, WIDTH, HEIGHT));
    sprite.setOrigin(WIDTH * 0.5f, HEIGHT * 0.5f);
    CreateSounds();
}

void VendingMachine::Update(float dt)
{
    stateMachine.Update(this, dt);
}

void VendingMachine::Draw(float tickPercent)
{
    sf::Vector2f scale = sf::Vector2f
    (
        lastScaleFactor.x * (1.0f - tickPercent) + scaleFactor.x * tickPercent,
        lastScaleFactor.y * (1.0f - tickPercent) + scaleFactor.y * tickPercent
    );

    sprite.setScale(scale);

    E2D::Renderer::Instance().Draw(sprite, renderStates);
}

void VendingMachine::OnInteract()
{
    elapsedTime = 0.0f;
    renderStates.shader = nullptr;

    if(PlayerHasEnoughResources())
    {
        RemoveResources();
        PlaySound(sounds[SoundIndex::CoinInSound]);//No estoy seguro de que vaya aqui
        PlaySound(sounds[SoundIndex::WorkingSound]);//No estoy seguro de que vaya aqui
        stateMachine.ChangeState(State::UsedWithResources);
    }
    else
    {
        PlaySound(sounds[SoundIndex::ErrorSound]);//No estoy seguro de que vaya aqui
        stateMachine.ChangeState(State::UsedWithoutResources);
    }
        
}

void VendingMachine::DefaultState(float dt)
{
    InteractiveEntity::Update(dt);
}

void VendingMachine::UsedWithResourcesState(float dt)
{
    lastScaleFactor = scaleFactor;
    elapsedTime += dt;

    if(elapsedTime >= USAGE_SPEED)
    {
        scaleFactor = sf::Vector2f(1.0f, 1.0f);
        lastScaleFactor = sf::Vector2f(1.0f, 1.0f);

        PlaySound(sounds[SoundIndex::SuccessSound]);//No estoy seguro de que vaya aqui
        OnSuccessfulUse();
        stateMachine.ChangeState(State::Default);
    }
    else
    {
        float scaleX = std::cos(elapsedTime * USAGE_SCALE_SPEED) * USAGE_SCALE_FACTOR;
        float scaleY = std::sin(elapsedTime * USAGE_SCALE_SPEED) * USAGE_SCALE_FACTOR;
        scaleFactor = sf::Vector2f(1.0f + scaleX, 1.0f + scaleY);
    }
}

void VendingMachine::UsedWithoutResourcesState(float dt)
{
    lastScaleFactor = scaleFactor;
    elapsedTime += dt;
    
    if(elapsedTime >= USAGE_SPEED * 0.5f)
    {
        scaleFactor = sf::Vector2f(1.0f, 1.0f);
        lastScaleFactor = sf::Vector2f(1.0f, 1.0f);

        stateMachine.ChangeState(State::Default);
    }
    else
    {
        float scaleX = std::cos(elapsedTime * USAGE_SCALE_SPEED) * USAGE_SCALE_FACTOR;
        float scaleY = std::sin(elapsedTime * USAGE_SCALE_SPEED) * USAGE_SCALE_FACTOR;
        scaleFactor = sf::Vector2f(1.0f + scaleX, 1.0f + scaleY);
    }
}

void VendingMachine::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& coinIn = sounds[SoundIndex::CoinInSound];
    coinIn.SetBuffer(*media.GetSoundBuffer("VendingMachineCoinIn"));
    coinIn.SetRelativeToListener(true);
    coinIn.SetVolume(35.0f);

    auto& succeed = sounds[SoundIndex::SuccessSound];
    succeed.SetBuffer(*media.GetSoundBuffer("VendingMachineSuccess"));
    succeed.SetRelativeToListener(false);
    succeed.SetAttenuation(0.001f);
    succeed.SetVolume(35.0f);

    auto& error = sounds[SoundIndex::ErrorSound];
    error.SetBuffer(*media.GetSoundBuffer("VendingMachineError"));
    error.SetRelativeToListener(true);
    error.SetVolume(35.0f);

    auto& working = sounds[SoundIndex::WorkingSound];
    working.SetBuffer(*media.GetSoundBuffer("VendingMachineWorking"));
    working.SetRelativeToListener(false);
    working.SetAttenuation(10.0f);
    working.SetVolume(35.0f);
}

//ITEM VENDING MACHINE
ItemVendingMachine::ItemVendingMachine(sf::Vector2f position, uint64_t seed) : 
    VendingMachine(position, seed, ItemVendingMachine::ACTIVE_DISTANCE_SQUARED, 0), itemPosition(), itemsLeft(0.0f)
{ 
    itemsLeft = random.RangeInt(MIN_ITEMS, MAX_ITEMS);
    itemPosition = position + sf::Vector2f(WIDTH, HEIGHT * 0.25f);
}

bool ItemVendingMachine::PlayerHasEnoughResources()
{
    return itemsLeft > 0 && player.GetMoney() >= ItemVendingMachine::PRICE;        
}

void ItemVendingMachine::RemoveResources()
{
    player.RemoveMoney(ItemVendingMachine::PRICE);
}

void ItemVendingMachine::OnSuccessfulUse()
{
    --itemsLeft;

    ItemType type = static_cast<ItemType>(random.NextInt(static_cast<int32_t>(ItemType::MaxItemTypes)));
    world.CreateItem(type, itemPosition);
    itemPosition.x += WIDTH;
}

//HEART VENDING MACHINE
HeartVendingMachine::HeartVendingMachine(sf::Vector2f position, uint64_t seed) : 
    VendingMachine(position, seed, HeartVendingMachine::ACTIVE_DISTANCE_SQUARED, 3)
{ }

bool HeartVendingMachine::PlayerHasEnoughResources()
{
    return player.GetMoney() >= HeartVendingMachine::PRICE;       
}

void HeartVendingMachine::RemoveResources()
{
    player.RemoveMoney(HeartVendingMachine::PRICE);
}

void HeartVendingMachine::OnSuccessfulUse()
{
    int32_t index = random.NextInt(HeartVendingMachine::PROBABILITIES_SIZE);
    Consumable* c = World::Instance().CreateConsumable(PROBABILITIES[index],  Position);

    //Lanzar el corazon en una dirección aleatoria
    float throwAngle = random.NextFloat(sf::PI_OVER_4, sf::PI - sf::PI_OVER_4);
    sf::Vector2f direction = sf::Vector2f(std::cos(throwAngle), -std::sin(throwAngle));
    c->Velocity = direction * 500.0f;
}

//COIN VENDING MACHINE
CoinVendingMachine::CoinVendingMachine(sf::Vector2f position, uint64_t seed) : 
    VendingMachine(position, seed, CoinVendingMachine::ACTIVE_DISTANCE_SQUARED, 4)
{ }

bool CoinVendingMachine::PlayerHasEnoughResources()
{
    return player.GetCurrentHealth() > CoinVendingMachine::PRICE && !player.IsInvulnerable();      
}

void CoinVendingMachine::RemoveResources()
{
    player.TakeDamage(CoinVendingMachine::PRICE, sf::Vector2f(0.0f, -1.0f), 200.0f);
}

void CoinVendingMachine::OnSuccessfulUse()
{
    int32_t index = random.NextInt(CoinVendingMachine::PROBABILITIES_SIZE);
    Consumable* c = World::Instance().CreateConsumable(PROBABILITIES[index],  Position);

    //Lanzar la moneda en una dirección aleatoria
    float throwAngle = random.NextFloat(sf::PI_OVER_4, sf::PI - sf::PI_OVER_4);
    sf::Vector2f direction = sf::Vector2f(std::cos(throwAngle), -std::sin(throwAngle));
    c->Velocity = direction * 500.0f;
}