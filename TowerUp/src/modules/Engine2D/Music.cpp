#include "Music.h"

namespace E2D
{

bool Music::OpenFromFile(const std::string& filename) { return music.openFromFile(filename.c_str()); }

void Music::Play() { music.play(); }

void Music::Pause() { music.pause(); }

void Music::Stop() { music.stop(); }

float Music::GetMinDistance() const { return music.getMinDistance(); }

void Music::SetMinDistance(float minDistance) { music.setMinDistance(minDistance); }

sf::Vector3f Music::GetPosition() const { return music.getPosition(); }

void Music::SetPosition(const sf::Vector3f& position) { music.setPosition(position); }

float Music::GetAttenuation() const { return music.getAttenuation(); }

void Music::SetAttenuation(float attenuation) { music.setAttenuation(attenuation); }

float Music::GetPitch() const { return music.getPitch(); }

void Music::SetPitch(float pitch) { music.setPitch(pitch); }

float Music::GetVolume() const { return music.getVolume(); }

void Music::SetVolume(float volume) { music.setVolume(volume); }

unsigned int Music::GetChannelCount() const { return music.getChannelCount(); }

unsigned int Music::GetSampleRate() const { return music.getSampleRate(); }

Music::Status Music::GetStatus() const { return static_cast<Music::Status>(music.getStatus()); }

void Music::SetPlayingOffset(sf::Time timeOffset) { music.setPlayingOffset(timeOffset); }

sf::Time Music::GetPlayingOffset() const { return music.getPlayingOffset(); }

void Music::SetLoop(bool loop) { music.setLoop(loop); }

bool Music::GetLoop() const { return music.getLoop(); }

sf::Time Music::GetDuration() const { return music.getDuration(); }

sf::Music::TimeSpan Music::GetLoopPoints() const { return music.getLoopPoints(); }

void Music::SetLoopPoints(sf::Music::TimeSpan timePoints) {  music.setLoopPoints(timePoints); }

} // namespace E2D