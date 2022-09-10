#include "GameScreenExitDoor.h"

#include "Game.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Entity/Enemies/Enemy.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "PathFinding.h"
#include "GameScreenPlaying.h"
#include "SFMLUtils.hpp"
#include "Engine2D/LightSystem.h"
#include "FinalScreen.h"

GameScreenExitDoor::GameScreenExitDoor(Game& game, GameScreen& gameScreen) : 
    Screen(game), gs(gameScreen), filter("./resources/shaders/ExitDoor.fs"), gameHasntFinished(false)
{

}


void GameScreenExitDoor::OnEnter()
{
    auto& lightSystem = E2D::LightSystem::Instance();

    lightSystem.ClearLights();
    gs.player->SetLight(lightSystem.CreateLight(gs.player->Position, 100, 100, 100, 350));
    gs.player->GetDeagle().SetLight(lightSystem.CreateLight(sf::Vector2f(0.0f, 0.0f), 0, 0, 0, 250));
    gameHasntFinished = gs.level.CreateNextRoom(gs.world.GetSelectedDoor());

    if(gameHasntFinished)
    {
        gs.level.CurrentRoom.ApplyCurses();
        gs.world.SetGlobalDropProbability(gs.level.CurrentRoom.GetNumCurses() * Enemy::DROP_INCREASE_PER_CURSE);
        gs.world.ResetSelectedDoor();

        gs.physics.SetCollisionLayer(gs.level.CurrentRoom.GetCollisionLayer());
        gs.pathfinding.UpdateGrid(gs.level.CurrentRoom.GetCollisionLayer(), 
                                  gs.level.CurrentRoom.GetWidthInTiles(), 
                                  gs.level.CurrentRoom.GetHeightInTiles());
        gs.SetCameraLimits();
        gs.CenterCameraOnPlayer(0.0f);

        sf::Vector2f playerPos = gs.renderer.MapCoordsToPixel(gs.player->Position);
        playerPos = playerPos / gs.renderer.GetSize(); //From 0.0 to 1.0
        playerPos.x *= -2.0f;   
        playerPos.y *=  2.0f;
        playerPos.x += 1.0f;
        playerPos.y -= 1.0f;

        filter.GetShader().setUniform("u_Position", playerPos);
        filter.GetClock().restart();
    }
    else
        game.ChangeScreen(FinalScreen::GetID());
}

void GameScreenExitDoor::OnExit()
{
    
}

void GameScreenExitDoor::HandleEvent([[maybe_unused]] const sf::Event& e)
{
    
}

void GameScreenExitDoor::Update([[maybe_unused]] float dt)
{
    if(filter.GetClock().getElapsedTime().asSeconds() >= TRANSITION_DURATION)
        gs.ChangeScreen(GameScreenPlaying::GetID());   
}

void GameScreenExitDoor::Draw(float tickPercent)
{
    if(gameHasntFinished)
    {
        gs.DrawTileMapAndEntities(tickPercent);

        gs.renderer.ApplyFilter(filter);
        gs.renderer.ApplyFilters();

        gs.renderer.Display();
    }
    else
        gs.renderer.Clear();
}