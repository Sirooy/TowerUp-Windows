#include "Renderer.h"
#include "Sprite.h"


namespace E2D
{

Renderer::Renderer(int32_t width, int32_t height) : window(sf::VideoMode(width, height), "TowerUp"), width(width), height(height)
{ 
    window.setKeyRepeatEnabled(false);

    for(int32_t i = 0;i < MAX_RENDERTEXTURES; ++i)
        renderTextures[i].create(width, height);
    activeRenderTexture = &renderTextures[activeRenderTextureIndex];
}

bool Renderer::IsOpen() const
{
    return window.isOpen();
}

void Renderer::AddFilter(FilterType id, Filter* filter)
{
    filters.emplace(id, filter);
}

void Renderer::TurnFilter(FilterType id)
{
    Filter* filter = filters.at(id).get();
    auto it = std::find(activeFilters.begin(), activeFilters.end(), filter);
    if(it == activeFilters.end())
        activeFilters.push_back(filter);
    else
        activeFilters.erase(it);
}

void Renderer::EnableFilter(FilterType id)
{
    Filter* filter = filters.at(id).get();
    activeFilters.push_back(filter);
}

void Renderer::DisableFilter(FilterType id)
{
    Filter* filter = filters.at(id).get();
    auto it = std::find(activeFilters.begin(), activeFilters.end(), filter);
    if(it != activeFilters.end())
        activeFilters.erase(it);
}

void Renderer::ClearFilters()
{
    activeFilters.clear();
}

sf::RenderTexture& Renderer::GetActiveRenderTexture()
{
    return *activeRenderTexture;
}

void Renderer::SwapRenderTextures()
{
    activeRenderTextureIndex = (activeRenderTextureIndex + 1) % MAX_RENDERTEXTURES;
    activeRenderTexture = &renderTextures[activeRenderTextureIndex];
}

sf::RenderTexture& Renderer::GetNextRenderTexture(int32_t offset)
{
    return renderTextures[(activeRenderTextureIndex + offset) % MAX_RENDERTEXTURES];
}

void Renderer::ApplyFilters()
{
    for(auto filter : activeFilters)
    {
        activeRenderTexture->display();
        filter->Apply(*this);
    }
}

void Renderer::ApplyFilter(Filter& filter)
{
    activeRenderTexture->display();
    filter.Apply(*this);
}

void Renderer::SetCamera(const Camera& camera)
{
    activeRenderTexture->setView(camera.view);
}

void Renderer::ResetCamera()
{
    activeRenderTexture->setView(activeRenderTexture->getDefaultView());
}

bool Renderer::PollEvent(sf::Event& event)
{
    return window.pollEvent(event);
}

void Renderer::Clear(const sf::Color& color)
{
    activeRenderTexture->clear(color);
}

void Renderer::Draw(sf::Text& text)
{
   activeRenderTexture->draw(text);
}

void Renderer::Draw(const E2D::Text& text)
{
   activeRenderTexture->draw(text.text);
}

void Renderer::Draw(const Sprite& sprite, const sf::RenderStates& states)
{
    if (sprite.m_texture)
    {
        sf::RenderStates statesCopy(states);
        
        statesCopy.transform *= sprite.getTransform();
        statesCopy.texture = sprite.m_texture;
        activeRenderTexture->draw(sprite.m_vertices, 4, sf::TriangleStrip, statesCopy);
    }
}

void Renderer::Draw(const sf::Vector2f& position, Sprite& sprite, const sf::RenderStates& states)
{
    if (sprite.m_texture)
    {
        sf::RenderStates statesCopy(states);
        
        sprite.setPosition(position);
        statesCopy.transform *= sprite.getTransform();
        statesCopy.texture = sprite.m_texture;
        activeRenderTexture->draw(sprite.m_vertices, 4, sf::TriangleStrip, statesCopy);
    }
}

void Renderer::Draw(TileMapLayer& tileMapLayer, const sf::RenderStates& states)//revisar
{
    if (tileMapLayer.m_texture)
    {
        sf::RenderStates statesCopy(states);
        statesCopy.transform *= tileMapLayer.getTransform();
        statesCopy.texture = tileMapLayer.m_texture;
        activeRenderTexture->draw(tileMapLayer.m_vertices, statesCopy);
    }
}

sf::Vector2i Renderer::GetMousePosition()
{
    return sf::Mouse::getPosition(window);
}

sf::Vector2f Renderer::GetMousePositionWithCamera()
{
    return activeRenderTexture->mapPixelToCoords(sf::Mouse::getPosition(window));
}

sf::Vector2f Renderer::MapCoordsToPixel(sf::Vector2f coords)
{
    return sf::Vector2f(activeRenderTexture->mapCoordsToPixel(coords)); 
}

sf::Vector2f Renderer::MapPixelToCoords(sf::Vector2f coords)
{
    return activeRenderTexture->mapPixelToCoords(sf::Vector2i(coords));
}

void Renderer::Display()
{
    activeRenderTexture->display();
    sf::Sprite sprite;
    sprite.setTexture(activeRenderTexture->getTexture());
    window.draw(sprite);
    window.display();
}

sf::Vector2f Renderer::GetSize() const
{
    return sf::Vector2f(width, height);
}

sf::Vector2f Renderer::GetWindowRealSize() const
{
    return sf::Vector2f(window.getSize());
}

Renderer& Renderer::Instance() { static Renderer SingletonInstance(1280, 720); return SingletonInstance; }

void Renderer::Close()
{
    window.close();
}

//Renderer Renderer::SingletonInstance(1280, 720); 

} // namespace E2D


