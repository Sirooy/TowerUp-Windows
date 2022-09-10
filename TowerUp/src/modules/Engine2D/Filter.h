#pragma once
#include "SFML/Graphics.hpp"
#include <string>
namespace E2D
{

class Renderer;

enum class FilterType : int32_t
{
    GrayScale = 0,
    Nausea,
    RGB,
    Pixelation,
    Rainbow,
    //Blindness,
    Negative,
    Bloom
};

class Filter
{
public:
    Filter(const std::string& path);

    virtual void Apply(Renderer& renderer);

    virtual ~Filter() = default;
protected:
    Filter() = default;

    sf::Shader shader;

    static sf::Vertex VertexArr[6];
};

class TimedFilter : public Filter
{
public:
    TimedFilter(const std::string& path);

    virtual void Apply(Renderer& renderer) override;

    sf::Clock& GetClock() { return clock; }
    sf::Shader& GetShader() { return shader; }

    ~TimedFilter() = default;
protected:
    sf::Clock clock;
};

class BloomFilter : public Filter
{
public:
    BloomFilter();

    virtual void Apply(Renderer& renderer) override;

    ~BloomFilter() = default;
protected:
    sf::Shader blurShader;
    sf::Shader blendShader;
};

} // namespace E2D