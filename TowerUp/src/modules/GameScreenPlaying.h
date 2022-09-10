#pragma once

#include "Screen.h"
#include "GameScreen.h"

class GameScreenPlaying : public Screen, public ScreenIDGenerator<GameScreenPlaying>
{
public:
    GameScreenPlaying(Game& game, GameScreen& gameScreen);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

protected:
    GameScreen& gs;
};