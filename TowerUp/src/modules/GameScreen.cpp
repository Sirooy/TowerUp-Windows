#include "GameScreen.h"

#include "Game.h"
#include "Hud.h"
#include "Entity/World.h"
#include "Entity/Player.h"
#include "Entity/Enemies/Enemy.h"
#include "Entity/Bullet.h"
#include "Entity/Consumable.h"
#include "Entity/Trap.h"
#include "Entity/GenericEntity.h"
#include "Physics/PhysicsEngine.h"
#include "Engine2D/MediaContent.h"
#include "PathFinding.h"
#include "MainMenuScreen.h"
#include "GameScreenPlaying.h"
#include "GameScreenPaused.h"
#include "GameScreenEnterDoor.h"
#include "GameScreenExitDoor.h"
#include "GameScreenPlayerDead.h"
#include "Engine2D/LightSystem.h"

GameScreen::GameScreen(Game& game) : Screen(game), world(World::Instance()), physics(PhysicsEngine::Instance()), 
    pathfinding(PathFinding::Instance()), renderer(E2D::Renderer::Instance()), media(E2D::MediaContent::Instance()), level()
{
    screens.emplace(std::make_pair(GameScreenPlaying::GetID(), new GameScreenPlaying(game, *this)));
    screens.emplace(std::make_pair(GameScreenPaused::GetID(), new GameScreenPaused(game, *this)));
    screens.emplace(std::make_pair(GameScreenEnterDoor::GetID(), new GameScreenEnterDoor(game, *this)));
    screens.emplace(std::make_pair(GameScreenExitDoor::GetID(), new GameScreenExitDoor(game, *this)));
    screens.emplace(std::make_pair(GameScreenPlayerDead::GetID(), new GameScreenPlayerDead(game, *this)));

    camera.SetSize(renderer.GetSize());
}


void GameScreen::OnEnter()
{
    LoadTextures();
    LoadSounds();
    LoadAnimations();
    media.LoadShader("OutlineElement", "./resources/shaders/OutlineElement.fs");
    media.LoadShader("ForegroundAmbient", "./resources/shaders/ForegroundAmbient.fs");
    media.LoadFont("Font", "resources/fonts/Modern.ttf");
    
    hud = world.CreateHud();
    player = world.CreatePlayer();
    foregroundShader = media.GetShader("ForegroundAmbient");
    
    world.ResetSelectedDoor();
    //level.SetSeed(5); //6
    level.Reset();
    level.CreateHub();
    player->SetLight(E2D::LightSystem::Instance().CreateLight(player->Position, 100, 100, 100, 350));
    player->GetDeagle().SetLight(E2D::LightSystem::Instance().CreateLight(sf::Vector2f(0.0f, 0.0f), 0, 0, 0, 250));
    
    camera.Reset(sf::FloatRect(sf::Vector2f(0, 0), renderer.GetSize()));
    SetCameraLimits();

    currentScreen = screens.at(GameScreenPlaying::GetID()).get();
    currentScreen->OnEnter();

    media.LoadMusic("Dungeon","./resources/sounds/Dungeon.wav");
    music = media.GetMusic("Dungeon");
    music->Play();
    music->SetVolume(3.0f);
    music->SetLoop(true);

    World::Instance().CreateItem(ItemType::BlueSoulItem, sf::Vector2f(544.0f, 320.0f));
    World::Instance().CreateItem(ItemType::MiniGunItem, sf::Vector2f(672.0f, 320.0f));
    World::Instance().CreateItem(ItemType::RocketsItem, sf::Vector2f(800.0f, 320.0f) );
    World::Instance().CreateItem(ItemType::WizardHatItem, sf::Vector2f(928.0f, 320.0f));
    World::Instance().CreateItem(ItemType::PineAppleItem, sf::Vector2f(1056.0f, 320.0f));
    World::Instance().CreateItem(ItemType::WatermelonItem, sf::Vector2f(1184.0f, 320.0f));

    World::Instance().CreateItem(ItemType::DoubleDeagleItem, sf::Vector2f(544.0f, 512.0f));
    World::Instance().CreateItem(ItemType::SantoryuItem, sf::Vector2f(672.0f, 512.0f));
    World::Instance().CreateItem(ItemType::QuadShotItem, sf::Vector2f(800.0f, 512.0f));
    World::Instance().CreateItem(ItemType::ShotGunItem, sf::Vector2f(928.0f, 512.0f));
    World::Instance().CreateItem(ItemType::CannonItem, sf::Vector2f(1056.0f, 512.0f));
    World::Instance().CreateItem(ItemType::BurstShotItem, sf::Vector2f(1184.0f, 512.0f));

    World::Instance().CreateItem(ItemType::DiamondItem, sf::Vector2f(544.0f, 704.0f));
    World::Instance().CreateItem(ItemType::SoapItem, sf::Vector2f(672.0f, 704.0f));
    World::Instance().CreateItem(ItemType::HeavyBulletsItem, sf::Vector2f(800.0f, 704.0f));
    World::Instance().CreateItem(ItemType::SoyMilkItem, sf::Vector2f(928.0f, 704.0f));
    World::Instance().CreateItem(ItemType::GhostFruitItem, sf::Vector2f(1056.0f, 704.0f));
    World::Instance().CreateItem(ItemType::AstronautHelmetItem, sf::Vector2f(1184.0f, 704.0f));
    
    
    
}

void GameScreen::LoadTextures()
{
    media.LoadTexture("TilesetZone1", "./resources/sprites/tilesets/TextureMap_1.png");
    media.LoadTexture("TilesetZone2", "./resources/sprites/tilesets/TextureMap_2.png");
    media.LoadTexture("TilesetZone3", "./resources/sprites/tilesets/TextureMap_3.png");
    media.LoadTexture("terrain", "./resources/sprites/terrain.png");
    media.LoadTexture("Doors", "./resources/sprites/Doors.png");
    media.LoadTexture("Player", "./resources/sprites/PlayerSpriteSheet.png");
    media.LoadTexture("Bullets", "./resources/sprites/BulletsSpriteSheet.png");
    media.LoadTexture("FlyingEye", "./resources/sprites/Enemies/FlyingEye.png");
    media.LoadTexture("PossessedCircularSaw", "./resources/sprites/Enemies/PossessedCircularSaw.png");
    media.LoadTexture("BatEye", "./resources/sprites/Enemies/BatEye.png");
    media.LoadTexture("AxeSkeleton", "./resources/sprites/Enemies/AxeSkeleton.png");
    media.LoadTexture("RogueKnight","./resources/sprites/Enemies/bosses/RogueKnight.png");
    media.LoadTexture("VendingMachine", "./resources/sprites/VendingMachine.png");
    media.LoadTexture("Consumables", "./resources/sprites/Consumables.png");
    media.LoadTexture("Items", "./resources/sprites/Items.png");
    media.LoadTexture("Traps", "./resources/sprites/Traps.png");
    media.LoadTexture("Hud", "./resources/sprites/Hud.png");
    media.LoadTexture("Curses", "./resources/sprites/Curses.png");
    media.LoadTexture("UI", "./resources/sprites/Menu/texturas-menu.png");
    media.LoadTexture("Torch", "./resources/sprites/Torch.png");
}

void GameScreen::LoadSounds()
{
    //Player
    media.LoadSoundBuffer("PlayerJump","./resources/sounds/PlayerJump.wav");
    media.LoadSoundBuffer("PlayerDash","./resources/sounds/PlayerDash.wav");
    media.LoadSoundBuffer("PlayerDead","./resources/sounds/PlayerDead.wav");
    media.LoadSoundBuffer("PlayerHurt","./resources/sounds/PlayerHurt.wav");

    //VendingMachine
    media.LoadSoundBuffer("VendingMachineCoinIn","./resources/sounds/VendingMachineCoinIn.wav");
    media.LoadSoundBuffer("VendingMachineSuccess","./resources/sounds/VendingMachineSuccess.wav");
    media.LoadSoundBuffer("VendingMachineError","./resources/sounds/VendingMachineError.wav");
    media.LoadSoundBuffer("VendingMachineWorking","./resources/sounds/VendingMachineWorking.wav");

    //Deagle
    media.LoadSoundBuffer("DesertEagleShoot","./resources/sounds/DesertEagleShoot.wav");
    media.LoadSoundBuffer("DesertEagleReload","./resources/sounds/DesertEagleReload.wav");

    //ENEMIES
    //FlyingEye
    media.LoadSoundBuffer("FlyingEyeAttack","./resources/sounds/FlyingEyeAttack.wav");
    media.LoadSoundBuffer("FlyingEyeDie","./resources/sounds/FlyingEyeDie.wav");

    //PossessedCircularSaw
    media.LoadSoundBuffer("PossessedCircularSawCharging","./resources/sounds/PossessedCircularSawCharging.wav");
    media.LoadSoundBuffer("PossessedCircularSawCollision","./resources/sounds/PossessedCircularSawCollision.wav");
    media.LoadSoundBuffer("PossessedCircularSawDie","./resources/sounds/PossessedCircularSawDie.wav");

    //BatEye
    media.LoadSoundBuffer("BatEyeDie","./resources/sounds/BatEyeDie.wav");
    media.LoadSoundBuffer("BatEyeShoot","./resources/sounds/BatEyeShoot.wav");

    //Consumables
    media.LoadSoundBuffer("PickHeart","./resources/sounds/PickHeart.wav");
    media.LoadSoundBuffer("PickCoin","./resources/sounds/PickCoin.wav");

    //Door
    media.LoadSoundBuffer("DoorEnter","./resources/sounds/DoorEnter.wav");

    //Items
    media.LoadSoundBuffer("ItemPickUp","./resources/sounds/ItemPickUp.wav");
    
    //Traps
    //ProximitySpikeTrap
    media.LoadSoundBuffer("ProximitySpikeTrapTrigger","./resources/sounds/ProximitySpikeTrapTrigger.wav");
    media.LoadSoundBuffer("ProximitySpikeTrapActivate","./resources/sounds/ProximitySpikeTrapActivate.wav");

    //ShooterTrap
    media.LoadSoundBuffer("ShooterTrapShoot","./resources/sounds/ShooterTrapShoot.wav");
    
    //CircularSawTrap 
    media.LoadSoundBuffer("CircularSawTrapCollision","./resources/sounds/CircularSawTrapCollision.wav");
    
    
    //Bullet
    media.LoadSoundBuffer("BulletCollisionEnemy","./resources/sounds/BulletCollisionEnemy.wav");
    media.LoadSoundBuffer("BulletCollisionTile","./resources/sounds/BulletCollisionTile.wav");
    media.LoadSoundBuffer("FireMagicBulletCollisionTile","./resources/sounds/FireMagicBulletCollisionTile.wav");


    //Rogue Knight
    media.LoadSoundBuffer("RogueKnightShoot","./resources/sounds/RogueKnightShoot.wav");
    media.LoadSoundBuffer("RogueKnightDie","./resources/sounds/RogueKnightDie.wav");
    media.LoadSoundBuffer("RogueKnightSummon","./resources/sounds/RogueKnightSummon.wav");
    media.LoadSoundBuffer("RogueKnightBlade","./resources/sounds/RogueKnightBlade.wav");

}

void GameScreen::LoadAnimations()
{
    media.LoadAnimationFrames("./resources/animations/Player.xml");
    media.LoadAnimationFrames("./resources/animations/Bullets.xml");
    media.LoadAnimationFrames("./resources/animations/enemies/FlyingEye.xml");
    media.LoadAnimationFrames("./resources/animations/enemies/PossessedCircularSaw.xml");
    media.LoadAnimationFrames("./resources/animations/enemies/BatEye.xml");
    media.LoadAnimationFrames("./resources/animations/enemies/AxeSkeleton.xml");
    media.LoadAnimationFrames("./resources/animations/enemies/bosses/RogueKnight.xml");
    media.LoadAnimationFrames("./resources/animations/Door.xml");
    media.LoadAnimationFrames("./resources/animations/Consumables.xml");
    media.LoadAnimationFrames("./resources/animations/Traps.xml");
    media.LoadAnimationFrames("./resources/animations/Hud.xml");
    media.LoadAnimationFrames("./resources/animations/Torch.xml");
}


void GameScreen::OnExit()
{
    if(music != nullptr)
        music->Stop();

    level.CurrentRoom.RemoveCurses();
    ClearResources();
}

void GameScreen::HandleEvent([[maybe_unused]] const sf::Event& e)
{
    currentScreen->HandleEvent(e);
}

void GameScreen::Update([[maybe_unused]] float dt)
{
    currentScreen->Update(dt);
}

void GameScreen::RemoveDeadEntities()
{
    world.RemoveDeadEnemies();
    world.RemoveDeadBullets();
    world.RemoveDeadConsumables();
    world.RemoveDeadInteractiveEntities();
    world.RemoveDeadGenericEntities();
}

void GameScreen::Draw(float tickPercent)
{
    currentScreen->Draw(tickPercent);
}

void GameScreen::UpdateEntities(float dt)
{
    player->Update(dt);
    
    auto& enemies = world.GetEnemies();
    for(size_t i = 0;i < enemies.size(); ++i)
        enemies[i]->Update(dt);

    auto& traps = world.GetTraps();
    for(size_t i = 0;i < traps.size(); ++i)
        traps[i]->Update(dt);

    auto& bullets = world.GetBullets();
    for(size_t i = 0;i < bullets.size(); ++i)
        bullets[i]->Update(dt);

    auto& consumables = world.GetConsumables();
    for(size_t i = 0;i < consumables.size(); ++i)
        consumables[i]->Update(dt);

    auto& iEntities = world.GetInteractiveEntities();
    for(size_t i = 0;i < iEntities.size(); ++i)
        iEntities[i]->Update(dt);

    auto& gEntities = world.GetGenericEntities();
    for(size_t i = 0;i < gEntities.size(); ++i)
        gEntities[i]->Update(dt);    

    auto& doors = world.GetDoors();
    for(size_t i = 0;i < doors.size(); ++i)
        doors[i]->Update(dt);

    level.CurrentRoom.UpdateTorches(dt);
    physics.UpdatePhysics(dt);
    pathfinding.ResetRequests();

    hud->Update(dt);
}

void GameScreen::SetCameraLimits()
{
    sf::Vector2f halfScreenSize = renderer.GetSize() * 0.5f;
    sf::Vector2f roomSize = level.CurrentRoom.GetSize();

    camera.SetLimits(halfScreenSize, roomSize - halfScreenSize); 
}

void GameScreen::CenterCameraOnPlayer(float tickPercent)
{
    camera.SetCenter(player->GetInterpolatedPosition(tickPercent));
    renderer.SetCamera(camera);
}

void GameScreen::DrawTileMapAndEntities(float tickPercent)
{
    renderer.Clear();
    player->UpdateLightPosition(tickPercent);
    CenterCameraOnPlayer(tickPercent);

    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Background);
    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Decoration);
    level.CurrentRoom.DrawTorches();

    for(auto d : world.GetDoors())
        d->Draw(tickPercent);

    for(auto t : world.GetTraps())
        t->Draw(tickPercent);

    for(auto i : world.GetInteractiveEntities())
        i->Draw(tickPercent);

    for(auto e : world.GetEnemies())
        e->Draw(tickPercent);

    E2D::LightSystem::Instance().UpdateActiveLights(camera);
    E2D::LightSystem::Instance().RenderLightMap(camera, level.CurrentRoom.GetTileMapLayer(E2D::TileMap::LayerType::Shadow));

    CenterCameraOnPlayer(tickPercent);

    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Foreground, foregroundShader);

    for(auto c : world.GetConsumables())
        c->Draw(tickPercent);

    for(auto b : world.GetBullets())
        b->Draw(tickPercent);

    for(auto g : world.GetGenericEntities())
        g->Draw(tickPercent);

    renderer.ApplyFilter(bloomFilter);

    CenterCameraOnPlayer(tickPercent);
    player->Draw(tickPercent);
}

void GameScreen::DrawTileMapAndEntitiesCentered(float tickPercent)
{
    renderer.Clear();
    player->UpdateLightPosition(tickPercent);
    sf::Vector2f playerPos = player->GetInterpolatedPosition(tickPercent);
    camera.SetCenterWithoutClamp(playerPos);

    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Background);
    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Foreground);
    level.CurrentRoom.Draw(E2D::TileMap::LayerType::Decoration);
    level.CurrentRoom.DrawTorches();

    for(auto d : world.GetDoors())
        d->Draw(tickPercent);

    for(auto t : world.GetTraps())
        t->Draw(tickPercent);

    for(auto i : world.GetInteractiveEntities())
        i->Draw(tickPercent);

    for(auto e : world.GetEnemies())
        e->Draw(tickPercent);

    E2D::LightSystem::Instance().UpdateActiveLights(camera);
    E2D::LightSystem::Instance().RenderLightMap(camera, level.CurrentRoom.GetTileMapLayer(E2D::TileMap::LayerType::Shadow));

    camera.SetCenterWithoutClamp(playerPos);

    for(auto c : world.GetConsumables())
        c->Draw(tickPercent);

    for(auto b : world.GetBullets())
        b->Draw(tickPercent);

    for(auto g : world.GetGenericEntities())
        g->Draw(tickPercent); 

    renderer.ApplyFilter(bloomFilter); //Apply Bloom

    camera.SetCenterWithoutClamp(playerPos);
    player->Draw(tickPercent);
}

void GameScreen::ClearResources()
{
    renderer.ResetCamera();
    physics.ClearEntities();
    world.ClearEntities();
    media.UnloadResources();
    E2D::LightSystem::Instance().ClearLights();
}

void GameScreen::ResetEntityInterpolatedPositions()
{
    player->ResetInterpolatedPosition();

    auto& enemies = world.GetEnemies();
    for(size_t i = 0;i < enemies.size(); ++i)
        enemies[i]->ResetInterpolatedPosition();

    auto& bullets = world.GetBullets();
    for(size_t i = 0;i < bullets.size(); ++i)
        bullets[i]->ResetInterpolatedPosition();

    auto& consumables = world.GetConsumables();
    for(size_t i = 0;i < consumables.size(); ++i)
        consumables[i]->ResetInterpolatedPosition();

    auto& traps = world.GetTraps();
    for(size_t i = 0;i < traps.size(); ++i)
        traps[i]->ResetInterpolatedPosition();

    auto& iEntities = world.GetInteractiveEntities();
    for(size_t i = 0;i < iEntities.size(); ++i)
        iEntities[i]->ResetInterpolatedPosition();

    auto& gEntities = world.GetGenericEntities();
    for(size_t i = 0;i < gEntities.size(); ++i)
        gEntities[i]->ResetInterpolatedPosition();
}

GameScreen::~GameScreen()
{
    ClearResources();
}