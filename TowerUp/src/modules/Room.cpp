#include "Room.h"
#include "Curse.h"

Room::Room() : tileMap(), collisionLayer(), curses(), size(0.0f, 0.0f), widthInTiles(0), heightInTiles(0), type(RoomType::Default)
{

}

void Room::ApplyCurses()
{
    for(auto c : curses)
        c->ApplyDebuff();
}

void Room::RemoveCurses()
{
    for(auto c : curses)
        c->RemoveDebuff();
}

void Room::UpdateTorches(float dt)
{
    for(std::size_t i = 0;i < torches.size(); ++i)
        torches[i].Update(dt);
}

void Room::Draw(E2D::TileMap::LayerType type, const sf::RenderStates& states)
{
    tileMap.Draw(type, states);
}

void Room::DrawTorches()
{
    for(std::size_t i = 0;i < torches.size(); ++i)
        torches[i].Draw();
}