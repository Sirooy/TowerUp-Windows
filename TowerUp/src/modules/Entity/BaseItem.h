#pragma once

#include "InteractiveEntity.h"
#include "../SFMLUtils.hpp"

enum class ItemRarity : int32_t 
{
    Common = 0,
    Uncommon,
    Rare,
    Epic,
    Lengendary,
    Mythical
};

class BaseItem : public InteractiveEntity
{
public:
    static constexpr float ICON_SIZE = 64.0f;

    BaseItem(sf::Vector2f position, const std::string& name, int32_t id, ItemRarity rarity);

    void Update(float dt) override;
    void Draw(float dt) override;
    void OnInteract() override;
    const sf::IntRect& GetTextureRect();

    inline int32_t GetID() const { return id; }
    inline sf::Color GetRarityColor() const { return RARITY_COLORS[static_cast<int32_t>(rarity)]; }
    inline const std::string& GetName() const { return name; }
    void ResetInterpolatedPosition() override { LastPosition = Position; lastDecorationRotation = decorationRotation; }

    virtual void OnPickUp() = 0;

protected:
    inline static const sf::Color RARITY_COLORS[6] = 
    {
        sf::Color(200, 200, 200),
        sf::Color(0, 124, 212),
        sf::Color(255, 100, 0),
        sf::Color(255, 0, 0),
        sf::Color(255, 0, 255),
        sf::Color(0, 255, 255)
    };

    static constexpr float WAVE_SIZE = 4.0f;
    static constexpr float WAVE_SPEED = 4.0f;
    static constexpr float DECORATION_SPEED = sf::PI_OVER_4; //45 Grados/Segundo
    static constexpr float ACTIVE_DISTANCE_SQUARED = ICON_SIZE * ICON_SIZE;
    static constexpr int32_t TEXTURE_ICONS_PER_ROW = 10;
    static constexpr int32_t DECORATION_ID = 89;

    sf::IntRect textureRect;
    std::string name;
    int32_t id;
    ItemRarity rarity;
    E2D::Sound sound;

private:
    E2D::Sprite decorationSprite;

    float baseYPosition;
    float elapsedTime;
    float decorationRotation;
    float lastDecorationRotation;
};