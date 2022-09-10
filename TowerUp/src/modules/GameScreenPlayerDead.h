#pragma once

#include "Screen.h"
#include "GameScreen.h"

class GameScreenPlayerDead : public Screen, public ScreenIDGenerator<GameScreenPlayerDead>
{
public:
    GameScreenPlayerDead(Game& game, GameScreen& gameScreen);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

protected:
    static constexpr float FADE_IN_TIME = 3.0f;
    static constexpr float CONTINUE_BLINK_TIME = 0.5f;
    
    GameScreen& gs;

    E2D::Text deathText { };
    E2D::Text continueText { };
    float currentBlinkTime { 0.0f };
};