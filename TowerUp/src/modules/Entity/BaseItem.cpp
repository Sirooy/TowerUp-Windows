#include "BaseItem.h"

#include "Player.h"
#include "World.h"
#include "../Hud.h"
#include "../Engine2D/MediaContent.h"
#include "../Engine2D/Renderer.h"

#include <cmath>

BaseItem::BaseItem(sf::Vector2f position, const std::string& name, int32_t id, ItemRarity rarity) : 
    InteractiveEntity(position, ACTIVE_DISTANCE_SQUARED), textureRect(), name(name), id(id), 
    rarity(rarity), sound(), decorationSprite(), baseYPosition(0.0f), elapsedTime(0.0f), decorationRotation(0.0f),
    lastDecorationRotation(0.0f)
{
    textureRect = sf::IntRect
    (
        sf::Vector2i( id % TEXTURE_ICONS_PER_ROW * ICON_SIZE, id / TEXTURE_ICONS_PER_ROW * ICON_SIZE ),
        sf::Vector2i(ICON_SIZE, ICON_SIZE)
    );

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Items")); 
    sprite.setOrigin(sf::Vector2f(ICON_SIZE * 0.5f, ICON_SIZE * 0.5f));
    sprite.SetTextureRect(textureRect);

    decorationSprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Items"));
    decorationSprite.setOrigin(sf::Vector2f(ICON_SIZE * 0.5f, ICON_SIZE * 0.5f));
    decorationSprite.SetTextureRect
    (
        sf::IntRect
        (
            sf::Vector2i(DECORATION_ID % TEXTURE_ICONS_PER_ROW * ICON_SIZE, DECORATION_ID / TEXTURE_ICONS_PER_ROW * ICON_SIZE ),
            sf::Vector2i(ICON_SIZE, ICON_SIZE)
        )
    );
    decorationSprite.SetColor(GetRarityColor());
    decorationSprite.setScale(2.0f, 2.0f);
    decorationSprite.SetAlpha(127);

    baseYPosition = position.y;
    sound.SetBuffer(*E2D::MediaContent::Instance().GetSoundBuffer("ItemPickUp"));  
}

const sf::IntRect& BaseItem::GetTextureRect()
{
    return textureRect;
}

void BaseItem::Update(float dt)
{
    InteractiveEntity::Update(dt);

    elapsedTime += dt;
    LastPosition.y = Position.y;
    Position.y = baseYPosition + std::sin(elapsedTime * WAVE_SPEED) * WAVE_SIZE;

    lastDecorationRotation = decorationRotation;
    decorationRotation += sf::degrees(dt * DECORATION_SPEED);
}

void BaseItem::Draw(float tickPercent)
{
    sf::Vector2f position = GetInterpolatedPosition(tickPercent);
    float rotation = lastDecorationRotation * (1.0f - tickPercent) + decorationRotation * tickPercent;
    decorationSprite.setRotation(rotation);

    E2D::Renderer::Instance().Draw(position, decorationSprite);
    E2D::Renderer::Instance().Draw(position, sprite, renderStates);
}

void BaseItem::OnInteract()
{
    PlaySound(sound);
    World::Instance().GetPlayer()->AddItemID(this->id);
    World::Instance().GetHud()->GetItemBar().AddItem(this);
    OnPickUp();
    IsDead = true;
}
