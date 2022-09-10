#pragma once

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "Texture.h"


namespace E2D
{

class Renderer;

//Copia-pega del codigo fuente de SFML (https://github.com/SFML/SFML/blob/32ad019304ae01731809140abec9f7d068e2fb37/src/SFML/Graphics/Sprite.cpp)
class Sprite : public sf::Transformable
{
public:
    friend class Renderer;

    Sprite();
    explicit Sprite(const E2D::Texture& texture);
    Sprite(const E2D::Texture& texture, const sf::IntRect& rectangle);
    void SetTexture(const E2D::Texture& texture, bool resetRect = false);
    void SetTextureRect(const sf::IntRect& rectangle);
    void SetColor(const sf::Color& color);
    void SetFlipHorizontally(bool flip);
    void SetFlipVertically(bool flip);
    void SetScale(float scaleX, float scaleY);
    void SetScale(const sf::Vector2f& factor);
    void SetAlpha(uint8_t alpha);
    void InvertAlpha();
    bool FadeOut(float time, float dt);
    bool FadeIn(float time, float dt);
    const sf::Texture* GetTexture() const;
    const sf::IntRect& GetTextureRect() const;
    const sf::Color& GetColor() const;
    uint8_t GetAlpha() const;
    sf::FloatRect GetLocalBounds() const;
    sf::FloatRect GetGlobalBounds() const;
    inline bool GetFlipHorizontally() { return horizontalFlip == -1.0f; }
    inline bool GetFlipVertically() { return verticalFlip == -1.0f; }
    inline bool IsFadedOut() const { return m_vertices[0].color.a == 0; }
    inline bool IsFadedIn() const { return m_vertices[0].color.a == 255; }

private:
    sf::Vertex         m_vertices[4];
    const sf::Texture* m_texture;    
    sf::IntRect        m_textureRect;
    float horizontalFlip;
    float verticalFlip;

    void UpdatePositions();
    void UpdateTexCoords();
};

} // namespace E2D