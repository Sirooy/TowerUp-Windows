#pragma once

#include <filesystem>
#include "SFML/Audio/Music.hpp"

namespace E2D
{
    
class Music
{
public:
    enum Status
    {
        Paused = sf::Music::Status::Paused,
        Playing = sf::Music::Status::Paused,
        Stopped = sf::Music::Status::Stopped
    };

    bool OpenFromFile(const std::string& filename);
    void Play();
    void Pause();
    void Stop();

    float GetMinDistance() const;
    void SetMinDistance(float minDistance); 
    sf::Vector3f GetPosition() const;
    void SetPosition(const sf::Vector3f& position);
    float GetAttenuation() const;
    void SetAttenuation(float attenuation);
    float GetPitch() const;
    void SetPitch(float pitch);
    float GetVolume() const;
    void SetVolume(float volume);
    unsigned int GetChannelCount() const;
    unsigned int GetSampleRate() const;
    Status GetStatus() const;
    void SetPlayingOffset(sf::Time timeOffset);
    sf::Time GetPlayingOffset() const;
    void SetLoop(bool loop);
    bool GetLoop() const;
    sf::Time GetDuration() const;
    sf::Music::TimeSpan GetLoopPoints() const;
    void SetLoopPoints(sf::Music::TimeSpan timePoints);

private:
    sf::Music music;
};
    
} // namespace E2D