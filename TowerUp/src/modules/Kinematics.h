#pragma once

#include "Screen.h"
#include "tinyxml2.h"
#include "MainMenuScreen.h"
#include "Engine2D/Texture.h"
#include "Engine2D/Sprite.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Engine2D/Sound.h"

class Kinematics : public Screen
{
public:
    
    int32_t MARGIN = 64;
    static constexpr int32_t SPEED = 20;
    static constexpr float MARGIN_TOP = 0.8445;

    Kinematics(Game& game, std::string path);

    void OnEnter() override;
    void OnExit() override;
    void HandleEvent(const sf::Event& e) override;
    void Update(float dt) override;
    void Draw(float tickPercent) override;
    void Next();
    void NextNode();
    void WriteAll();
    void CheckTextOutOfWindowRight();
    bool CheckTextOutOfWindowDown();

    virtual ~Kinematics();
protected:

    std::string  folderPath;
    E2D::MediaContent& media;
    tinyxml2::XMLDocument Doc;
    tinyxml2::XMLElement* img;
    tinyxml2::XMLElement* p;
    E2D::Sound currentSound;
    std::string currentText;
    std::string renderText;
    E2D::Sprite currentImg;
    E2D::Text text; //cambiar a e2D
    E2D::Font font;
    sf::Clock clock;
    unsigned int charCont;
    float xFactor;//factores de escalado de la pantalla
    float yFactor;

};