#include "SoundBuffer.h"

#include <filesystem>
#include <istream>

namespace E2D
{

bool SoundBuffer::LoadFromFile(const std::string& path) { return buffer.loadFromFile(path); }

const int16_t* SoundBuffer::GetSamples() const { return buffer.getSamples(); }

uint64_t SoundBuffer::GetSampleCount() const { return buffer.getSampleCount(); }

unsigned int SoundBuffer::GetSampleRate() const { return buffer.getSampleRate(); }

unsigned int SoundBuffer::GetChannelCount() const { return buffer.getChannelCount(); }

sf::Time SoundBuffer::GetDuration() const { return buffer.getDuration(); }
    
} // namespace E2D


