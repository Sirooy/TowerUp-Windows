#include "GameScreenPlaying.h"

#include "Game.h"
#include "Hud.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Enemies/Enemy.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/LightSystem.h"
#include "PathFinding.h"
#include "GameScreenPaused.h"
#include "GameScreenEnterDoor.h"
#include "GameScreenPlayerDead.h"
#include <iostream>

GameScreenPlaying::GameScreenPlaying(Game& game, GameScreen& gameScreen) : Screen(game), gs(gameScreen)
{

}

void GameScreenPlaying::OnEnter()
{
    std::cout << "Enter Playing\n";
}

void GameScreenPlaying::OnExit()
{
    
}

void GameScreenPlaying::HandleEvent(const sf::Event& e)
{
    gs.player->HandleEvent(e);

    if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        gs.ChangeScreen(GameScreenPaused::GetID());

    if(gs.world.CheatsOn())
    {
        if(e.type == sf::Event::KeyPressed)
        {
            switch (e.key.code)
            {
                case sf::Keyboard::Key::Enter:
                    for(auto e : gs.world.GetEnemies())
                        e->Kill();
                break;

                case sf::Keyboard::Key::Num1:
                    gs.player->AddMoney(100);
                break;

                case sf::Keyboard::Num2:
                    gs.player->GetDeagle().AddBulletDamage(10.0f);
                break;

                default: break;
            }
        }
    }
}

void GameScreenPlaying::Update(float dt)
{
    if(gs.player->IsDead)
        gs.ChangeScreen(GameScreenPlayerDead::GetID());

    gs.RemoveDeadEntities();

    if(!gs.world.DoorsAreOpen() && gs.world.GetEnemies().size() == 0)
    {
        gs.level.CurrentRoom.RemoveCurses();

    }

    gs.UpdateEntities(dt);

    if(gs.world.DoorIsSelected())
        gs.ChangeScreen(GameScreenEnterDoor::GetID());
}

void GameScreenPlaying::Draw(float tickPercent)
{
    gs.CenterCameraOnPlayer(tickPercent);

    gs.DrawTileMapAndEntities(tickPercent);

    gs.renderer.ApplyFilters();
    
    gs.renderer.ResetCamera();
    gs.hud->Draw(tickPercent);
    gs.renderer.SetCamera(gs.camera);

    gs.renderer.Display();
}