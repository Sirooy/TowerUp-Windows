#include "Camera.h"
#include "../SFMLUtils.hpp"

namespace E2D
{

Camera::Camera(const sf::FloatRect& rectangle) : view(rectangle) { }
Camera::Camera(const sf::Vector2f& center, const sf::Vector2f& size) : view(center, size) { }

void Camera::SetCenter(sf::Vector2f center) 
{ 
    center = sf::clamp(center, minCenter, maxCenter);

    view.setCenter(center); 
}

void Camera::SetCenterWithoutClamp(sf::Vector2f center)
{
    view.setCenter(center);
}

void Camera::SetSize(float width, float height) 
{ 
    view.setSize(width, height);
}

void Camera::SetSize(const sf::Vector2f& size) 
{ 
    view.setSize(size); 
}

void Camera::SetRotation(float angle) 
{ 
    view.setRotation(angle);
}

void Camera::SetViewport(const sf::FloatRect& viewport) 
{ 
    view.setViewport(viewport); 
}

void Camera::Reset(const sf::FloatRect& rectangle) 
{ 
    view.reset(rectangle);
}

const sf::Vector2f& Camera::GetCenter() const 
{ 
    return view.getCenter(); 
}

const sf::Vector2f& Camera::GetSize() const 
{ 
    return view.getSize(); 
}

float Camera::GetRotation() const 
{ 
    return view.getRotation(); 
}

const sf::FloatRect& Camera::GetViewport() const 
{ 
    return view.getViewport(); 
}

const sf::View Camera::GetView() const
{
    return view;
}

void Camera::Move(float offsetX, float offsetY) 
{ 
    view.move(offsetX, offsetY); 
}

void Camera::Move(const sf::Vector2f& offset) 
{ 
    view.move(offset); 
}

void Camera::Rotate(float angle) 
{ 
    view.rotate(angle); 
}

void Camera::Zoom(float factor) 
{ 
    zoom = factor;

    view.zoom(factor); 
}

const sf::Transform& Camera::GetTransform() const 
{ 
    return view.getTransform(); 
}

const sf::Transform& Camera::GetInverseTransform() const 
{ 
    return view.getInverseTransform(); 
}

void Camera::SetLimits(sf::Vector2f min, sf::Vector2f max)
{
    minLimit = min;
    maxLimit = max;
    
    minCenter = min;
    maxCenter = max;

    if(minCenter.x > maxCenter.x)
        maxCenter.x = minCenter.x;
    if(minCenter.y > maxCenter.y)
        maxCenter.y = minCenter.y;
}

}
