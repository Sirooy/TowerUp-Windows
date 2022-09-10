#pragma once

#include <vector>
#include "Engine2D/Sprite.h"
#include "Animation.h"

class Player;
class DesertEagle;

class AmmoBar
{
public:
    AmmoBar();

    void RemoveOneBullet();
    void SetCurrentAmmo(int32_t currentAmmo);
    void SetMaxAmmo(int32_t maxAmmo);
    void SetReloading(bool value);
    void Update(float dt);
    void Draw(E2D::Renderer& renderer);

    void SetPosition(sf::Vector2f position);
    sf::Vector2f GetPosition();
    float GetWidth();
    
private:
    static constexpr float BULLET_SEPARATION = 0.0f;

    enum AmmoRectsIndices
    {
        BottomHolder = 0,
        TopHolder = 1,
        Bullet = 2
    };

    E2D::Sprite bottomHolderSprite;
    E2D::Sprite topHolderSprite;
    E2D::Sprite reloadSprite;
    std::vector<E2D::Sprite> bullets;
    std::vector<sf::IntRect>* rects;
    Animation reloadAnimation;
    E2D::Texture* texture;
    sf::Vector2f position;
    int32_t currentBulletCount; 
    int32_t bulletHeight;
    int32_t holderHeight;
    float width;
    bool isReloading;

    inline void SetTopHolderPosition()
    {
        topHolderSprite.setPosition(position - sf::Vector2f(0.0f, 
            holderHeight * 2.0f + currentBulletCount * bulletHeight + currentBulletCount * BULLET_SEPARATION));
    }
};
