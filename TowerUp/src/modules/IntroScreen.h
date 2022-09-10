#pragma once

#include "Screen.h"
#include "Kinematics.h"
#include "tinyxml2.h"


class IntroScreen : public Screen, public ScreenIDGenerator<IntroScreen>
{

public:

    IntroScreen(Game& game);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

    virtual ~IntroScreen();
protected:
    Kinematics *kinematic;

};