#include "GameScreenPaused.h"

#include "Game.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "PathFinding.h"
#include "GameScreenPlaying.h"
#include "MainMenuScreen.h"

#include <iostream>

GameScreenPaused::GameScreenPaused(Game& game, GameScreen& gameScreen) : 
    Screen(game), gs(gameScreen), filter("./resources/shaders/PauseScreen.fs"), 
    titleText(), resumeButton(), exitButton(), currentBlinkTime(0.0f)
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

    titleText.SetString("Pausa");
    titleText.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.1f);
    titleText.SetCharacterSize(130);
    titleText.SetFillColor(sf::Color::White);
    titleText.SetOutlineColor(sf::Color::Black);
    titleText.SetOutlineThickness(4);
    titleText.SetStyle(sf::Text::Bold);

    resumeButton.SetText("Reanudar");
    resumeButton.SetDefaultRect(sf::IntRect(0, 80, 176, 48));
    resumeButton.SetHoverRect(sf::IntRect(0, 32, 176, 48));
    resumeButton.SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    resumeButton.SetTextCharacterSize(32);
    resumeButton.SetTextColor(sf::Color::White);
    resumeButton.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);

    exitButton.SetText("Salir");
    exitButton.SetDefaultRect(sf::IntRect(0, 80, 176, 48));
    exitButton.SetHoverRect(sf::IntRect(0, 32, 176, 48));
    exitButton.SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    exitButton.SetTextCharacterSize(32);
    exitButton.SetTextColor(sf::Color::White);
    exitButton.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.7f);
}


void GameScreenPaused::OnEnter()
{
    gs.ResetEntityInterpolatedPositions();

    auto& media = E2D::MediaContent::Instance();

    titleText.SetFont(*media.GetFont("Font"));
    sf::FloatRect bounds = titleText.GetGlobalBounds();
    titleText.SetOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

    resumeButton.SetTexture(media.GetTexture("UI"));
    resumeButton.SetTextFont(media.GetFont("Font"));

    exitButton.SetTexture(media.GetTexture("UI"));
    exitButton.SetTextFont(media.GetFont("Font"));

    currentBlinkTime = 0.0f;
}

void GameScreenPaused::OnExit()
{
    titleText.SetAlpha(255);
}

void GameScreenPaused::HandleEvent(const sf::Event& e)
{
    if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        gs.ChangeScreen(GameScreenPlaying::GetID());

    resumeButton.Update(e);
    exitButton.Update(e);

    if(resumeButton.IsClicked())
    {
        gs.ChangeScreen(GameScreenPlaying::GetID());
    }

    if(exitButton.IsClicked())
    {
        game.ChangeScreen(MainMenuScreen::GetID());
    }
}

void GameScreenPaused::Update([[maybe_unused]] float dt)
{
    currentBlinkTime += dt;
    if(currentBlinkTime >= TITLE_BLINK_TIME)
    {
        currentBlinkTime = 0.0f;
        titleText.InvertAlpha();
    }
}

void GameScreenPaused::Draw(float tickPercent)
{
    gs.DrawTileMapAndEntities(tickPercent);

    gs.renderer.ApplyFilter(filter);

    gs.renderer.ResetCamera();
    gs.renderer.Draw(titleText);
    resumeButton.Draw();
    exitButton.Draw();

    gs.renderer.Display();
}