#pragma once

#include "Screen.h"
#include "GameScreen.h"
#include "Engine2D/Filter.h"

class GameScreenEnterDoor : public Screen, public ScreenIDGenerator<GameScreenEnterDoor>
{
public:
    static constexpr float TRANSITION_DURATION = 1.0f;

    GameScreenEnterDoor(Game& game, GameScreen& gameScreen);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

protected:
    GameScreen& gs;
    E2D::TimedFilter filter;
};