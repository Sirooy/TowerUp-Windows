#include "HealthBar.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Entity/Player.h"

HealthBar::HealthBar() : hearts(), rects(nullptr), texture(nullptr), position(0.0f, 0.0f), heartWidth(0)
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    texture = media.GetTexture("Hud");
    rects = media.GetAnimationFrames("HudHealthBar");
    heartWidth = rects->front().width;
    height = rects->front().height;
}

void HealthBar::SetPosition(sf::Vector2f position)
{
    this->position = position;

    for(std::size_t i = 0;i < hearts.size(); ++i)
        hearts[i].setPosition(sf::Vector2f(position.x + i * heartWidth + i * HEART_SEPARATION, position.y));
}

sf::Vector2f HealthBar::GetPosition()
{
    return position;
}

float HealthBar::GetHeight()
{
    return height;
}

void HealthBar::SetMaxHealth(int32_t maxHealth)
{
    int32_t numHearts = maxHealth / 2;
    int32_t diff = numHearts - static_cast<int32_t>(hearts.size()); //Nuevos corazones menos los que tenia

    if(diff > 0) //Añadir corazones
    {
        //Calculamos la proxima posicion a partir del número de corazones que hay en el vector
        int32_t size = static_cast<int32_t>(hearts.size());
        sf::Vector2f nextPosition = sf::Vector2f(position.x + size * heartWidth + 
            (size) * HEART_SEPARATION, position.y); 

        while(diff > 0)
        {
            E2D::Sprite heart;
            heart.setPosition(nextPosition);
            heart.SetTexture(*texture);
            heart.SetTextureRect((*rects)[HeartRectsIndices::Empty]);
            hearts.push_back(heart);
            nextPosition.x += heartWidth + HEART_SEPARATION;
            --diff;
        }
    }
    else if(diff < 0) //Borrar corazones
    {
        while(diff < 0)
        {
            hearts.pop_back();
            ++diff;
        }
    }
}

void HealthBar::SetCurrentHealth(int32_t currentHealth)
{
    int32_t numFullHearts = currentHealth / 2;
    bool hasHalfHeart = currentHealth % 2 != 0;

    //Cambiamos el estado de todos los corazones
    int32_t size = static_cast<int32_t>(hearts.size());
    for(int32_t i = 0;i < size; ++i)
    {
        if(i < numFullHearts)
            hearts[i].SetTextureRect((*rects)[HeartRectsIndices::Full]);
        else
            hearts[i].SetTextureRect((*rects)[HeartRectsIndices::Empty]);
    }

    if(hasHalfHeart && numFullHearts < size)
        hearts[numFullHearts].SetTextureRect((*rects)[HeartRectsIndices::Half]);
}

void HealthBar::Draw(E2D::Renderer& renderer)
{ 
    for(auto& h : hearts)
    {
        renderer.Draw(h);
    }
}