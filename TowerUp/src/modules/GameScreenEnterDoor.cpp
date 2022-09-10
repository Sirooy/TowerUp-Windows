#include "GameScreenEnterDoor.h"

#include "Game.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "PathFinding.h"
#include "GameScreenExitDoor.h"
#include "LevelGenerator.h"
#include "SFMLUtils.hpp"

#include <iostream>

GameScreenEnterDoor::GameScreenEnterDoor(Game& game, GameScreen& gameScreen) : 
    Screen(game), gs(gameScreen), filter("./resources/shaders/EnterDoor.fs")
{

}


void GameScreenEnterDoor::OnEnter()
{
    gs.ResetEntityInterpolatedPositions();

    //Normalize the coords in the range -1.0, 1.0
    sf::Vector2f playerPos = gs.renderer.MapCoordsToPixel(gs.player->Position);
    playerPos = playerPos / gs.renderer.GetSize(); //From 0.0 to 1.0
    playerPos.x *= -2.0f;   
    playerPos.y *=  2.0f;
    playerPos.x += 1.0f;
    playerPos.y -= 1.0f;

    filter.GetShader().setUniform("u_Position", playerPos);
    filter.GetClock().restart();
}

void GameScreenEnterDoor::OnExit()
{
    gs.world.ClearRoomEntities();
    gs.physics.ClearEntities();
    gs.physics.AddDynamicEntity(gs.player);
}

void GameScreenEnterDoor::HandleEvent([[maybe_unused]] const sf::Event& e)
{
    
}

void GameScreenEnterDoor::Update([[maybe_unused]] float dt)
{
    if(filter.GetClock().getElapsedTime().asSeconds() >= TRANSITION_DURATION)
        gs.ChangeScreen(GameScreenExitDoor::GetID());   
}

void GameScreenEnterDoor::Draw(float tickPercent)
{
    gs.DrawTileMapAndEntities(tickPercent);

    gs.renderer.ApplyFilter(filter);

    gs.renderer.Display();
}