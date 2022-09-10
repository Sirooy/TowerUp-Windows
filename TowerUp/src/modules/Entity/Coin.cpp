#include "Coin.h"
#include "Player.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/CollisionData.h"


Coin::Coin(sf::Vector2f position, int32_t value, sf::Color color) : 
    Consumable(position, WIDTH, HEIGHT, MAX_ALIVE_TIME), value(value)
{
    sprite.SetColor(color);

    auto& def = animations[AnimationIndex::DefaultIndex];
    def.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames("CoinDefault"));
    def.SetDuration(0.5f);

    auto& pickedUp = animations[AnimationIndex::PickedUpIndex];
    pickedUp.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames("CoinPickedUp"));
    pickedUp.SetDuration(0.1f);
    pickedUp.SetLoop(false);

    auto& frame = def.GetFrame(0);
    float scaleFactor = SPRITE_SIZE / frame.width;
    sprite.setOrigin(frame.width * 0.5f, frame.height * 0.5f);
    sprite.setScale(scaleFactor, scaleFactor);

    animations.ChangeAnimation(sprite, AnimationIndex::DefaultIndex);
    sound.SetBuffer(*E2D::MediaContent::Instance().GetSoundBuffer("PickCoin"));
    sound.SetRelativeToListener(true);
    sound.SetVolume(35.0f);
}

void Coin::OnCollision(Player* player, const CollisionData& data)
{
    Consumable::OnCollision(player, data);

    player->AddMoney(value);
    PlaySound(sound);
}

void Coin::OnVerticalTileCollision(const TileCollisionData& data)
{
    Position += data.Normal * data.Penetration;
    Velocity.y = -Velocity.y * 0.8f;
}

void Coin::OnHorizontalTileCollision(const TileCollisionData& data)
{
    Position += data.Normal * data.Penetration;
    Velocity.x = -Velocity.x;
}
