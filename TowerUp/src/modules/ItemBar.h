#pragma once

#include "Engine2D/Sprite.h"
#include "Engine2D/Text.h"
#include "StateMachine.h"
#include "SFMLUtils.hpp"
#include <vector>

class BaseItem;

class ItemBar
{ 
public:
    ItemBar();

    void AddItem(BaseItem* item);
    void Update(float dt);
    void Draw(E2D::Renderer& renderer, float tickPercent);

    void SetPosition(sf::Vector2f position);
    sf::Vector2f GetPosition();

protected:
    static constexpr int32_t ITEMS_PER_ROW = 4;
    static constexpr float ITEM_SIZE = 32.0f;
    static constexpr float ITEM_SEPARATION = 8.0f;
    static constexpr int32_t ITEM_TITLE_CHARACTER_SIZE = 60;
    static constexpr int32_t ITEM_TITLE_OUTLINE_THICKNESS = 2;

    class ItemTitle
    {
    public:
        static constexpr float TITLE_AND_NAME_SEPARATION = 24.0f;

        ItemTitle(BaseItem* item, E2D::Font* font);
        void Update(float dt);
        void Draw(E2D::Renderer& renderer, float tickPercent);
        inline bool CanBeDeleted() { return canBeDeleted; }

        inline static E2D::Text TitleText {};

    protected:
        static constexpr float FADE_TIME = 0.25f;
        static constexpr float STAY_TIME = 1.5f;
        static constexpr float WAVE_SIZE = 16.0f;
        static constexpr float WAVE_SPEED = sf::PI_2 * 2.0f; //2 oscilaciones/segundo

        enum State
        {
            Appearing = 0,
            Staying,
            Dissapearing
        };

        E2D::Text nameText;
        StateMachine<ItemTitle, 3> stateMachine {};
        float baseYPosition { 0.0f };
        float positionY { 0.0f };
        float lastPositionY { 0.0f };
        float currentStayTime { 0.0f };
        bool canBeDeleted { false };

        void UpdateAppearing(float dt);
        void UpdateStaying(float dt);
        void UpdateDissapearing(float dt);
    };

    std::vector<E2D::Sprite> items;
    std::vector<ItemTitle> itemTitles;
    E2D::Texture* texture;
    E2D::Font* font;
    sf::Vector2f position;

public:
    static constexpr float MAX_WIDTH = ITEMS_PER_ROW * ITEM_SIZE + ITEMS_PER_ROW * ITEM_SEPARATION;
};

