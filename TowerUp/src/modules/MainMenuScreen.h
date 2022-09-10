#pragma once

#include "Screen.h"
#include "Engine2D/Renderer.h"
#include "Engine2D/Sprite.h"
#include "Engine2D/Texture.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Text.h"
#include "Button.h"

class MainMenuScreen : public Screen, public ScreenIDGenerator<MainMenuScreen>
{
private:
    

public:
    MainMenuScreen(Game& game);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;


    virtual ~MainMenuScreen();
protected:
    static constexpr float BUTTON_WIDTH = 300.0f;
    static constexpr float BUTTON_HEIGHT = 100.0f;

    Button play;
    Button settings;
    Button back;
    

    E2D::MediaContent& media = E2D::MediaContent::Instance();

    E2D::Texture* texturebackGround;
    E2D::Texture* textureTitle;
    
        
    E2D::Sprite backGround;
   
    E2D::Text title;
    E2D::Font font;

    E2D::Renderer& window = E2D::Renderer::Instance();




};
