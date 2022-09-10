#pragma once

#include "Screen.h"
#include "Engine2D/Renderer.h"
#include "LevelGenerator.h"
#include "Engine2D/Camera.h"

class Hud;
class World;
class PhysicsEngine;
class PathFinding;
class Media;
class Player;

class GameScreen : public Screen, public ScreenIDGenerator<GameScreen>
{
public:
    GameScreen(Game& game);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

    void SetCameraLimits();
    void UpdateEntities(float dt);
    void CenterCameraOnPlayer(float tickPercent);
    void DrawTileMapAndEntities(float tickPercent);
    void DrawTileMapAndEntitiesCentered(float tickPercent);
    void ResetEntityInterpolatedPositions();

    virtual ~GameScreen();
protected:
    friend class GameScreenPlaying;
    friend class GameScreenPaused;
    friend class GameScreenEnterDoor;
    friend class GameScreenExitDoor;
    friend class GameScreenPlayerDead;

    World& world;
    PhysicsEngine& physics;
    PathFinding& pathfinding;
    E2D::Renderer& renderer;
    E2D::MediaContent& media;
    LevelGenerator level;
    E2D::BloomFilter bloomFilter{};
    E2D::Camera camera{};
    Hud* hud {};
    Player* player {};
    E2D::Music* music {};
    sf::Shader* foregroundShader {};

    void RemoveDeadEntities();
    void LoadTextures();
    void LoadSounds();
    void LoadAnimations();
    void ClearResources();
};
