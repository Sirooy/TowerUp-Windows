#include "Sprite.h"

namespace E2D
{

Sprite::Sprite() :
m_texture    (nullptr),
m_textureRect(),
horizontalFlip(1.0f),
verticalFlip(1.0f)
{
}

Sprite::Sprite(const E2D::Texture& texture) :
m_texture    (nullptr),
m_textureRect(),
horizontalFlip(1.0f),
verticalFlip(1.0f)
{
    SetTexture(texture, true);
}

Sprite::Sprite(const E2D::Texture& texture, const sf::IntRect& rectangle) :
m_texture    (nullptr),
m_textureRect(),
horizontalFlip(1.0f),
verticalFlip(1.0f)
{
    // Compute the texture area
    SetTextureRect(rectangle);
    // Assign texture
    SetTexture(texture, false);
}

void Sprite::SetTexture(const E2D::Texture& texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no valid texture & rect before
    if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
    {
        SetTextureRect(sf::IntRect({0, 0}, sf::Vector2i(texture.GetSize())));
    }

    // Assign the new texture
    m_texture = &texture.texture;
}

void Sprite::SetTextureRect(const sf::IntRect& rectangle)
{
    if (rectangle != m_textureRect)
    {
        m_textureRect = rectangle;
        UpdatePositions();
        UpdateTexCoords();
    }
}

void Sprite::SetColor(const sf::Color& color)
{
    // Update the vertices' color
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}

void Sprite::SetFlipHorizontally(bool flip)
{
    if(flip)
    {
        if(horizontalFlip != -1.0f)
        {
            horizontalFlip = -1.0f;
            sf::Vector2f scale = getScale();
            scale.x = std::abs(scale.x);
            scale.y = std::abs(scale.y);
            SetScale(scale);
        }
    }
    else
    {
        if(horizontalFlip != 1.0f)
        {
            horizontalFlip = 1.0f;
            sf::Vector2f scale = getScale();
            scale.x = std::abs(scale.x);
            scale.y = std::abs(scale.y);
            SetScale(scale);
        }
    }
}

void Sprite::SetFlipVertically(bool flip)
{
    if(flip)
    {
        if(verticalFlip != -1.0f)
        {
            verticalFlip = -1.0f;
            sf::Vector2f scale = getScale();
            scale.x = std::abs(scale.x);
            scale.y = std::abs(scale.y);
            SetScale(scale);
        }
    }
    else
    {
        if(verticalFlip != 1.0f)
        {
            verticalFlip = 1.0f;
            sf::Vector2f scale = getScale();
            scale.x = std::abs(scale.x);
            scale.y = std::abs(scale.y);
            SetScale(scale);
        }
    }
}

void Sprite::SetScale(float scaleX, float scaleY)
{
    scaleX *= horizontalFlip;
    scaleY *= verticalFlip;

    Transformable::setScale(scaleX, scaleY);
}

void Sprite::SetScale(const sf::Vector2f& factor)
{
    sf::Vector2f copy = factor;
    copy.x *= horizontalFlip;
    copy.y *= verticalFlip;

    Transformable::setScale(copy);
}

void Sprite::SetAlpha(uint8_t alpha)
{
    m_vertices[0].color.a = alpha;
    m_vertices[1].color.a = alpha;
    m_vertices[2].color.a = alpha;
    m_vertices[3].color.a = alpha;
}

void Sprite::InvertAlpha()
{
    uint8_t alpha = 255 - m_vertices[0].color.a;

    m_vertices[0].color.a = alpha;
    m_vertices[1].color.a = alpha;
    m_vertices[2].color.a = alpha;
    m_vertices[3].color.a = alpha;
}

bool Sprite::FadeOut(float time, float dt)
{
    float percent = dt / time;
    uint8_t amount = static_cast<uint8_t>(percent * 255.0f);
    uint8_t alpha = m_vertices[0].color.a;
    
    if(amount >= alpha)
    {   
        SetAlpha(0);
        return true;
    }
    else
        SetAlpha(alpha - amount);

    return false;
}

bool Sprite::FadeIn(float time, float dt)
{
    float percent = dt / time;
    uint8_t amount = static_cast<uint8_t>(percent * 255.0f);
    uint8_t alpha = m_vertices[0].color.a;
    
    if(amount + alpha >= 255)
    {
        SetAlpha(255);
        return true;
    }
    else
        SetAlpha(amount + alpha);

    return false;
}

const sf::Texture* Sprite::GetTexture() const
{
    return m_texture;
}

const sf::IntRect& Sprite::GetTextureRect() const
{
    return m_textureRect;
}

const sf::Color& Sprite::GetColor() const
{
    return m_vertices[0].color;
}

uint8_t Sprite::GetAlpha() const
{
    return m_vertices[0].color.a;
}

sf::FloatRect Sprite::GetLocalBounds() const
{
    auto width = static_cast<float>(std::abs(m_textureRect.width));
    auto height = static_cast<float>(std::abs(m_textureRect.height));

    return sf::FloatRect({0.f, 0.f}, {width, height});
}

sf::FloatRect Sprite::GetGlobalBounds() const
{
    return getTransform().transformRect(GetLocalBounds());
}

void Sprite::UpdatePositions()
{
    sf::FloatRect bounds = GetLocalBounds();

    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(0, bounds.height);
    m_vertices[2].position = sf::Vector2f(bounds.width, 0);
    m_vertices[3].position = sf::Vector2f(bounds.width, bounds.height);
}

void Sprite::UpdateTexCoords()
{
    sf::FloatRect convertedTextureRect(m_textureRect);

    float left   = convertedTextureRect.left;
    float right  = left + convertedTextureRect.width;
    float top    = convertedTextureRect.top;
    float bottom = top + convertedTextureRect.height;

    m_vertices[0].texCoords = sf::Vector2f(left, top);
    m_vertices[1].texCoords = sf::Vector2f(left, bottom);
    m_vertices[2].texCoords = sf::Vector2f(right, top);
    m_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

} // namespace E2D
