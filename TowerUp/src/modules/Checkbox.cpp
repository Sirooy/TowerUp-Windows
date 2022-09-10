#include "Checkbox.h"
#include "Engine2D/Renderer.h"

Checkbox::Checkbox() : renderer(E2D::Renderer::Instance()), sprite(), text(), defaultRect(1,1,1,1), activeRect(1,1,1,1),
    width(1.0f), height(1.0f), isClicked(false), mouseIsOver(false), isActive(false)
{

}

void Checkbox::SetDefaultRect(const sf::IntRect& defaultRect)
{
    this->defaultRect = defaultRect;
    sprite.SetTextureRect(defaultRect);

    RecalculateCheckboxSize();
}


void Checkbox::SetActiveRect(const sf::IntRect& activeRect)
{
    this->activeRect = activeRect;

    RecalculateCheckboxSize();
}

void Checkbox::SetSize(float width, float height)
{
    this->width = width;
    this->height = height;
    
    RecalculateCheckboxSize();
}

void Checkbox::RecalculateCheckboxSize()
{
    float xFactor = width / defaultRect.width;
    float yFactor = height / defaultRect.height;
    sprite.setScale(xFactor, yFactor);
    sprite.setOrigin(defaultRect.width * 0.5f, defaultRect.height * 0.5f);
}

void Checkbox::SetText(const std::string& string)
{
    text.SetString(string);

    CenterText();
}

void Checkbox::CenterText()
{
    sf::FloatRect bounds = text.GetGlobalBounds();

    text.SetOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

void Checkbox::SetTextStyle(sf::Text::Style style)
{
    text.SetStyle(style);

    CenterText();
}


void Checkbox::SetTextCharacterSize(uint32_t size)
{
    text.SetCharacterSize(size);

    CenterText();
}

void Checkbox::SetTextColor(const sf::Color& color)
{
    text.SetFillColor(color);
    text.SetOutlineColor(color);
}

void Checkbox::SetTexture(E2D::Texture* texture)
{
    sprite.SetTexture(*texture);
}

void Checkbox::SetTextFont(E2D::Font* font)
{
    text.SetFont(*font);
}

void Checkbox::SetPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

void Checkbox::SetPositionText(sf::Vector2f pos)
{
    text.SetPosition(pos);
}


void Checkbox::SetPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void Checkbox::SetPositionText(float x, float y)
{
    text.SetPosition(x, y);
}

void Checkbox::Update(const sf::Event& event)
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
        
        if(isClicked==true)
        {
            if(isActive==false)
            {
                isActive=true;
                sprite.SetTextureRect(activeRect);
            }
            else
            {
                isActive=false;
                sprite.SetTextureRect(defaultRect);
            }
        }

    }
    else
    {
        mouseIsOver = false;
        isClicked = false;

        if(isActive==true)
        {
            sprite.SetTextureRect(activeRect);
        }
        else
        {
            sprite.SetTextureRect(defaultRect);
        }
    
    }


}

void Checkbox::Draw()
{
    renderer.Draw(sprite);
    renderer.Draw(text);
}