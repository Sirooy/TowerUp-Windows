#include "Game.h"

#include "SFML/System/Clock.hpp"
#include "Engine2D/Renderer.h"
#include "Engine2D/Sprite.h"
#include "Engine2D/Filter.h"
#include "Engine2D/LightSystem.h"
#include "IntroScreen.h"
#include "MainMenuScreen.h"
#include "SettingsScreen.h"
#include "GameScreen.h"
#include "PathFinding.h"
#include "FinalScreen.h"

#include <iostream>

Game::Game() : ScreenController()
{
    screens.emplace(std::make_pair(IntroScreen::GetID(), new IntroScreen(*this)));
    screens.emplace(std::make_pair(MainMenuScreen::GetID(), new MainMenuScreen(*this)));
    screens.emplace(std::make_pair(SettingsScreen::GetID(), new SettingsScreen(*this)));
    screens.emplace(std::make_pair(GameScreen::GetID(), new GameScreen(*this)));
    screens.emplace(std::make_pair(FinalScreen::GetID(), new FinalScreen(*this)));

    currentScreen = GetScreen(GameScreen::GetID());

    E2D::Renderer& renderer = E2D::Renderer::Instance();
    renderer.AddFilter(E2D::FilterType::GrayScale, new E2D::Filter("./resources/shaders/GrayScaleFilter.fs"));
    renderer.AddFilter(E2D::FilterType::Nausea, new E2D::TimedFilter("./resources/shaders/NauseaFilter.fs"));
    renderer.AddFilter(E2D::FilterType::RGB, new E2D::TimedFilter("./resources/shaders/RGBFilter.fs"));
    renderer.AddFilter(E2D::FilterType::Pixelation, new E2D::Filter("./resources/shaders/PixelationFilter.fs"));
    renderer.AddFilter(E2D::FilterType::Rainbow, new E2D::TimedFilter("./resources/shaders/RainbowFilter.fs"));
    renderer.AddFilter(E2D::FilterType::Negative, new E2D::TimedFilter("./resources/shaders/NegativeFilter.fs"));

    E2D::LightSystem::Instance().CreateLightMap(renderer.GetSize().x * 0.5f, renderer.GetSize().y * 0.5f);

    PathFinding& pathFinding = PathFinding::Instance();
    pathFinding.CreateGrid(LevelGenerator::MAX_WIDTH, LevelGenerator::MAX_HEIGHT);
    pathFinding.SetMaxRequests(4);
}

void Game::Start()
{
    E2D::Renderer& renderer = E2D::Renderer::Instance();

    currentScreen->OnEnter();

    sf::Clock updateClock;
    sf::Clock deltaClock; //!
    uint32_t count = 0;   //!
    while(renderer.IsOpen())
    {
        ++count;            //!
        if(count%500 == 0)  //!
            std::cout << 500.0f / deltaClock.restart().asSeconds() << '\n'; //!

        SwapScreens();

        sf::Event event;
        while(renderer.PollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                renderer.Close();

            currentScreen->HandleEvent(event);
        }

        if(updateClock.getElapsedTime().asSeconds() > Game::UPDATE_TICK_TIME)
        {
            float dt = updateClock.restart().asSeconds();
            currentScreen->Update(dt);
        }
        
        float tickPercent = std::min(1.0f, updateClock.getElapsedTime().asSeconds() / Game::UPDATE_TICK_TIME);
        currentScreen->Draw(tickPercent);
    }
}