#include "GameScreenPlayerDead.h"

#include "Game.h"
#include "Hud.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "PathFinding.h"
#include "Engine2D/Renderer.h"
#include "MainMenuScreen.h"

#include <iostream>

GameScreenPlayerDead::GameScreenPlayerDead(Game& game, GameScreen& gameScreen) : Screen(game), gs(gameScreen)
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

    deathText.SetString("Has muerto");
    deathText.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.4f);
    deathText.SetCharacterSize(128);
    deathText.SetFillColor(sf::Color::Red);
    deathText.SetStyle(sf::Text::Bold);

    continueText.SetString("- Pulsa intro para volver al menu principal -");
    continueText.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.6f);
    continueText.SetCharacterSize(32);
    continueText.SetFillColor(sf::Color::Red);
    continueText.SetStyle(sf::Text::Bold);
}


void GameScreenPlayerDead::OnEnter()
{
    gs.ResetEntityInterpolatedPositions();
    gs.camera.Zoom(0.5f);
    gs.camera.SetCenterWithoutClamp(gs.player->Position);

    auto& media = E2D::MediaContent::Instance();

    deathText.SetFont(*media.GetFont("Font"));
    sf::FloatRect deathTextBounds = deathText.GetLocalBounds();
    deathText.SetOrigin(deathTextBounds.width * 0.5f, deathTextBounds.height * 0.5f);

    continueText.SetFont(*media.GetFont("Font"));
    sf::FloatRect continueTextBounds = continueText.GetLocalBounds();
    continueText.SetOrigin(continueTextBounds.width * 0.5f, continueTextBounds.height * 0.5f);

    deathText.SetAlpha(0);
    continueText.SetAlpha(0);
    currentBlinkTime = CONTINUE_BLINK_TIME;
}

void GameScreenPlayerDead::OnExit()
{

}

void GameScreenPlayerDead::HandleEvent(const sf::Event& e)
{
    if(deathText.IsFadedIn() && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
        game.ChangeScreen(MainMenuScreen::GetID());
}

void GameScreenPlayerDead::Update(float dt)
{
    gs.RemoveDeadEntities();
    gs.UpdateEntities(dt);

    if(deathText.FadeIn(FADE_IN_TIME, dt))
    {
        currentBlinkTime += dt;

        if(currentBlinkTime >= CONTINUE_BLINK_TIME)
        {
            currentBlinkTime = 0.0f;
            continueText.InvertAlpha();
        }
    }
}

void GameScreenPlayerDead::Draw(float tickPercent)
{
    gs.DrawTileMapAndEntitiesCentered(tickPercent);

    gs.renderer.ResetCamera();
    gs.hud->Draw(tickPercent);

    gs.renderer.Draw(deathText);
    gs.renderer.Draw(continueText);

    gs.renderer.SetCamera(gs.camera);

    gs.renderer.Display();
}