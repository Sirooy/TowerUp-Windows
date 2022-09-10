#include "Texture.h"

namespace E2D
{

bool Texture::Create(unsigned int width, unsigned int height) { return texture.create(width, height); }

bool Texture::LoadFromFile(const std::string& filename, const sf::IntRect& area) { return texture.loadFromFile(filename, area); }

sf::Vector2u Texture::GetSize() const { return texture.getSize(); }

void Texture::SetSmooth(bool smooth) { texture.setSmooth(smooth); }

bool Texture::IsSmooth() const { return texture.isSmooth(); }

void Texture::SetSrgb(bool sRgb) { texture.setSrgb(sRgb); }

bool Texture::IsSrgb() const { return texture.isSrgb(); }

void Texture::SetRepeated(bool repeated) { texture.setRepeated(repeated); }

bool Texture::IsRepeated() const { return texture.isRepeated(); }

unsigned int Texture::GetMaximumSize() { return sf::Texture::getMaximumSize(); }

} // namespace E2D