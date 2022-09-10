#include "Door.h"
#include "World.h"
#include "../SFMLUtils.hpp"
#include "../Engine2D/Renderer.h"
#include "../Engine2D/MediaContent.h"
#include "Player.h"
#include "../Curse.h"
#include <iostream>

Door::Door(sf::Vector2f position, int32_t id, DoorType type, const std::vector<Curse*>& curses) : 
    InteractiveEntity(position, ACTIVE_DISTANCE_SQUARED), doorId(id)
{
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Doors"));
    sprite.setPosition(position);
    sprite.setOrigin(WIDTH * 0.5f, HEIGHT * 0.5f);

    stateMachine.SetState(State::Opened, &Door::WhenOpened);
    stateMachine.SetState(State::Closed, &Door::WhenClosed);

    CreateOpenAnimation(type);
    CreateCursesSprites(curses);
    CreateSounds();
}

void Door::CreateOpenAnimation(DoorType type)
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    switch(type)
    {
        case DoorType::Default: openAnimation.SetFrames(media.GetAnimationFrames("DefaultDoorOpen")); 
            break;
        case DoorType::Shop: openAnimation.SetFrames(media.GetAnimationFrames("ShopDoorOpen")); 
            break;
        case DoorType::Treasure: openAnimation.SetFrames(media.GetAnimationFrames("TreasureDoorOpen")); 
            break;
        case DoorType::Boss: openAnimation.SetFrames(media.GetAnimationFrames("BossDoorOpen")); 
            break;
        case DoorType::NewLevel: openAnimation.SetFrames(media.GetAnimationFrames("NewLevelDoorOpen")); 
            break;
        default: break;
    }

    openAnimation.SetLoop(false);
    openAnimation.SetDuration(2.0f);
    openAnimation.Play(sprite);
}

void Door::CreateCursesSprites(const std::vector<Curse*>& curses)
{
    constexpr float ICON_SEPARATION = 10.0f;
    float iconsWidth = curses.size() * Curse::ICON_SIZE + 
        curses.size() * ICON_SEPARATION - ICON_SEPARATION; //Le restamos la ultima separacion ya que sobra
    sf::Vector2f iconStartPosition
    (
        this->Position.x - iconsWidth * 0.5f,
        this->Position.y - Door::HEIGHT * 0.5f - Curse::ICON_SIZE
    );

    for(auto c : curses)
    {
        E2D::Sprite& sprite = cursesSprites.emplace_back
        (
            *E2D::MediaContent::Instance().GetTexture("Curses"),
            c->GetTextureRect()
        );

        sprite.setPosition(iconStartPosition);
        sprite.setScale(Curse::ICON_SIZE / Curse::SPRITE_SIZE, Curse::ICON_SIZE / Curse::SPRITE_SIZE);
        sprite.SetColor(sf::Color(255 * 0.4f, 0, 255 * 0.4f));
        iconStartPosition.x += Curse::ICON_SIZE + ICON_SEPARATION;
    }
}

void Door::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& enter = sounds[SoundIndex::EnterSound];
    enter.SetBuffer(*media.GetSoundBuffer("DoorEnter"));
    enter.SetRelativeToListener(true); 
}

void Door::Update([[maybe_unused]] float dt)
{
    stateMachine.Update(this, dt);
}

void Door::WhenClosed([[maybe_unused]] float dt)
{
    if(world.GetEnemies().size() == 0)
    {
        stateMachine.ChangeState(State::Opened);
        for(auto& cs : cursesSprites)
            cs.SetColor(sf::Color::Magenta);
    }
}

void Door::WhenOpened(float dt)
{
    InteractiveEntity::Update(dt);
    openAnimation.Update(sprite, dt);
}

void Door::OnInteract()
{
    PlaySound(sounds[SoundIndex::EnterSound]);
    world.SetSelectedDoor(doorId);
}

void Door::Draw([[maybe_unused]] float tickPercent)
{
    for(auto& cs : cursesSprites)
        E2D::Renderer::Instance().Draw(cs);
    
    E2D::Renderer::Instance().Draw(sprite, renderStates);
}
