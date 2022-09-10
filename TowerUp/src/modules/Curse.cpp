#include "Curse.h"
#include "Entity/Player.h"
#include "Entity/World.h"
#include "Engine2D/Renderer.h"

//! BASE CURSE
Curse::Curse(int32_t id) : textureRect(), id(id)
{
    textureRect = sf::IntRect
    (
        sf::Vector2i( id % TEXTURE_ICONS_PER_ROW * SPRITE_SIZE, id / TEXTURE_ICONS_PER_ROW * SPRITE_SIZE ),
        sf::Vector2i(SPRITE_SIZE, SPRITE_SIZE)
    );
}

//! VISUAL CURSE
VisualCurse::VisualCurse(E2D::FilterType filterType) : Curse(static_cast<int32_t>(filterType)), filterType(filterType) 
 { }

void VisualCurse::ApplyDebuff()
{
    E2D::Renderer::Instance().EnableFilter(filterType);
}

void VisualCurse::RemoveDebuff()
{
    E2D::Renderer::Instance().DisableFilter(filterType);
}

//############################################################
DoubleLifeEnemiesCurse::DoubleLifeEnemiesCurse() : Curse(7){}

void DoubleLifeEnemiesCurse::ApplyDebuff()
{
    DesertEagle& desertEagle = World::Instance().GetPlayer()->GetDeagle();
    removedDamage = desertEagle.GetBulletDamage()/2;
    desertEagle.RemoveBulletDamage(removedDamage);
}

void DoubleLifeEnemiesCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->GetDeagle().AddBulletDamage(removedDamage);
}

//############################################################
OneHeartCurse::OneHeartCurse() : Curse(8){}

void OneHeartCurse::ApplyDebuff()
{
    Player& player = *World::Instance().GetPlayer();
    lastCurrentHealth = player.GetCurrentHealth();
    lastMaxHealth = player.GetMaxHealth();

    if(lastCurrentHealth>2)
        player.SetCurrentHealth(2);

    player.SetMaxHealth(2);
}

void OneHeartCurse::RemoveDebuff()
{
    Player& player = *World::Instance().GetPlayer();
    
    player.AddMaxHealth(lastMaxHealth-2);

    if(lastCurrentHealth>2)
        player.AddCurrentHealth(lastCurrentHealth-2);
}

//############################################################
OneBulletCurse::OneBulletCurse() : Curse(9){}

void OneBulletCurse::ApplyDebuff()
{
    DesertEagle& desertEagle = World::Instance().GetPlayer()->GetDeagle();
    lastMaxAmmo = desertEagle.GetMaxAmmo();
    desertEagle.SetMaxAmmo(1);//Falta un setAmmo a 1??
}

void OneBulletCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->GetDeagle().SetMaxAmmo(lastMaxAmmo);
}

//############################################################
MaxDashCooldownCurse::MaxDashCooldownCurse() : Curse(10){}

void MaxDashCooldownCurse::ApplyDebuff()
{
    Player& player = *World::Instance().GetPlayer();
    lastDashCooldown = player.GetDashCooldown();
    player.SetDashCooldown(5.0f);
}

void MaxDashCooldownCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->SetDashCooldown(lastDashCooldown);
}


//############################################################
HalfMoneyCurse::HalfMoneyCurse() : Curse(11){}

void HalfMoneyCurse::ApplyDebuff()
{
    Player& player = *World::Instance().GetPlayer();
    player.SetMoney(player.GetMoney()/2);
}

void HalfMoneyCurse::RemoveDebuff(){}

//############################################################
MinMoveSpeedCurse::MinMoveSpeedCurse() : Curse(12){}

void MinMoveSpeedCurse::ApplyDebuff()
{
    Player& player = *World::Instance().GetPlayer();
    lastMoveSpeed = player.GetMoveSpeed();
    player.SetMoveSpeed(1000.0f);
}

void MinMoveSpeedCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->AddMoveSpeed(lastMoveSpeed-1000.0f);
}

//############################################################
MoreRecoilCurse::MoreRecoilCurse() : Curse(13){}

void MoreRecoilCurse::ApplyDebuff()
{
    DesertEagle& desertEagle = World::Instance().GetPlayer()->GetDeagle();
    float currentRecoil = desertEagle.GetRecoil();
    desertEagle.AddRecoil(300.0f);
    addedRecoil = desertEagle.GetRecoil()-currentRecoil;
}

void MoreRecoilCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->GetDeagle().RemoveRecoil(addedRecoil);
}

//############################################################
MoreReloadTimeCurse::MoreReloadTimeCurse() : Curse(14){}

void MoreReloadTimeCurse::ApplyDebuff()
{
    DesertEagle& desertEagle = World::Instance().GetPlayer()->GetDeagle();
    float currentReloadTime = desertEagle.GetReloadTime();
    desertEagle.AddReloadTime(2.0f);
    addedReloadTime = desertEagle.GetReloadTime()-currentReloadTime;
}

void MoreReloadTimeCurse::RemoveDebuff()
{
    World::Instance().GetPlayer()->GetDeagle().RemoveReloadTime(addedReloadTime);
}
