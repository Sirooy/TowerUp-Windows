#include "MediaContent.h"

#include <exception>

namespace E2D
{

bool MediaContent::LoadTexture(const std::string& key, const std::string& path)
{
    Texture* texture = new Texture();
    bool result = texture->LoadFromFile(path);

    if(result)
        textures.insert(std::make_pair(key, texture));
    else
    {
        delete texture;
        throw std::runtime_error("No se ha podido cargar la textura: " + path);
    }

    return result;
}

Texture* MediaContent::GetTexture(const std::string& key)
{
    return textures.at(key).get();
}

bool MediaContent::LoadSoundBuffer(const std::string& key, const std::string& path)
{
    SoundBuffer* buffer = new SoundBuffer();
    bool result = buffer->LoadFromFile(path);

    if(result)
        soundBuffers.insert(std::make_pair(key, buffer));
    else
    {
        delete buffer;
        throw std::runtime_error("No se ha podido cargar el buffer de sonido: " + path);
    }

    return result;
}

E2D::SoundBuffer* MediaContent::GetSoundBuffer(const std::string& key)
{
    return soundBuffers.at(key).get();
}

bool MediaContent::LoadMusic(const std::string& key, const std::string& path)
{
    Music* music = new Music();
    bool result = music->OpenFromFile(path);

    if(result)
        songs.insert(std::make_pair(key, music));
    else
    {
        delete music;
        throw std::runtime_error("No se ha podido cargar la musica: " + path);
    }

    return result;
}

E2D::Music* MediaContent::GetMusic(const std::string& key)
{
    return songs.at(key).get();
}

bool MediaContent::LoadFont(const std::string& key, const std::string& path)
{
    Font* font = new Font();
    bool result = font->LoadFromFile(path);

    if(result)
        fonts.insert(std::make_pair(key, font));
    else
    {
        delete font;
        throw std::runtime_error("No se ha podido cargar la fuente: " + path);
    }

    return result;
}

E2D::Font* MediaContent::GetFont(const std::string& key)
{
    return fonts.at(key).get();
}

bool MediaContent::LoadShader(const std::string& key, const std::string& fragment)
{
    sf::Shader* shader = new sf::Shader();
    bool result = shader->loadFromFile(fragment, sf::Shader::Fragment);

    if(result)
        shaders.insert(std::make_pair(key, shader));
    else
    {
        delete shader;
        throw std::runtime_error("No se ha podido cargar el shader con clave: " + key);
    }

    return result;
}

bool MediaContent::LoadShader(const std::string& key, const std::string& vertex, 
    const std::string& fragment)
{
    sf::Shader* shader = new sf::Shader();
    bool result = shader->loadFromFile(vertex, fragment);

    if(result)
        shaders.insert(std::make_pair(key, shader));
    else
    {
        delete shader;
        throw std::runtime_error("No se ha podido cargar el shader con clave: " + key);
    }

    return result;
}

bool MediaContent::LoadShader(const std::string& key, const std::string& vertex, const std::string& geometry, 
    const std::string& fragment)
{
    sf::Shader* shader = new sf::Shader();
    bool result = shader->loadFromFile(vertex, geometry, fragment);

    if(result)
        shaders.insert(std::make_pair(key, shader));
    else
    {
        delete shader;
        throw std::runtime_error("No se ha podido cargar el shader con clave: " + key);
    }

    return result;
}

sf::Shader* MediaContent::GetShader(const std::string& key)
{
    return shaders.at(key).get();
}

bool MediaContent::LoadAnimationFrames(const std::string& path)
{
    bool result = true;
    tinyxml2::XMLDocument document;
    document.LoadFile(path.c_str());

    tinyxml2::XMLElement* animation = document.FirstChildElement("animations")->FirstChildElement("animation");
    while(animation)
    {
        const char* key = animation->Attribute("key");
        if(key)
        {
            std::vector<sf::IntRect>* v = ParseAnimationFrames(animation);
            if(v->size() > 0)
                animationFrames.insert(std::make_pair(key, v));
            else
            {
                delete v;
                throw std::runtime_error("No se ha podido cargar la animacion con clave: [" + std::string(key) + "], no tiene ningun frame.");
            }
        }

        animation = animation->NextSiblingElement("animation");
    }

    return result;
}

std::vector<sf::IntRect>* MediaContent::GetAnimationFrames(const std::string& key)
{
    return animationFrames.at(key).get();
}

std::vector<sf::IntRect>* MediaContent::ParseAnimationFrames(tinyxml2::XMLElement* animation)
{
    std::vector<sf::IntRect>* ret = new std::vector<sf::IntRect>();

    tinyxml2::XMLElement* frame = animation->FirstChildElement("frame");
    while (frame)
    {
        int32_t x = std::stoi(frame->Attribute("x"));
        int32_t y = std::stoi(frame->Attribute("y"));
        int32_t width = std::stoi(frame->Attribute("width"));
        int32_t height = std::stoi(frame->Attribute("height"));
        
        const char* repeatXAtt = frame->Attribute("repeat-x");
        const char* repeatYAtt = frame->Attribute("repeat-y");
        if(repeatXAtt && repeatYAtt)
            ParseFramesRepeatXY(ret, frame, repeatXAtt, repeatYAtt, x, y, width, height);
        else if(repeatXAtt)
            ParseFramesRepeatX(ret, frame, repeatXAtt, x, y, width, height);
        else if(repeatYAtt)
            ParseFramesRepeatY(ret, frame, repeatYAtt, x, y, width, height);
        else
            ret->emplace_back(x, y, width, height);

        frame = frame->NextSiblingElement("frame");
    }

    return ret;
}

void MediaContent::ParseFramesRepeatX(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
    const char* att, int32_t x, int32_t y, int32_t width, int32_t height)
{
    int32_t repeat = std::stoi(att);
    int32_t separation = 0;
    const char* separationAtt = frame->Attribute("separation");
    if(separationAtt)
        separation = std::stoi(separationAtt);
    else
    {
        separationAtt = frame->Attribute("separation-x");
        if(separationAtt)
            separation =  std::stoi(separationAtt);
    }

    int currentX = x;
    for(int32_t i = 0; i < repeat; ++i)
    {
        v->emplace_back(currentX, y, width, height);
        currentX += width + separation;
    }
}

void MediaContent::ParseFramesRepeatY(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
    const char* att, int32_t x, int32_t y, int32_t width, int32_t height)
{
    int32_t repeat = std::stoi(att);
    int32_t separation = 0;
    const char* separationAtt = frame->Attribute("separation");
    if(separationAtt)
        separation = std::stoi(separationAtt);
    else
    {
        separationAtt = frame->Attribute("separation-y");
        if(separationAtt)
            separation =  std::stoi(separationAtt);
    }

    int currentY = y;
    for(int32_t i = 0; i < repeat; ++i)
    {
        v->emplace_back(x, currentY, width, height);
        currentY += height + separation;
    }
}

void MediaContent::ParseFramesRepeatXY(std::vector<sf::IntRect>* v, tinyxml2::XMLElement* frame, 
    const char* attX, const char* attY, int32_t x, int32_t y, int32_t width, int32_t height)
{
    int32_t repeatX = std::stoi(attX);
    int32_t repeatY = std::stoi(attY);
    int32_t separationX = 0;
    int32_t separationY = 0;
    const char* separationXAtt = frame->Attribute("separation-x");
    const char* separationYAtt = frame->Attribute("separation-y");
    const char* direction = frame->Attribute("direction");

    if(separationXAtt)
        separationX = std::stoi(separationXAtt);

    if(separationYAtt)
        separationY = std::stoi(separationYAtt);

    if(direction && std::strcmp(direction, "y") == 0)
    {
        int32_t currentX = x;
        for(int32_t i = 0;i < repeatX; ++i)
        {
            int32_t currentY = y;
            for(int32_t j = 0;j < repeatY; ++j)
            {
                v->emplace_back(currentX, currentY, width, height);
                currentY += height + separationY;
            }
            currentX += width + separationX;
        }
    }
    else
    {
        int32_t currentY = y;
        for(int32_t i = 0;i < repeatY; ++i)
        {
            int32_t currentX = x;
            for(int32_t j = 0;j < repeatX; ++j)
            {
                v->emplace_back(currentX, currentY, width, height);
                currentX += width + separationX;
            }
            currentY += height + separationY;
        }
    }
}

void MediaContent::UnloadResources()
{
    textures.clear();
    soundBuffers.clear();
    songs.clear();
    animationFrames.clear();
}


MediaContent MediaContent::SingletonInstance {}; 

} // namespace E2D
