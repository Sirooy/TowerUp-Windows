#include "ItemBar.h"
#include <vector>
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Entity/BaseItem.h"

ItemBar::ItemBar() : items(), texture(), position(0.0f, 0.0f)
{    
    auto& media = E2D::MediaContent::Instance();
    texture = media.GetTexture("Items");
    font = media.GetFont("Font");

    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

    auto& titleText = ItemTitle::TitleText;
    titleText.SetFont(*font);
    titleText.SetString("Has conseguido");
    titleText.SetFillColor(sf::Color(255, 255, 255, 0));
    titleText.SetOutlineColor(sf::Color(0, 0, 0, 0));
    titleText.SetOutlineThickness(ITEM_TITLE_OUTLINE_THICKNESS);
    titleText.SetCharacterSize(ITEM_TITLE_CHARACTER_SIZE);

    sf::FloatRect bounds = titleText.GetGlobalBounds();
    titleText.SetOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    titleText.SetPosition(screenSize.x * 0.5f, bounds.height * 0.5f + screenSize.y * 0.2f);
}

void ItemBar::SetPosition(sf::Vector2f position)
{
    this->position = position;

    int32_t totalIcons = static_cast<int32_t>(items.size());
    for(int32_t i = 0; i < totalIcons; ++i)
    {
        items[i].setPosition(sf::Vector2f
        (
            position.x + (i % ITEMS_PER_ROW) * ITEM_SIZE + (i % ITEMS_PER_ROW) * ITEM_SEPARATION,
            position.y + (i / ITEMS_PER_ROW) * ITEM_SIZE
        ));
    }
}

sf::Vector2f ItemBar::GetPosition()
{
    return position;
}

void ItemBar::AddItem(BaseItem* item)
{
    int32_t totalIcons = static_cast<int32_t>(items.size());

    E2D::Sprite itemIcon;
    itemIcon.SetTexture(*texture);
    itemIcon.SetTextureRect(item->GetTextureRect());
    itemIcon.setPosition(sf::Vector2f
    (
        position.x + (totalIcons % ITEMS_PER_ROW) * ITEM_SIZE + (totalIcons % ITEMS_PER_ROW) * ITEM_SEPARATION,
        position.y + (totalIcons / ITEMS_PER_ROW) * ITEM_SIZE + (totalIcons / ITEMS_PER_ROW) * ITEM_SEPARATION
    ));
    itemIcon.setScale(sf::Vector2f //Establecemos el tamaño de los iconos basandonos en el tamaño
    (
        ITEM_SIZE / BaseItem::ICON_SIZE,
        ITEM_SIZE / BaseItem::ICON_SIZE
    ));
    itemIcon.SetAlpha(128);

    items.push_back(itemIcon);
    itemTitles.emplace_back(item, font);
}

void ItemBar::Update(float dt)
{
    if(itemTitles.size() > 0)
    {
        ItemTitle& title = itemTitles[0];

        title.Update(dt);

        if(title.CanBeDeleted())
        {
            itemTitles.erase(itemTitles.begin());
            ItemTitle::TitleText.SetAlpha(0);
        }
    }
}

void ItemBar::Draw(E2D::Renderer& renderer, float tickPercent)
{
    for (auto& i : items)
        renderer.Draw(i);
    
    if(itemTitles.size() > 0)
        itemTitles[0].Draw(renderer, tickPercent);
}

ItemBar::ItemTitle::ItemTitle(BaseItem* item, E2D::Font* font)
{
    nameText.SetFont(*font);
    nameText.SetString(item->GetName());
    nameText.SetFillColor(item->GetRarityColor());
    nameText.SetOutlineColor(sf::Color::White);
    nameText.SetOutlineThickness(ITEM_TITLE_OUTLINE_THICKNESS);
    nameText.SetCharacterSize(ITEM_TITLE_CHARACTER_SIZE);
    nameText.SetAlpha(0);

    sf::FloatRect nameBounds = nameText.GetGlobalBounds();
    sf::FloatRect titleBounds = TitleText.GetGlobalBounds();
    nameText.SetOrigin(nameBounds.width * 0.5f, nameBounds.height * 0.5f);

    sf::Vector2f position = TitleText.GetPosition();
    position.y += titleBounds.height * 0.5f + nameBounds.height * 0.5f + TITLE_AND_NAME_SEPARATION;
    positionY = lastPositionY = baseYPosition = position.y;
    nameText.SetPosition(position);

    stateMachine.SetState(State::Dissapearing, &ItemTitle::UpdateDissapearing);
    stateMachine.SetState(State::Staying,      &ItemTitle::UpdateStaying);
    stateMachine.SetState(State::Appearing,    &ItemTitle::UpdateAppearing);
}

void ItemBar::ItemTitle::Update(float dt)
{
    stateMachine.Update(this, dt);
}

void ItemBar::ItemTitle::UpdateAppearing(float dt)
{
    bool result = nameText.FadeIn(FADE_TIME, dt);
    TitleText.FadeIn(FADE_TIME, dt);

    if(result)
        stateMachine.ChangeState(State::Staying);
}

void ItemBar::ItemTitle::UpdateStaying(float dt)
{
    currentStayTime += dt;

    lastPositionY = positionY;
    positionY = baseYPosition + std::sin(currentStayTime * WAVE_SPEED) * WAVE_SIZE;

    if(currentStayTime >= STAY_TIME)
    {
        lastPositionY = positionY;
        stateMachine.ChangeState(State::Dissapearing);
    }
}

void ItemBar::ItemTitle::UpdateDissapearing(float dt)
{
    canBeDeleted = nameText.FadeOut(FADE_TIME, dt);
    TitleText.FadeOut(FADE_TIME, dt);
}

void ItemBar::ItemTitle::Draw(E2D::Renderer& renderer, float tickPercent)
{
    sf::Vector2f position = nameText.GetPosition();
    position.y = lastPositionY * (1.0f - tickPercent) + positionY * tickPercent;
    nameText.SetPosition(position);

    renderer.Draw(TitleText);
    renderer.Draw(nameText);
}