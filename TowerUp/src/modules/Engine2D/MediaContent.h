#pragma once

#include <vector>
#include <unordered_map>
#include "SFML/Graphics/Shader.hpp"
#include "Texture.h"
#include "SoundBuffer.h"
#include "Music.h"
#include "Font.h"
#include "../tinyxml2.h"


namespace E2D
{

class Sprite;

class MediaContent
{
public:
    
    bool LoadTexture(const std::string& key, const std::string& path);
    E2D::Texture* GetTexture(const std::string& key);

    bool LoadSoundBuffer(const std::string& key, const std::string& path);
    E2D::SoundBuffer* GetSoundBuffer(const std::string& key);

    bool LoadMusic(const std::string& key, const std::string& path);
    E2D::Music* GetMusic(const std::string& key);

    bool LoadFont(const std::string& key, const std::string& path);
    E2D::Font* GetFont(const std::string& key);

    bool LoadShader(const std::string& key, const std::string& fragment);
    bool LoadShader(const std::string& key, const std::string& vertex, const std::string& fragment);
    bool LoadShader(const std::string& key, const std::string& vertex, const std::string& geometry, const std::string& fragment);
    sf::Shader* GetShader(const std::string& key);

    bool LoadAnimationFrames(const std::string& path);
    std::vector<sf::IntRect>* GetAnimationFrames(const std::string& key);

    void UnloadResources();

    inline static MediaContent& Instance() { return SingletonInstance; }

    ~MediaContent() = default;
private:
    std::unordered_map<std::string, std::unique_ptr<E2D::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<E2D::SoundBuffer>> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<E2D::Music>> songs;
    std::unordered_map<std::string, std::unique_ptr<E2D::Font>> fonts; 
    std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaders;
    std::unordered_map<std::string, std::unique_ptr<std::vector<sf::IntRect>>> animationFrames;

    MediaContent() = default;

    std::vector<sf::IntRect>* ParseAnimationFrames(tinyxml2::XMLElement* animation);
    void ParseFramesRepeatX(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
        const char* att, int32_t x, int32_t y, int32_t width, int32_t height);
    void ParseFramesRepeatY(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
        const char* att, int32_t x, int32_t y, int32_t width, int32_t height);
    void ParseFramesRepeatXY(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
        const char* attX, const char* attY, int32_t x, int32_t y, int32_t width, int32_t height);

    static MediaContent SingletonInstance;
};

} // namespace E2D