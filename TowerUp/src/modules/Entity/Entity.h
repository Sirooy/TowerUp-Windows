#pragma once

#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include "../Engine2D/Sound.h"

//TODO: Metodos para saber si está destruida
class Entity
{
public:
    sf::Vector2f Position;
    sf::Vector2f LastPosition;
    const uint32_t ID;
    bool IsDead;

    Entity(sf::Vector2f position = sf::Vector2f{0.0f, 0.0f});
    inline sf::Vector2f GetInterpolatedPosition(float tickPercent) { return  LastPosition *  (1.0f - tickPercent) + Position * tickPercent; }
    virtual inline void ResetInterpolatedPosition() { LastPosition = Position; }
    inline void PlaySound(E2D::Sound& sound)
    {
        sound.SetPosition(sf::Vector3f(Position.x, Position.y, 0.0));
        sound.Play();
    }

    virtual ~Entity() = default;
protected:
    
private:
    static uint32_t CurrentID;
};