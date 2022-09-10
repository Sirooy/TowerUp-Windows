#include "Filter.h"
#include "Renderer.h"
#include <iostream>

namespace E2D
{

//!BASE FILTER
Filter::Filter(const std::string& path)
{
    if(!shader.loadFromFile("./resources/shaders/FilterBase.vs", path))
    {
        std::cout << "No se ha podido cargar el shader.\n" << path << '\n';
        exit(0);
    }
}

void Filter::Apply(Renderer& renderer)
{
    shader.setUniform("u_Texture", renderer.GetActiveRenderTexture().getTexture());
    renderer.SwapRenderTextures();
    renderer.GetActiveRenderTexture().clear();
    renderer.GetActiveRenderTexture().draw(VertexArr, 6, sf::PrimitiveType::Triangles, &shader);
}

sf::Vertex Filter::VertexArr[6] = 
{
    sf::Vertex{sf::Vector2f{ -1.0f, -1.0f }, sf::Vector2f{ 0.0f, 0.0f }},
    sf::Vertex{sf::Vector2f{  1.0f, -1.0f }, sf::Vector2f{ 1.0f, 0.0f }},
    sf::Vertex{sf::Vector2f{  1.0f,  1.0f }, sf::Vector2f{ 1.0f, 1.0f }},
    sf::Vertex{sf::Vector2f{ -1.0f, -1.0f }, sf::Vector2f{ 0.0f, 0.0f }},
    sf::Vertex{sf::Vector2f{  1.0f,  1.0f }, sf::Vector2f{ 1.0f, 1.0f }},
    sf::Vertex{sf::Vector2f{ -1.0f,  1.0f }, sf::Vector2f{ 0.0f, 1.0f }}
};

//!TIMED FILTER

TimedFilter::TimedFilter(const std::string& path) : Filter(path), clock{} { }

void TimedFilter::Apply(Renderer& renderer)
{
    shader.setUniform("u_Time", clock.getElapsedTime().asSeconds());
    Filter::Apply(renderer);
}

//!BLOOM FILTER
BloomFilter::BloomFilter() : Filter("./resources/shaders/BloomFirstPass.fs"), blurShader() 
{
    if(!blurShader.loadFromFile("./resources/shaders/FilterBase.vs", "./resources/shaders/BloomSecondPass.fs"))
    {
        std::cout << "No se ha podido cargar el shader.\n" << "./resources/shaders/BloomSecondPass.fs" << '\n';
        exit(0);
    }

    if(!blendShader.loadFromFile("./resources/shaders/FilterBase.vs", "./resources/shaders/BloomThirdPass.fs"))
    {
        std::cout << "No se ha podido cargar el shader.\n" << "./resources/shaders/BloomThirdPass.fs" << '\n';
        exit(0);
    }
}

void BloomFilter::Apply(Renderer& renderer)
{
    auto& currentRt = renderer.GetActiveRenderTexture();
    auto& rt1 = renderer.GetNextRenderTexture(1);
    auto& rt2 = renderer.GetNextRenderTexture(2);

    //First Pass
    shader.setUniform("u_Texture", currentRt.getTexture());
    //rt2.clear();
    rt2.draw(VertexArr, 6, sf::PrimitiveType::Triangles, &shader);
    rt2.display();

    //Horizontal blur
    blurShader.setUniform("u_HorizontalBlur", true);
    blurShader.setUniform("u_Texture", rt2.getTexture());
    //rt1.clear();
    rt1.draw(VertexArr, 6, sf::PrimitiveType::Triangles, &blurShader);
    rt1.display();

    //Vertical blur
    blurShader.setUniform("u_HorizontalBlur", false);
    blurShader.setUniform("u_Texture", rt1.getTexture());
    //rt2.clear();
    rt2.draw(VertexArr, 6, sf::PrimitiveType::Triangles, &blurShader);
    rt2.display();

    //Blend
    blendShader.setUniform("u_Texture1", currentRt.getTexture());
    blendShader.setUniform("u_Texture2", rt2.getTexture());
    renderer.SwapRenderTextures();
    //rt1.clear();
    rt1.draw(VertexArr, 6, sf::PrimitiveType::Triangles, &blendShader);
}

} // namespace E2D
