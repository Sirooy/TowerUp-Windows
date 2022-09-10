#include "Button.h"
#include "Engine2D/Renderer.h"

Button::Button() : renderer(E2D::Renderer::Instance()), sprite(), text(), defaultRect(1,1,1,1), hoverRect(1,1,1,1),
    width(1.0f), height(1.0f), isClicked(false), mouseIsOver(false) 
{
    sprite.SetTextureRect(sf::IntRect());
}

void Button::SetDefaultRect(const sf::IntRect& defaultRect)
{
    this->defaultRect = defaultRect;
    sprite.SetTextureRect(defaultRect);

    RecalculateButtonSize();
}

void Button::SetHoverRect(const sf::IntRect& hoverRect)
{
    this->hoverRect = hoverRect;

    RecalculateButtonSize();
}

void Button::SetSize(float width, float height)
{
    this->width = width;
    this->height = height;
    
    RecalculateButtonSize();
}

void Button::RecalculateButtonSize()
{
    float xFactor = width / defaultRect.width;
    float yFactor = height / defaultRect.height;
    sprite.setOrigin(defaultRect.width * 0.5f, defaultRect.height * 0.5f);
    sprite.setScale(xFactor, yFactor);
}

void Button::SetText(const std::string& string)
{
    text.SetString(string);

    CenterText();
}

void Button::CenterText()
{
    sf::FloatRect bounds = text.GetLocalBounds();

    text.SetOrigin(bounds.width * 0.5f, text.GetCharacterSize() * 0.75f);
}

void Button::SetTextStyle(sf::Text::Style style)
{
    text.SetStyle(style);

    CenterText();
}


void Button::SetTextCharacterSize(uint32_t size)
{
    text.SetCharacterSize(size);

    CenterText();
}

void Button::SetTextColor(const sf::Color& color)
{
    text.SetFillColor(color);
    text.SetOutlineColor(color);
}

void Button::SetTexture(E2D::Texture* texture)
{
    sprite.SetTexture(*texture);
}

void Button::SetTextFont(E2D::Font* font)
{
    text.SetFont(*font);
    
    CenterText();
}

void Button::SetPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
    text.SetPosition(pos);
}

void Button::SetPosition(float x, float y)
{
    sprite.setPosition(x, y);
    text.SetPosition(x, y);
}


void Button::Update(const sf::Event& event)
{
    sf::Vector2i mousePos = renderer.GetMousePosition();
    sf::Vector2f position = sprite.getPosition();
    float hw = width * 0.5f;
    float hh = height * 0.5f;

    //El raton esta dentro
    if(mousePos.x > position.x - hw && mousePos.x < position.x + hw &&
       mousePos.y > position.y - hh && mousePos.y < position.y + hh)
    {
        mouseIsOver = true;
        isClicked = event.type == sf::Event::MouseButtonPressed &&  event.mouseButton.button == sf::Mouse::Button::Left;

        sprite.SetTextureRect(hoverRect);
    }
    else
    {
        mouseIsOver = false;
        isClicked = false;

        sprite.SetTextureRect(defaultRect);
    }
}

void Button::Draw()
{
    renderer.Draw(sprite);
    renderer.Draw(text);
}