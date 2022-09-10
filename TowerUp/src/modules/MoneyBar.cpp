#include "MoneyBar.h"
#include "Entity/Player.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"

MoneyBar::MoneyBar() : coinSprite(), moneyText(), 
    position(0.0f, 0.0f), money(0), height(0.0f)
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    coinSprite.SetTexture(*media.GetTexture("Hud"));

    coinAnimation.SetFrames(media.GetAnimationFrames("HudCoin"));
    coinAnimation.SetDuration(1.0f);
    coinAnimation.Play(coinSprite);

    height = coinAnimation.GetFrame(0).height;

    moneyText.SetFont(*media.GetFont("Font"));
    moneyText.SetFillColor(sf::Color::Yellow);
    moneyText.SetString("0");
    moneyText.SetCharacterSize(height - height * 4.0f / 16.0f);
}

void MoneyBar::SetSize(float size)
{
    float factor = size / height;
    height = size;
    
    coinSprite.setScale(factor, factor);
    moneyText.SetCharacterSize(height - height * 4.0f / 16.0f);
    moneyText.SetPosition(sf::Vector2f(position.x + size + TEXT_SEPARATION, position.y));
}

void MoneyBar::SetPosition(sf::Vector2f position)
{
    this->position = position;

    coinSprite.setPosition(position);
    moneyText.SetPosition(sf::Vector2f(position.x + height + TEXT_SEPARATION, position.y)); //Le sumas la altura a x por que la moneda es cuadrada
}

sf::Vector2f MoneyBar::GetPosition()
{
    return position;
}

float MoneyBar::GetHeight()
{
    return height;
}

void MoneyBar::SetMoney(int32_t money)
{
    this->money = money; 

    moneyText.SetString(std::to_string(money));
}

void MoneyBar::Update(float dt)
{
    coinAnimation.Update(coinSprite, dt);
}

void MoneyBar::Draw(E2D::Renderer& renderer)
{
    renderer.Draw(coinSprite);
    renderer.Draw(moneyText);
}