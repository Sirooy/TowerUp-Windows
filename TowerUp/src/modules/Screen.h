#pragma once

#include <cstdint>
#include "SFML/Window/Event.hpp"
#include "ScreenController.h"

class Game;

class Screen : public ScreenController
{
public:
    Screen(Game& game) : ScreenController(), game(game) { };

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void HandleEvent(const sf::Event& e) = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw(float tickPercent) = 0;

    virtual ~Screen() = default;

protected:
    Game& game;
};