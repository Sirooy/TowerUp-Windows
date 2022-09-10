#include "FinalScreen.h"
#include "Kinematics.h"


FinalScreen::FinalScreen(Game& game) : Screen(game)
{
    std::string path = "./resources/FinalKinematic/";
    kinematic = new Kinematics(game,path);
}

void FinalScreen::OnEnter()
{
    kinematic->OnEnter();
}

void FinalScreen::OnExit()
{
    kinematic->OnExit();
}

void FinalScreen::Update(float dt)
{
    kinematic->Update(dt);
}

void FinalScreen::HandleEvent(const sf::Event& e)
{
    kinematic->HandleEvent(e);
}

void FinalScreen::Draw([[maybe_unused]] float tickPercent)
{
    kinematic->Draw(tickPercent);
}

FinalScreen::~FinalScreen()
{
    delete kinematic;
}