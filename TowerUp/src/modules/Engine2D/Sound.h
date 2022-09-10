#pragma once
#include <SFML/System/Vector3.hpp>
#include "SoundBuffer.h"
#include "SFML/Audio/Sound.hpp"
#include "Listener.h"
#include "SFML/System/Vector2.hpp"

namespace E2D
{

class Sound
{
public:
    enum Status
    {
        Paused = sf::Sound::Status::Paused,
        Playing = sf::Sound::Status::Playing,
        Stopped = sf::Sound::Status::Stopped
    };

    Sound() = default;
    explicit Sound(const SoundBuffer& buffer);

    void Play();
    void Pause();
    void Stop();
    void SetRelativeToListener(bool relative);
    void SetPosition(const sf::Vector3f& position);
    void SetBuffer(const SoundBuffer& buffer);
    void SetLoop(bool loop);
    void SetVolume(float volume);
    void SetAttenuation(float attenuation);
    void SetMinDIstance(float distance); 
    void SetPlayingOffset(sf::Time timeOffset);
    void SetPoisition(sf::Vector2f position);
    void SetPosition(float x, float y);
    bool GetLoop() const;
    sf::Time GetPlayingOffset() const;
    Status GetStatus() const;
    void ResetBuffer();

private:
    sf::Sound sound;
};

} // namespace E2D