#include "IntroScreen.h"
#include "Kinematics.h"


IntroScreen::IntroScreen(Game& game) : Screen(game)
{
    std::string path = "./resources/InitialKinematic/";
    kinematic = new Kinematics(game,path);
}

void IntroScreen::OnEnter()
{
    kinematic->OnEnter();
}

void IntroScreen::OnExit()
{
    kinematic->OnExit();
}

void IntroScreen::Update(float dt)
{
    kinematic->Update(dt);
}

void IntroScreen::HandleEvent(const sf::Event& e)
{
    kinematic->HandleEvent(e);
}

void IntroScreen::Draw([[maybe_unused]] float tickPercent)
{
    kinematic->Draw(tickPercent);
}

IntroScreen::~IntroScreen()
{
    delete kinematic;
}
