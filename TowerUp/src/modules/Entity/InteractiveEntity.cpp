#include "InteractiveEntity.h"
#include "World.h"
#include "Player.h"
#include "../Engine2D/Renderer.h"
#include "../Engine2D/MediaContent.h"
#include "../SFMLUtils.hpp"

InteractiveEntity::InteractiveEntity(sf::Vector2f position, float activeDistanceSquared) : Entity(position), 
    InteractiveElement(), world(World::Instance()), sprite(), renderStates(sf::RenderStates::Default), 
    outlineShader(E2D::MediaContent::Instance().GetShader("OutlineElement")), activeDistanceSquared(activeDistanceSquared)
{
    sprite.setPosition(position);
}


void InteractiveEntity::Update([[maybe_unused]] float dt)
{
    Player* player = world.GetPlayer();
    float distanceSquared = sf::lengthSquared(Position - player->Position);

    if(distanceSquared < activeDistanceSquared)
    {
        renderStates.shader = outlineShader;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            OnInteract();
    }
    else
        renderStates.shader = nullptr;
}

void InteractiveEntity::Draw([[maybe_unused]] float tickPercent)
{
    E2D::Renderer::Instance().Draw(sprite, renderStates);
}


