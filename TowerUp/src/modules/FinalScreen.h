#pragma once

#include "Screen.h"
#include "Kinematics.h"
#include "tinyxml2.h"


class FinalScreen : public Screen, public ScreenIDGenerator<FinalScreen>
{

public:

    FinalScreen(Game& game);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

    virtual ~FinalScreen();
protected:
    Kinematics *kinematic;

};