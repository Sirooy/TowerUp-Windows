#pragma once
#include "SFML/Audio/SoundBuffer.hpp"

namespace E2D
{

class SoundBuffer
{
public:
    friend class Sound;

    bool LoadFromFile(const std::string& path);
    const int16_t* GetSamples() const;
    uint64_t GetSampleCount() const;
    unsigned int GetSampleRate() const;
    unsigned int GetChannelCount() const;
    sf::Time GetDuration() const;

private:
    sf::SoundBuffer buffer;
};

} // namespace E2D