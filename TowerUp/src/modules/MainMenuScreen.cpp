#include "MainMenuScreen.h"
#include "Game.h"
#include "GameScreen.h"
#include "SettingsScreen.h"

#include <iostream>

MainMenuScreen::MainMenuScreen(Game& game) : Screen(game)
{

}

void MainMenuScreen::OnEnter()
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();
//Cargamos background
    media.LoadTexture("MenuBackground","./resources/sprites/Menu/menuBackground2.png");
    texturebackGround = media.GetTexture("MenuBackground");

    float scaleFactorX = screenSize.x / texturebackGround->GetSize().x;
    float scaleFactorY = screenSize.y / texturebackGround->GetSize().y;
    backGround.SetTexture(*texturebackGround);
    backGround.setScale(scaleFactorX, scaleFactorY);

//Cargamos el titulo
    media.LoadFont("Fuente","./resources/fonts/Modern.ttf");

    title.SetFont(*media.GetFont("Fuente"));
    title.SetCharacterSize(130);
    title.SetFillColor(sf::Color::White);
    title.SetOutlineColor(sf::Color::Black);
    title.SetOutlineThickness(4);
    title.SetStyle(sf::Text::Bold);
    title.SetPosition(screenSize.x * 0.5, screenSize.y * 0.1f);
    title.SetString("Tower up");
    sf::FloatRect bounds = title.GetGlobalBounds();
    title.SetOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
   
//Cargamos los botones
    media.LoadTexture("boton","./resources/sprites/Menu/texturas-menu.png");

    play.SetTexture(media.GetTexture("boton"));
    play.SetDefaultRect(sf::IntRect(0, 80, 176, 48));
    play.SetHoverRect(sf::IntRect(0, 32, 176, 48));
    play.SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    play.SetTextFont(media.GetFont("Fuente"));
    play.SetTextCharacterSize(32);
    play.SetText("Jugar");
    play.SetTextColor(sf::Color::White);
    play.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.4f);
    
    settings.SetTexture(media.GetTexture("boton"));
    settings.SetDefaultRect(sf::IntRect(0, 80, 176, 48));
    settings.SetHoverRect(sf::IntRect(0, 32, 176, 48));
    settings.SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    settings.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.55f);
    settings.SetTextFont(media.GetFont("Fuente"));
    settings.SetText("Ajustes");
    settings.SetTextCharacterSize(32);
    settings.SetTextColor(sf::Color::White);
    
    back.SetTexture(media.GetTexture("boton"));
    back.SetDefaultRect(sf::IntRect(0, 80, 176, 48));
    back.SetHoverRect(sf::IntRect(0, 32, 176, 48));
    back.SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    back.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.75f);
    back.SetTextFont(media.GetFont("Fuente"));
    back.SetText("Salir");
    back.SetTextCharacterSize(32);
    back.SetTextColor(sf::Color::White);
}

void MainMenuScreen::OnExit()
{
    media.UnloadResources();
}

void MainMenuScreen::HandleEvent([[maybe_unused]] const sf::Event& e)
{
    play.Update(e);
    settings.Update(e);
    back.Update(e);

    if(play.IsClicked())
    {
        game.ChangeScreen(GameScreen::GetID());
    }

    if(settings.IsClicked())
    { 
        game.ChangeScreen(SettingsScreen::GetID());
    }

    if(back.IsClicked())
    { 
        window.Close();
    }
}

void MainMenuScreen::Update([[maybe_unused]] float dt)
{
    
}

void MainMenuScreen::Draw([[maybe_unused]] float tickPercent)
{
    window.Clear(sf::Color::Black);
    
    window.Draw(backGround);
    window.Draw(title);
    play.Draw();
    settings.Draw();
    back.Draw();

    window.Display();
}
    


MainMenuScreen::~MainMenuScreen()
{

}