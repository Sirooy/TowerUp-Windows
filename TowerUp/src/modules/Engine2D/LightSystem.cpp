#include "LightSystem.h"
#include "Renderer.h"
#include "Camera.h"
#include "../SFMLUtils.hpp"
#include "TileMapLayer.h"
#include <iostream>
#include <limits>

namespace E2D
{

//! LIGHT
Light::Light(sf::Vector2f position, sf::Vector3f color, float radius) : 
    Position(position), Color(color), Radius(radius), IsActive(true), lightMapPosition(), realRadius(radius) { }

Light::Light(sf::Vector2f position, uint8_t red, uint8_t green, uint8_t blue, float radius) : 
    Position(position), Color(), Radius(radius), IsActive(true), lightMapPosition(), realRadius(radius)
{ 
    Color.x = static_cast<float>(red) / std::numeric_limits<uint8_t>::max();
    Color.y = static_cast<float>(green) / std::numeric_limits<uint8_t>::max();
    Color.z = static_cast<float>(blue) / std::numeric_limits<uint8_t>::max();
}

//! LIGHT SYSTEM
void LightSystem::CreateLightMap(int32_t width, int32_t height)
{
    sf::Vector2f screenSize = Renderer::Instance().GetSize();

    lightMap.create(width, height);
    lightMapWidth = width;
    lightMapHeight = height;
    scaleFactorX = width / screenSize.x;
    scaleFactorY = height / screenSize.y;

    for (int32_t i = 0; i < MAX_LIGHTS / 3; ++i)
        shadowMaps[i].create(width * 0.5f, height * 0.5f);
}

LightSystem::LightSystem()
{
    if(!lightMapShader.loadFromFile("./resources/shaders/FilterBase.vs", "./resources/shaders/LightMap.fs"))
    {
        std::cout << "No se ha podido cargar el shader de iluminacion.\n";
        exit(0);
    }
    
    if(!blendLightMapShader.loadFromFile("./resources/shaders/FilterBase.vs", "./resources/shaders/BlendLightMap.fs"))
    {
        std::cout << "No se ha podido cargar el shader de mezclado de la iluminacion.\n";
        exit(0);
    }

    if(!shadowMapShader.loadFromFile("./resources/shaders/ShadowMap.vs", "./resources/shaders/ShadowMap.gs", 
        "./resources/shaders/ShadowMap.fs"))
    {
        std::cout << "No se ha podido cargar el shader de mapeado de sombras.\n";
        exit(0);
    }
    
    vertexArr[0] = sf::Vertex{sf::Vector2f{ -1.0f, -1.0f }, sf::Vector2f{ 0.0f, 0.0f }};
    vertexArr[1] = sf::Vertex{sf::Vector2f{  1.0f, -1.0f }, sf::Vector2f{ 1.0f, 0.0f }};
    vertexArr[2] = sf::Vertex{sf::Vector2f{  1.0f,  1.0f }, sf::Vector2f{ 1.0f, 1.0f }};
    vertexArr[3] = sf::Vertex{sf::Vector2f{ -1.0f, -1.0f }, sf::Vector2f{ 0.0f, 0.0f }};
    vertexArr[4] = sf::Vertex{sf::Vector2f{  1.0f,  1.0f }, sf::Vector2f{ 1.0f, 1.0f }};
    vertexArr[5] = sf::Vertex{sf::Vector2f{ -1.0f,  1.0f }, sf::Vector2f{ 0.0f, 1.0f }};

    lights.reserve(MAX_LIGHTS * 2);
}

Light* LightSystem::CreateLight(sf::Vector2f position, sf::Vector3f color, float radius)
{
    auto& light = lights.emplace_back(position, color, radius);
    light.realRadius *= scaleFactorX;

    return &light;
}

Light* LightSystem::CreateLight(sf::Vector2f position, uint8_t red, uint8_t green, uint8_t blue, float radius)
{
    auto& light = lights.emplace_back(position, red, green, blue, radius);
    light.realRadius *= scaleFactorX;

    return &light;
}

void LightSystem::ClearLights()
{
    lights.clear();
    activeLights.clear();
}

void LightSystem::UpdateActiveLights(const Camera& camera)
{   
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();
    sf::Vector2f center = camera.GetCenter();
    sf::Vector2f topLeftCorner = center - (screenSize * 0.5f);

    activeLights.clear();
    for(std::size_t i = 0;i < lights.size(); ++i)
    {
        Light& light = lights[i];

        if(!light.IsActive || !LightIsVisible(light, topLeftCorner, screenSize))
            continue;
        
        light.lightMapPosition = E2D::Renderer::Instance().MapCoordsToPixel(light.Position);
        light.shadowMapPosition =  light.lightMapPosition;

        light.shadowMapPosition.x = light.shadowMapPosition.x / (screenSize.x);
        light.shadowMapPosition.y = light.shadowMapPosition.y / (screenSize.y);
        light.shadowMapPosition.x = (light.shadowMapPosition.x * 2.0f) - 1.0f;
        light.shadowMapPosition.y = (light.shadowMapPosition.y * 2.0f) - 1.0f;
        light.shadowMapPosition.y = -light.shadowMapPosition.y;

        light.lightMapPosition.x = light.lightMapPosition.x *  scaleFactorX;
        light.lightMapPosition.y = light.lightMapPosition.y * -scaleFactorY + lightMapHeight; //Transformar a coordenadas igual que gl_FragCoord
        activeLights.push_back(&light);
    }
}

bool LightSystem::LightIsVisible(const Light& light, sf::Vector2f corner, sf::Vector2f size)
{
    if(light.Position.x > corner.x && light.Position.x < corner.x + size.x &&
        light.Position.y > corner.y && light.Position.y < corner.y + size.y) //Esta dentro
        return true;

    sf::Vector2f closestPoint = sf::Vector2f
    (
        sf::clamp(light.Position.x, corner.x, corner.x + size.x),
        sf::clamp(light.Position.y, corner.y, corner.y + size.y)
    );

    float dist = sf::lengthSquared(closestPoint - light.Position);

    return dist < light.Radius * light.Radius;
}


void LightSystem::RenderLightMap(const Camera& camera, TileMapLayer& shadowLayer)
{
    //Render shadow maps
    sf::RenderTexture* currentShadowMap = nullptr;
    int32_t currentShadowMapIndex = -1;
    int32_t currentLightIndex = 0;
    sf::RenderStates states(sf::BlendAdd);
    states.shader = &shadowMapShader;

    for (std::size_t i = 0; i < activeLights.size(); ++i)
    {
        if (i % LIGHTS_PER_SHADOW_MAP == 0)
        {
            if (currentShadowMap)
                currentShadowMap->display();

            currentShadowMap = &shadowMaps[++currentShadowMapIndex];
            currentShadowMap->clear(sf::Color(0, 0, 0, 0));
            currentShadowMap->setView(camera.GetView());
        }

        shadowMapShader.setUniform("u_LightIndex", currentLightIndex);
        shadowMapShader.setUniform("u_LightPosition", activeLights[i]->shadowMapPosition);
        currentShadowMap->draw(shadowLayer.m_vertices, states);

        currentLightIndex = (currentLightIndex + 1) % LIGHTS_PER_SHADOW_MAP;
    }

    if (currentShadowMap)
        currentShadowMap->display();

    //Set uniforms
    lightMapShader.setUniform("u_ActiveLights", static_cast<int32_t>(activeLights.size()));
    for(std::size_t i = 0;i < activeLights.size(); ++i)
    {
        Light* light = activeLights[i];
        int32_t shadowMapIndex = i / LIGHTS_PER_SHADOW_MAP;
        int32_t shadowMapColorIndex = i % LIGHTS_PER_SHADOW_MAP;
        std::string id = "u_Lights[" + std::to_string(i) + "].";
        lightMapShader.setUniform(id + "Position", light->lightMapPosition);
        lightMapShader.setUniform(id + "Color", light->Color);
        lightMapShader.setUniform(id + "Radius", light->realRadius);
        lightMapShader.setUniform(id + "ShadowMapIndex", shadowMapIndex);
        lightMapShader.setUniform(id + "ShadowMapColorIndex", shadowMapColorIndex);
        lightMapShader.setUniform("u_ShadowMaps[" + std::to_string(shadowMapIndex)  + "]", shadowMaps[shadowMapIndex].getTexture());
    }

    //Render map
    lightMap.clear(sf::Color::White);
    lightMap.draw(vertexArr, 6, sf::PrimitiveType::Triangles, &lightMapShader);
    lightMap.display();

    auto& renderer = E2D::Renderer::Instance();

    blendLightMapShader.setUniform("u_LightMap", lightMap.getTexture());
    blendLightMapShader.setUniform("u_Texture", renderer.GetActiveRenderTexture().getTexture());
    
    renderer.SwapRenderTextures();
    renderer.GetActiveRenderTexture().clear();
    renderer.GetActiveRenderTexture().draw(vertexArr, 6, sf::PrimitiveType::Triangles, &blendLightMapShader);
    
    /*
    sf::Sprite sprite;
    sprite.setTexture(lightMap.getTexture());
    E2D::Renderer::Instance().window.draw(sprite);
    E2D::Renderer::Instance().window.display();
    */
}

//LightSystem LightSystem::SingletonInstance;

} // namespace E2D
