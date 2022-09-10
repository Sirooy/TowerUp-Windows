#pragma once

#include "Screen.h"
#include "GameScreen.h"
#include "Button.h"

class GameScreenPaused : public Screen, public ScreenIDGenerator<GameScreenPaused>
{
public:
    GameScreenPaused(Game& game, GameScreen& gameScreen);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

protected:
    static constexpr float BUTTON_WIDTH = 300.0f;
    static constexpr float BUTTON_HEIGHT = 100.0f;
    static constexpr float TITLE_BLINK_TIME = 0.5f;

    GameScreen& gs;
    E2D::Filter filter;
    E2D::Text titleText;
    Button resumeButton;
    Button exitButton;
    float currentBlinkTime;
};