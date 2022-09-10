#pragma once
#include "SFML/Graphics.hpp"

namespace E2D
{

class Camera;
class TileMapLayer;

class Light
{
public:
    Light(sf::Vector2f position, sf::Vector3f color, float radius);
    Light(sf::Vector2f position, uint8_t red, uint8_t green, uint8_t blue, float radius);

    sf::Vector2f Position;
    sf::Vector3f Color;
    float Radius;
    bool IsActive;

protected:
    friend class LightSystem;

    sf::Vector2f lightMapPosition;
    sf::Vector2f shadowMapPosition;
    float realRadius;
};

class LightSystem
{
public:
    static constexpr int32_t MAX_LIGHTS = 21;
    static constexpr int32_t LIGHTS_PER_SHADOW_MAP = 3;

    void CreateLightMap(int32_t width, int32_t height);
    Light* CreateLight(sf::Vector2f position, sf::Vector3f color, float radius);
    Light* CreateLight(sf::Vector2f position, uint8_t red, uint8_t green, uint8_t blue, float radius);
    Light* GetLight(int32_t index) { return &lights[index]; }
    void UpdateActiveLights(const Camera& camera);
    void RenderLightMap(const Camera& camera, TileMapLayer& shadowLayer);
    void ClearLights();

    inline static LightSystem& Instance() { static LightSystem SingletonInstance; return SingletonInstance; }
protected:
    LightSystem();

    std::vector<Light> lights {};
    std::vector<Light*> activeLights {};
    sf::RenderTexture lightMap {};
    sf::RenderTexture shadowMaps[MAX_LIGHTS / LIGHTS_PER_SHADOW_MAP] {};
    sf::Shader lightMapShader {};
    sf::Shader shadowMapShader {};
    sf::Shader blendLightMapShader {};
    sf::Vertex vertexArr[6];
    float lightMapWidth { 0.0f };
    float lightMapHeight { 0.0f };
    float scaleFactorX { 0.0f };
    float scaleFactorY { 0.0f };

    bool LightIsVisible(const Light& light, sf::Vector2f corner, sf::Vector2f size);

    //static LightSystem SingletonInstance;
};

} // namespace E2D