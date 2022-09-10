#include "AmmoBar.h"
#include "Engine2D/MediaContent.h"
#include "Engine2D/Renderer.h"
#include "Entity/DesertEagle.h"

AmmoBar::AmmoBar() : bottomHolderSprite(), topHolderSprite(), reloadSprite(), bullets(), rects(), reloadAnimation(), 
    texture(nullptr), position(0.0f, 0.0f), currentBulletCount(0), bulletHeight(0), holderHeight(0), isReloading(false)
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    texture = media.GetTexture("Hud");    
    rects = media.GetAnimationFrames("HudAmmoBar");
    bulletHeight = (*rects)[AmmoRectsIndices::Bullet].height;
    width = (*rects)[AmmoRectsIndices::BottomHolder].width;

    reloadAnimation.SetFrames(media.GetAnimationFrames("HudAmmoBarReload"));
    reloadAnimation.SetDuration(0.5f);
    reloadSprite.SetTexture(*texture);
    reloadSprite.setPosition(sf::Vector2f(0.0f, -reloadAnimation.GetFrame(0).height));
    reloadAnimation.Play(reloadSprite);

    holderHeight = (*rects)[AmmoRectsIndices::BottomHolder].height;
    bottomHolderSprite.SetTextureRect((*rects)[AmmoRectsIndices::BottomHolder]);
    bottomHolderSprite.SetTexture(*texture);
    bottomHolderSprite.setPosition(0.0f, -holderHeight);

    topHolderSprite.SetTextureRect((*rects)[AmmoRectsIndices::TopHolder]);
    topHolderSprite.SetTexture(*texture);
    topHolderSprite.setPosition(0.0f, -holderHeight * 2.0f);
}

void AmmoBar::SetPosition(sf::Vector2f position)
{
    this->position = position;

    reloadSprite.setPosition(position - sf::Vector2f(0.0f, reloadAnimation.GetFrame(0).height));
    bottomHolderSprite.setPosition(position - sf::Vector2f(0.0f, holderHeight));
    topHolderSprite.setPosition(position - sf::Vector2f(0.0f, 
        holderHeight * 2.0f + currentBulletCount * bulletHeight + currentBulletCount * BULLET_SEPARATION));

    for(std::size_t i = 0; i < bullets.size(); ++i)
        bullets[i].setPosition(position - sf::Vector2f(0.0f, 
            holderHeight + bulletHeight * (i + 1) + BULLET_SEPARATION * (i + 1)));

    reloadSprite.setPosition(position - sf::Vector2f(0.0f, reloadAnimation.GetFrame(0).height));
}

sf::Vector2f AmmoBar::GetPosition()
{
    return position;
}

float AmmoBar::GetWidth()
{
    return width;
}

void AmmoBar::RemoveOneBullet()
{
    --currentBulletCount;

    SetTopHolderPosition();
}

void AmmoBar::SetCurrentAmmo(int32_t currentAmmo)
{
    currentBulletCount = currentAmmo;

    SetTopHolderPosition();
}

void AmmoBar::SetMaxAmmo(int32_t maxAmmo)
{
    int32_t numBullets = maxAmmo;
    int32_t diff = numBullets - static_cast<int32_t>(bullets.size()); //Nuevas balas menos las que tenia

    if(diff > 0) //Añadir balas
    {
        //Calculamos la proxima posicion a partir del número de balas que hay en el vector
        int32_t size = static_cast<int32_t>(bullets.size());
        sf::Vector2f nextPosition = position - sf::Vector2f(0.0f, 
            holderHeight + bulletHeight * (size + 1) + BULLET_SEPARATION * (size + 1)); 

        while(diff > 0)
        {
            E2D::Sprite bullet;
            bullet.setPosition(nextPosition);
            bullet.SetTexture(*texture);
            bullet.SetTextureRect((*rects)[AmmoRectsIndices::Bullet]);
            bullets.push_back(bullet);
            nextPosition.y -= bulletHeight - BULLET_SEPARATION;
            --diff;
        }
    }
    else if(diff < 0) //Borrar balas
    {
        while(diff < 0)
        {
            bullets.pop_back();
            ++diff;
        }
    }
}

void AmmoBar::Update(float dt)
{
    if(isReloading)
    {
        reloadAnimation.Update(reloadSprite, dt);
    }
}

void AmmoBar::SetReloading(bool value)
{
    isReloading = value;

    if(value)
    {
        reloadAnimation.Play(reloadSprite);
    }
}

void AmmoBar::Draw(E2D::Renderer& renderer)
{
    if(isReloading)
    {
        renderer.Draw(reloadSprite);
    }
    else
    {
        renderer.Draw(bottomHolderSprite);
        renderer.Draw(topHolderSprite);

        for(int32_t i = 0;i < currentBulletCount; ++i)
        {
            renderer.Draw(bullets[i]);
        }
    }
}