#pragma once

#include "SFML/Graphics.hpp"
#include "TileMapLayer.h"
#include "Text.h"
#include "Camera.h"
#include "Filter.h"
#include <unordered_map>

class RogueKnight;

namespace E2D
{

class Sprite;

class Renderer
{
public:

    bool IsOpen() const;
    bool PollEvent(sf::Event& event);
    void SetCamera(const Camera& camera);
    void ResetCamera();
    void Clear(const sf::Color& color = sf::Color(0, 0, 0, 255));
    void Draw(sf::Text& text);
    void Draw(const E2D::Text& text);
    void Draw(const Sprite& sprite, const sf::RenderStates& states = sf::RenderStates::Default);
    void Draw(const sf::Vector2f& position, Sprite& sprite, const sf::RenderStates& states = sf::RenderStates::Default);
    void Draw(TileMapLayer& tileMapLayer, const sf::RenderStates& states = sf::RenderStates::Default);
    void AddFilter(FilterType id, Filter* effect);
    void TurnFilter(FilterType id);
    void EnableFilter(FilterType id);
    void DisableFilter(FilterType id);
    void ApplyFilters();
    void ApplyFilter(Filter& filter);
    void ClearFilters();

    inline int32_t GetWidth() { return width; }
    inline int32_t GetHeight() { return height; }
    sf::Vector2i GetMousePosition();
    sf::Vector2f GetMousePositionWithCamera();
    sf::Vector2f MapCoordsToPixel(sf::Vector2f coords);
    sf::Vector2f MapPixelToCoords(sf::Vector2f coords);
    void Display();
    void Close();
    sf::Vector2f GetSize() const;
    sf::Vector2f GetWindowRealSize() const;


    static Renderer& Instance();


    ~Renderer() = default;
private:
    friend class Filter;
    friend class BloomFilter;
    friend class LightSystem;

    static constexpr int32_t MAX_RENDERTEXTURES = 3;

    Renderer(int32_t width, int32_t height);
    sf::RenderTexture& GetActiveRenderTexture();
    sf::RenderTexture& GetNextRenderTexture(int32_t offset);
    void SwapRenderTextures();

    sf::RenderWindow window;
    sf::RenderTexture renderTextures[MAX_RENDERTEXTURES] {};
    sf::RenderTexture* activeRenderTexture { nullptr };
    std::unordered_map<FilterType, std::unique_ptr<Filter>> filters {};
    std::vector<Filter*> activeFilters {};
    uint32_t activeRenderTextureIndex { 0 };
    E2D::Camera* camera { nullptr };
    int32_t width;
    int32_t height;

    //static Renderer SingletonInstance;
};

} // namespace E2D

