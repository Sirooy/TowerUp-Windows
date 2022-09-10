#include "Torch.h"

#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Engine2D/LightSystem.h"

Torch::Torch(E2D::Light* light)
{
    auto& media = E2D::MediaContent::Instance();
    
    animation.SetFrames(media.GetAnimationFrames("Torch"));
    animation.SetDuration(0.5f);

    sf::IntRect firstFrame = animation.GetFrame(0); 
    sf::Vector3f color = light->Color;
    sprite.SetTexture(*media.GetTexture("Torch"));
    sprite.SetColor(sf::Color(255 * color.x, 255 * color.y, 255 * color.z));
    sprite.setPosition(light->Position);
    sprite.setOrigin(firstFrame.width * 0.5f, firstFrame.height * 0.25f);
    sprite.setScale(2.0f, 2.0f);

    animation.Play(sprite);
}

void Torch::Update(float dt)
{
    animation.Update(sprite, dt);
}   

void Torch::Draw()
{
    E2D::Renderer::Instance().Draw(sprite);
}