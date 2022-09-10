#pragma once

#include "Screen.h"
#include "GameScreen.h"

class GameScreenExitDoor : public Screen, public ScreenIDGenerator<GameScreenExitDoor>
{
public:
    static constexpr float TRANSITION_DURATION = 1.0f;

    GameScreenExitDoor(Game& game, GameScreen& gameScreen);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

protected:
    GameScreen& gs;
    E2D::TimedFilter filter;
    bool gameHasntFinished;
};