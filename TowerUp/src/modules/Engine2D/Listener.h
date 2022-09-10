#pragma once

#include "SFML/Audio/Listener.hpp"
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
namespace E2D
{

class Listener
{
public:

    static void SetGlobalVolume(float volume);
    static float GetGlobalVolume();
    static void SetPosition(const sf::Vector2f& position);
    static void SetPosition(const sf::Vector3f& position);
    static sf::Vector3f GetPosition();
    static void SetDirection(const sf::Vector3f& direction);
    static sf::Vector3f GetDirection();
    static void SetUpVector(const sf::Vector3f& upVector);
    static sf::Vector3f GetUpVector();
};

} // namespace E2D