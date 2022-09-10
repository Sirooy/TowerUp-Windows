#include "SettingsScreen.h"

#include "Game.h"
#include "GameScreen.h"
#include "SettingsScreen.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Entity/World.h"

SettingsScreen::SettingsScreen(Game& game) : Screen(game), media(E2D::MediaContent::Instance()), renderer(E2D::Renderer::Instance())
{
    
}

void SettingsScreen::OnEnter()
{
    sf::Vector2f screenSize = E2D::Renderer::Instance().GetSize();

   //Cargamos el background
   media.LoadTexture("MenuBackground","./resources/sprites/Menu/menuBackground2.png");
   texturebackGround = media.GetTexture("MenuBackground");
   float scaleFactorX = screenSize.x / texturebackGround->GetSize().x;
   float scaleFactorY = screenSize.y / texturebackGround->GetSize().y;
   backGround.SetTexture(*texturebackGround);
   backGround.setScale(scaleFactorX, scaleFactorY);

   //Cargamos las texturas del menu

   media.LoadTexture("textura","./resources/sprites/Menu/texturas-menu.png");

   //Cargamos el background de settings

   texturebackGroundSettings = media.GetTexture("textura");
   backGroundSettings.SetTexture(*texturebackGroundSettings);
   backGroundSettings.SetTextureRect(sf::IntRect(176, 0, 144, 128));
   backGroundSettings.setScale(scaleFactorX * 7.5f, scaleFactorY * 4.5f);
   backGroundSettings.setPosition(screenSize.x * 0.5f, screenSize.y * 0.55f);
   backGroundSettings.setOrigin(144 * 0.5f, 128 * 0.5f);

   //Cargamos el boton Back

   back.SetDefaultRect(sf::IntRect(64, 0, 32, 32 ));
   back.SetHoverRect(sf::IntRect(96, 0, 32, 32 ));
   back.SetSize(75.f, 75.f);
   back.SetPosition(325.f, 535.f);

   font.LoadFromFile("./resources/fonts/Modern.ttf");
   volver.SetFont(font);
   volver.SetCharacterSize(24);
   volver.SetFillColor(sf::Color::White);
   volver.SetStyle(sf::Text::Bold);
   volver.SetPosition(290.f, 570.f);
   volver.SetString("Volver");

   
   back.SetTexture(media.GetTexture("textura"));

    //Cargamos titulo de Ajustes
    titulo.SetFont(font);
    titulo.SetCharacterSize(90);
    titulo.SetFillColor(sf::Color::White);
    titulo.SetStyle(sf::Text::Bold);
    titulo.SetString("Ajustes");  
    sf::FloatRect bounds = titulo.GetLocalBounds();
    titulo.SetOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    titulo.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.05f);

    //Cargamos el checkbox Silenciar

    check.SetDefaultRect(sf::IntRect(0, 0, 32, 32));
    check.SetActiveRect(sf::IntRect(32, 0, 32, 32));
    check.SetSize(75.f, 75.f);
    check.SetPosition(625.f, 540.f);
    
    check.SetTexture(media.GetTexture("textura"));
    media.LoadFont("Fuente","./resources/fonts/Modern.ttf");
    check.SetTextFont(media.GetFont("Fuente"));
    check.SetText("Silenciar");
    check.SetTextCharacterSize(24);
    check.SetTextColor(sf::Color::White);
    check.SetPositionText(625.f, 575.f);

    //Cargamos el checkbox Trucos

    cheats.SetDefaultRect(sf::IntRect(0, 0, 32, 32));
    cheats.SetActiveRect(sf::IntRect(32, 0, 32, 32));
    cheats.SetSize(75.f, 75.f);
    cheats.SetPosition(825.f, 540.f);
    
    cheats.SetTexture(media.GetTexture("textura"));
    cheats.SetTextFont(media.GetFont("Fuente"));
    cheats.SetText("Trucos");
    cheats.SetTextCharacterSize(24);
    cheats.SetTextColor(sf::Color::White);
    cheats.SetPositionText(825.f, 575.f);

   //Cargamos el texto controles
   
    tituloControles.SetFont(font);
    tituloControles.SetCharacterSize(30);
    tituloControles.SetFillColor(sf::Color::White);
    tituloControles.SetStyle(sf::Text::Bold);
    tituloControles.SetPosition(325.f, 275.f);
    tituloControles.SetString("Controles:");

    controls.SetFont(font);
    controls.SetCharacterSize(24);
    controls.SetFillColor(sf::Color::White);
    controls.SetStyle(sf::Text::Bold);
    controls.SetPosition(550.f, 190.f);
    controls.SetString(" -A: Moverse hacia la izquierda \n -D: Moverse hacia la derecha \n -Space: Saltar \n -R: Recargar \n -E: Accion \n -Shift: Dash  \n -Click izquierdo: Disparar \n -Movimiento del raton: Apuntar \n");

}

void SettingsScreen::OnExit()
{
    media.UnloadResources();
}

void SettingsScreen::HandleEvent([[maybe_unused]] const sf::Event& e)
{
    back.Update(e);
    check.Update(e);
    cheats.Update(e);

    if(back.IsClicked())
        game.ChangeScreen(MainMenuScreen::GetID());

    if(check.IsClicked())
    { 
        if(check.IsActive()==true)
        {
            E2D::Listener::SetGlobalVolume(0.0f);
        }else
        {
            E2D::Listener::SetGlobalVolume(100.0f);
        }
        
    }

    if(cheats.IsClicked())
    { 
        World::Instance().SetCheatsOn(cheats.IsActive());
    }
}

void SettingsScreen::Update([[maybe_unused]] float dt)
{
    
}

void SettingsScreen::Draw([[maybe_unused]] float tickPercent)
{
    renderer.Clear(sf::Color::Black);
    
    renderer.Draw(backGround);
    renderer.Draw(backGroundSettings);    
    renderer.Draw(titulo);
    renderer.Draw(tituloControles);
    back.Draw();
    renderer.Draw(volver);
    check.Draw();
    cheats.Draw();
    renderer.Draw(controls);

    renderer.Display();
}

SettingsScreen::~SettingsScreen()
{

}