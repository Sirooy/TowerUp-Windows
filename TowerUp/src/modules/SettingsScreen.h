#pragma once

#include "Screen.h"
#include "Engine2D/Sprite.h"
#include "Engine2D/Texture.h"
#include "Engine2D/Text.h"
#include "Button.h"
#include "Checkbox.h"
#include "MainMenuScreen.h"

namespace E2D
{

class MediaContent;
class Renderer;

}

class SettingsScreen : public Screen, public ScreenIDGenerator<SettingsScreen>
{
public:
    SettingsScreen(Game& game);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;

    virtual ~SettingsScreen();
protected:
    E2D::MediaContent& media;
    E2D::Renderer& renderer;

    Checkbox check {}, cheats{};
    Button back {};
    E2D::Text controls {};
    E2D::Text volver {};
    E2D::Text titulo {};
    E2D::Text tituloControles {};

    E2D::Font font {};

    E2D::Texture* texturebackGround {};
    E2D::Texture* texturebackGroundSettings {};

    E2D::Sprite backGround {};
    E2D::Sprite backGroundSettings {};
};
