#include "Kinematics.h"

#include "Game.h"
#include "Entity/Player.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Font.h"
#include "Engine2D/Text.h"

#include <iostream>

Kinematics::Kinematics(Game& game, std::string path) : Screen(game), media(E2D::MediaContent::Instance())
{
    clock.restart();
    renderText="";
    charCont = 0;
    folderPath = path;
}

void Kinematics::OnEnter()
{

    xFactor = E2D::Renderer::Instance().GetSize().x/1280;
    yFactor = E2D::Renderer::Instance().GetSize().y/720;

    Kinematics::MARGIN = Kinematics::MARGIN * yFactor;

    //Cargar fuente
    E2D::Renderer &render = E2D::Renderer::Instance();
    font.LoadFromFile("resources/fonts/Modern.ttf");
    text.SetFont(font);
    text.SetCharacterSize(28);
    text.SetFillColor(sf::Color::Black);
    
    text.SetStyle(sf::Text::Bold);
    text.SetScale(xFactor,yFactor);

    sf::Vector2f position (Kinematics::MARGIN, render.GetSize().y*Kinematics::MARGIN_TOP);
    text.SetPosition(position);
    
    Doc.LoadFile((folderPath+"kinematic.xml").c_str());
    img = Doc.FirstChildElement("img");

    while(img)
    {
        std::string textureName = img->Attribute("source");
        std::string soundName = img->Attribute("sound");
        media.LoadSoundBuffer(soundName, "resources/sounds/kinematics/"+soundName+".wav"); 
        media.LoadTexture(textureName, folderPath+textureName+".png");
        img = img->NextSiblingElement("img");
    }

    //cargar la primera imagen y texto
    img = Doc.FirstChildElement("img");
    if(img)
    {
        std::string imgKey = img->Attribute("source");
        currentImg.SetTexture(*media.GetTexture(imgKey), true);
        currentImg.SetScale(xFactor,yFactor);

        std::string soundKey = img->Attribute("sound");
        currentSound.SetBuffer(*media.GetSoundBuffer(soundKey));
        currentSound.Play();
    }
        

    p = img->FirstChildElement("p");
    if(p)
        currentText = p->GetText();

}

void Kinematics::OnExit()
{
    media.UnloadResources();
}

void Kinematics::HandleEvent(const sf::Event& e)
{
    if(e.type == sf::Event::KeyPressed)
    {
        if(e.key.code == sf::Keyboard::Enter) 
            Next();
        else if(e.key.code == sf::Keyboard::Escape) //implementamos que si pulsamos tecla esc saltará directamente al menú
            game.ChangeScreen(MainMenuScreen::GetID());
    }
}

void Kinematics::Next()
{
    if(charCont == currentText.size()) //si hemos acabado con el nodo de texto pasamos al siguiente
    {
        NextNode(); 
    }
    else
    {
        if(!CheckTextOutOfWindowDown())//si el texto no se ha salido por abajo quiere decir que el jugador ha decidido pasarlo
        {   
            WriteAll();
        }
        else //el texto se ha pasado por abajo por lo que se limpia la pantalla y se continua con lo que queda del nodo
        {
            renderText = "";
            text.SetString("");
        }
    }
}

void Kinematics::WriteAll()//se escribe todo lo que quepa en la pantalla
{
    bool out = false;

    renderText.erase(renderText.size()-1); //hay que borrar la _ porque se ha quedado escrita al darle al enter

    while(charCont < currentText.size() && !out) //pintamos por pantalla hasta que se sale por abajo o se acaba el texto
    {
        renderText = renderText + currentText[charCont];

        if(!CheckTextOutOfWindowDown()) 
        {
            CheckTextOutOfWindowRight();
            charCont++;
            text.SetString(renderText);
        }
        else
        {
            out = true;
        }
    }
}

void Kinematics::NextNode()
{
    charCont = 0;
    text.SetString("");
    renderText="";
    currentText="";

    p = p->NextSiblingElement("p");
    
    if(p)
    {
        currentText = p->GetText();
    }
    else
    {
        img = img->NextSiblingElement("img");   

        if(img)
        {
            p = img->FirstChildElement("p");
            if(p)
                currentText = p->GetText();
            
            std::string key = img->Attribute("source");
            currentImg.SetTexture(*media.GetTexture(key));
            currentImg.SetScale(xFactor,yFactor);

            std::string soundKey = img->Attribute("sound");
            currentSound.SetBuffer(*media.GetSoundBuffer(soundKey));
            currentSound.Play();
        }
        else
        {
            game.ChangeScreen(MainMenuScreen::GetID());
        }  
    } 
}

void Kinematics::CheckTextOutOfWindowRight()
{
    //si estamos en un espacio comprobamos si la ultima letra de la siguiente  
    //palabra se sale de la pantalla y si lo hace anadimos unsalto de linea
    if(currentText[charCont] == ' ')
    {
        std::string renderTextCopy = renderText;
        E2D::Text textCopy = text;
        unsigned int cont = charCont + 1;
        
        renderTextCopy = renderTextCopy + currentText[cont];

        while(currentText.size()>cont+1 && currentText[cont+1] != ' ')
        {
            renderTextCopy = renderTextCopy + currentText[cont];
            cont++;
        }
        
        textCopy.SetString(renderTextCopy);

        sf::Vector2f positionRight = textCopy.FindCharacterPos(renderTextCopy.size()-1);

        if(positionRight.x >  E2D::Renderer::Instance().GetSize().x-Kinematics::MARGIN)
        {
            renderText.erase(renderText.size()-1);//borra la _
            renderText = renderText + "\n";
        }
    }
}

bool Kinematics::CheckTextOutOfWindowDown()//devuelve true si el texto se sale por abajo
{
    bool out = false;
    sf::Vector2f positionBot = text.FindCharacterPos(renderText.size()-1);
                
    if(positionBot.y > E2D::Renderer::Instance().GetSize().y-Kinematics::MARGIN)
    {
        out = true;
       
    }

    return out;
}

void Kinematics::Update([[maybe_unused]] float dt)
{
    float elapsedTime = clock.getElapsedTime().asMilliseconds();
   
    if(elapsedTime>Kinematics::SPEED && charCont!=currentText.size())//si ha pasado el tiempo se pinta la siguiente letra
    {
        
        clock.restart();

        if(renderText.size()>=1 && renderText[renderText.size()-1] != '\n')
        {
            renderText.erase(renderText.size()-1); 
        }

        renderText = renderText + currentText[charCont] + "_";

        if(!CheckTextOutOfWindowDown())
        {
            
            CheckTextOutOfWindowRight();
            text.SetString(renderText);
            charCont++;
        }  
    }
}

void Kinematics::Draw([[maybe_unused]] float tickPercent)
{
    //hud->Draw();
    E2D::Renderer &render = E2D::Renderer::Instance();
    render.Clear(sf::Color::Cyan);
    render.Draw(currentImg);
    render.Draw(text);
    render.Display();
}

Kinematics::~Kinematics()
{

}