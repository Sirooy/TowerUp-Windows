#include "Heart.h"
#include "Player.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/CollisionData.h"


Heart::Heart(sf::Vector2f position, int32_t amountOfHealing, sf::Color color) : 
    Consumable(position, WIDTH, HEIGHT, MAX_ALIVE_TIME), amountOfHealing(amountOfHealing)
{
    sprite.SetColor(color);

    auto& def = animations[AnimationIndex::DefaultIndex];
    def.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames("HeartDefault"));
    def.SetDuration(0.5f);

    auto& pickedUp = animations[AnimationIndex::PickedUpIndex];
    pickedUp.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames("HeartPickedUp"));
    pickedUp.SetDuration(0.1f);
    pickedUp.SetLoop(false);

    auto& frame = def.GetFrame(0);
    float scaleFactor = SPRITE_SIZE / frame.width;
    sprite.setOrigin(frame.width * 0.5f, frame.height * 0.5f);
    sprite.setScale(scaleFactor, scaleFactor);

    animations.ChangeAnimation(sprite, AnimationIndex::DefaultIndex);
    sound.SetBuffer(*E2D::MediaContent::Instance().GetSoundBuffer("PickHeart"));
    sound.SetRelativeToListener(true); 
    sound.SetVolume(35.0f);
}

void Heart::OnCollision(Player* player, const CollisionData& data)
{
    
    Consumable::OnCollision(player, data);

    player->AddCurrentHealth(amountOfHealing);
    PlaySound(sound);
    
}

void Heart::OnHorizontalTileCollision(const TileCollisionData& data)
{
    Position += data.Normal * data.Penetration;
    Velocity.x = -Velocity.x;
}