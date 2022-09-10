#include "Sound.h"

namespace E2D
{

Sound::Sound(const SoundBuffer& buffer) : sound(buffer.buffer) { }

void Sound::Play() { sound.play(); }

void Sound::Pause() { sound.pause(); }

void Sound::Stop() { sound.stop(); }

void Sound::SetRelativeToListener(bool relative) { sound.setRelativeToListener(relative); }

void Sound::SetPosition(const sf::Vector3f& position) { sound.setPosition(position); }

void Sound::SetBuffer(const SoundBuffer& buffer) {sound.setBuffer(buffer.buffer); }

void Sound::SetLoop(bool loop) { sound.setLoop(loop); }

void Sound::SetVolume(float volume) { sound.setVolume(volume); }

void Sound::SetAttenuation(float attenuation) { sound.setAttenuation(attenuation); }

void Sound::SetMinDIstance(float distance) { sound.setMinDistance(distance); }

void Sound::SetPlayingOffset(sf::Time timeOffset) { sound.setPlayingOffset(timeOffset); }

bool Sound::GetLoop() const { return sound.getLoop(); }

void Sound::SetPoisition(sf::Vector2f position) { sound.setPosition(position.x, position.y, 0.0f); }

void Sound::SetPosition(float x, float y) { sound.setPosition(x, y, 0.0f); }

sf::Time Sound::GetPlayingOffset() const { return sound.getPlayingOffset(); }

Sound::Status Sound::GetStatus() const { return static_cast<Sound::Status>(sound.getStatus()); }

void Sound::ResetBuffer() { sound.resetBuffer(); }

} // namespace E2D
