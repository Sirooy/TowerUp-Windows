#include "Listener.h"

namespace E2D
{

    void Listener::SetGlobalVolume(float volume){ sf::Listener::setGlobalVolume(volume); }

    float Listener::GetGlobalVolume(){ return sf::Listener::getGlobalVolume(); }
    
    void Listener::SetPosition(const sf::Vector2f& position){ sf::Listener::setPosition(position.x, position.y, 0.0f); } 
    
    void Listener::SetPosition(const sf::Vector3f& position){ sf::Listener::setPosition(position); } 

    sf::Vector3f Listener::GetPosition(){ return sf::Listener::getPosition(); }

    void Listener::SetDirection(const sf::Vector3f& direction){ sf::Listener::setDirection(direction); }

    sf::Vector3f Listener::GetDirection(){ return sf::Listener::getDirection(); }

    void Listener::SetUpVector(const sf::Vector3f& upVector){ sf::Listener::setUpVector(upVector); }

    sf::Vector3f Listener::GetUpVector(){ return sf::Listener::getUpVector(); }

} // namespace E2D
