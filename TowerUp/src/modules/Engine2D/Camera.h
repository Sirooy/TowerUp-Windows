#pragma once
#include "SFML/Graphics/View.hpp"

namespace E2D
{

class Camera
{

public:
    Camera() = default;
    explicit Camera(const sf::FloatRect& rectangle);
    Camera(const sf::Vector2f& center, const sf::Vector2f& size);

    void SetCenter(sf::Vector2f center);
    void SetCenterWithoutClamp(sf::Vector2f center);
    void SetSize(float width, float height);
    void SetSize(const sf::Vector2f& size);
    void SetRotation(float angle);
    void SetViewport(const sf::FloatRect& viewport);
    void Reset(const sf::FloatRect& rectangle);
    void Reset();
    const sf::Vector2f& GetCenter() const;
    const sf::Vector2f& GetSize() const;
    float GetRotation() const;
    const sf::FloatRect& GetViewport() const;
    const sf::View GetView() const;
    void Move(float offsetX, float offsetY);
    void Move(const sf::Vector2f& offset);
    void Rotate(float angle);
    void Zoom(float factor);
    const sf::Transform& GetTransform() const;
    const sf::Transform& GetInverseTransform() const;
    void SetLimits(sf::Vector2f min, sf::Vector2f max);

private:
    friend class Renderer;

    sf::View view {};
    sf::Vector2f minLimit { 0.0f, 0.0f };
    sf::Vector2f maxLimit { 0.0f, 0.0f };
    sf::Vector2f minCenter { 0.0f, 0.0f };
    sf::Vector2f maxCenter { 0.0f, 0.0f };
    float zoom { 1.0f };
};

}