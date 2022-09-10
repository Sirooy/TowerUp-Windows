#pragma once

#include "ScreenController.h"

class Game : public ScreenController
{
public:
    static constexpr float UPDATE_TICK_TIME = 1.0f / 30.0f;

    Game();

    void Start();

    ~Game() = default;
};