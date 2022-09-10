#include "DesertEagle.h"
#include "../Engine2D/MediaContent.h"
#include "../Engine2D/Renderer.h"
#include "../Engine2D/LightSystem.h"
#include "../Hud.h"
#include "Player.h"
#include "CollidableEntity.h"
#include "Bullet.h"
#include "../SFMLUtils.hpp"
#include "../Random.h"
#include "World.h"

DesertEagle::DesertEagle()
{
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Player"));
    sprite.setOrigin(12.0f, 34.0f);
    gunBarrelOffset = sf::Vector2f(44.0f - 12.0f, 536.0f - 546.0f); //Vector que va desde el origen hasta el cañon de la pistola

    CreateBulletGenerator();
    InitAnimations();
    CreateSounds();

    hud = World::Instance().GetHud();
    hud->GetAmmoBar().SetMaxAmmo(shootMode->MaxAmmo);
    hud->GetAmmoBar().SetCurrentAmmo(shootMode->CurrentAmmo);
}

void DesertEagle::InitAnimations()
{
    auto& media = E2D::MediaContent::Instance();

    Animation& shootAnimation = animations[AnimationIndex::ShootIndex];
    shootAnimation.SetFrames(media.GetAnimationFrames("PlayerShoot"));
    shootAnimation.SetDuration(0.1f);
    shootAnimation.SetLoop(false);
    shootAnimation.SetCurrentFrameIndex(shootAnimation.GetNumFrames() - 1, sprite);

    Animation& reloadAnimation = animations[AnimationIndex::ReloadIndex];
    reloadAnimation.SetFrames(media.GetAnimationFrames("PlayerReload"));
    reloadAnimation.SetDuration(0.0f);
    reloadAnimation.SetLoop(false);
}

void DesertEagle::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& shoot = sounds[SoundIndex::ShootSound];
    shoot.SetBuffer(*media.GetSoundBuffer("DesertEagleShoot"));
    shoot.SetRelativeToListener(true); 
    shoot.SetVolume(6.0f);

    auto& reload = sounds[SoundIndex::ReloadSound];
    reload.SetBuffer(*media.GetSoundBuffer("DesertEagleReload"));
    reload.SetRelativeToListener(true); 
    reload.SetVolume(100.0f);
}


void DesertEagle::CreateBulletGenerator()
{
    auto& media = E2D::MediaContent::Instance();

    generator.BulletTexture = media.GetTexture("Bullets");
    generator.BulletAnimationFrames[0] = media.GetAnimationFrames("DesertEagleBulletAlive");
    generator.BulletAnimationFrames[1] = media.GetAnimationFrames("DesertEagleBulletDead");
    generator.BulletAnimationDurations[0] = 0.5f;
    generator.BulletAnimationDurations[1] = 0.5f;
    generator.BulletCollisionType = CollisionType::PlayerBulletCollisionType;
    generator.BulletCollisionMask = CollisionType::EnemyCollisionType | CollisionType::TileCollisionType;
    generator.BulletSpeed = 550.0f; //550.0f
    generator.BulletDamage = 20.0f; //15
    generator.BulletSize = 12.0f;
    generator.BulletKnockBack = 150.0f;
    generator.BulletMaxAliveTime = 4.0f;
    generator.BulletType = BulletType::Default;
    generator.BulletPlayerDamage = 1;
    generator.BulletBaseAcceleration = sf::Vector2f(0.0f, 0.0f);
    generator.BulletTexturePadding = 3.0f;

    shootMode = new SingleShotMode();
    shootMode->MaxAmmo  = 6;
    shootMode->CurrentAmmo = 6;
    shootMode->AttackSpeed = 0.3f;
    shootMode->ReloadTime = 1.0f;
}

void DesertEagle::Update(float dt)
{
    shootMode->Update(dt);
    animations.CurrentAnimation().Update(sprite, dt);

    if(shootMode->JustReloaded())
    {
        hud->GetAmmoBar().SetReloading(false);
        hud->GetAmmoBar().SetCurrentAmmo(shootMode->CurrentAmmo);
        animations[AnimationIndex::ShootIndex].SetCurrentFrameIndex
            (animations[AnimationIndex::ShootIndex].GetNumFrames() - 1, sprite);
    }
}

void DesertEagle::CalculateOrientation()
{
    sf::Vector2f dir = E2D::Renderer::Instance().GetMousePositionWithCamera() - player->Position;

    lastRotation = rotation;
    rotation = std::atan2(dir.y, dir.x);

    aimDirection = sf::getNormalized(dir);
    if(aimDirection.x == 0.0f && aimDirection.y == 0.0f)
        aimDirection.x = 1.0f;   
}

void DesertEagle::HandleReload(const sf::Event& ev)
{
    if(ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::R)
    {
        if(shootMode->TryReload())
        {
            hud->GetAmmoBar().SetReloading(true);
            animations.ChangeAnimation(sprite, AnimationIndex::ReloadIndex);

            sounds[SoundIndex::ReloadSound].SetPosition(sf::Vector3f(player->Position.x, player->Position.y, 0.0));
            sounds[SoundIndex::ReloadSound].Play();
            
        }
    }
}

void DesertEagle::ResetOrientation()
{
    rotation = lastRotation = 0;
    aimDirection = sf::Vector2f(1.0f, 0.0);
}

void DesertEagle::ResetInterpolatedRotation()
{
    lastRotation = rotation;
}

void DesertEagle::HandleShoot()
{
    light->IsActive = false;
    shootMode->SetHoldingTrigger(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
    
    if(shootMode->CanShoot())
    {
        sounds[SoundIndex::ShootSound].SetPosition(sf::Vector3f(player->Position.x, player->Position.y, 0.0));
        sounds[SoundIndex::ShootSound].Play();

        //Rotamos el vector dependiendo de la precision
        float angle = Random::GlobalInstance.NextFloat(accuracy) * sf::PI_OVER_2;
        angle *=   Random::GlobalInstance.RangeInt(1) == 1 ? 1.0f : -1.0f;

        //Calculamos donde se encuentra la posicion del cañon
        sf::Vector2f currentBarrelOffset = gunBarrelOffset;
        if(aimDirection.x < 0.0f)
            currentBarrelOffset.y = -currentBarrelOffset.y;
        sf::Vector2f bulletOffset = sf::rotate(currentBarrelOffset, rotation);

        //Establecemos la posicion de salida de la bala y la direccion
        sf::Vector2f dir = sf::rotate(aimDirection, angle);
        sf::Vector2f pos = (player->Position) + bulletOffset + positionRelativeToPlayer;

        //Generamos la bala
        shootMode->Shoot(generator, pos, dir);
        animations.ChangeAnimation(sprite, AnimationIndex::ShootIndex);
        player->Velocity -= aimDirection * recoil; //Aplicar el retroceso
        hud->GetAmmoBar().SetCurrentAmmo(shootMode->CurrentAmmo);
        
        //Aplicamos el retroceso a la rotacion del arma
        float recoilRotSign = (aimDirection.x > 0.0f) ? -1.0f : 1.0f;
        rotation += (recoil / 200.0f) * sf::PI * 0.1f * recoilRotSign; //El retroceso base (200.0f) añade 18 grados al angulo

        light->IsActive = true;
        light->Position = pos;
    }
}

void DesertEagle::Draw(float tickPercent, sf::Vector2f position)
{
    sprite.setRotation(sf::degrees(sf::lerpAngle(lastRotation, rotation, tickPercent)));

    position += positionRelativeToPlayer;
    E2D::Renderer::Instance().Draw(position, sprite);
}

void DesertEagle::RemoveDraw()
{
    sprite.SetAlpha(0);
}

//!Setters Deagle

void DesertEagle::SetPlayer(Player* player)
{
    this->player = player;
}

void DesertEagle::SetLight(E2D::Light* light)
{
    this->light = light;
    this->light->IsActive = false;
    this->light->Color = lightColor;
}

void DesertEagle::SetLightColor(uint8_t red, uint8_t green, uint8_t blue)
{
    sf::Vector3f color = sf::Vector3f{ red / 255.0f, green / 255.0f, blue / 255.0f};
    this->lightColor = color;
    if(this->light)
        this->light->Color = this->lightColor;
}

void DesertEagle::SetMaxAmmo(int32_t ammo)
{
    shootMode->MaxAmmo = ammo;

    if(shootMode->MaxAmmo < MIN_AMMO)
        shootMode->MaxAmmo = MIN_AMMO;
    else if(shootMode->MaxAmmo > MAX_AMMO)
        shootMode->MaxAmmo = MAX_AMMO;
    hud->GetAmmoBar().SetMaxAmmo(shootMode->MaxAmmo);

    if(shootMode->CurrentAmmo > shootMode->MaxAmmo)
    {
        shootMode->CurrentAmmo = shootMode->MaxAmmo;
        hud->GetAmmoBar().SetCurrentAmmo(shootMode->CurrentAmmo);
    }
}

void DesertEagle::AddMaxAmmo(int32_t amount)
{
    shootMode->MaxAmmo += amount;

    if(shootMode->MaxAmmo > MAX_AMMO)
        shootMode->MaxAmmo = MAX_AMMO;

    hud->GetAmmoBar().SetMaxAmmo(shootMode->MaxAmmo);
}

void DesertEagle::RemoveMaxAmmo(int32_t amount)
{
    shootMode->MaxAmmo -= amount;

    if(shootMode->MaxAmmo < MIN_AMMO)
        shootMode->MaxAmmo = MIN_AMMO;
    hud->GetAmmoBar().SetMaxAmmo(shootMode->MaxAmmo);

    if(shootMode->CurrentAmmo > shootMode->MaxAmmo)
    {
        shootMode->CurrentAmmo = shootMode->MaxAmmo;
        hud->GetAmmoBar().SetCurrentAmmo(shootMode->CurrentAmmo);
    }
}

void DesertEagle::SetRecoil(float recoil)
{
    this->recoil = recoil;

    if(this->recoil < MIN_RECOIL)
        this->recoil = MIN_RECOIL;
    else if(this->recoil > MAX_RECOIL)
        this->recoil = MAX_RECOIL;
}

void DesertEagle::AddRecoil(float amount)
{
    recoil += amount;

    if(recoil > MAX_RECOIL)
        recoil = MAX_RECOIL;
}

void DesertEagle::RemoveRecoil(float amount)
{
    recoil -= amount;

    if(recoil < MIN_RECOIL)
        recoil = MIN_RECOIL;
}

void DesertEagle::SetAccuracy(float accuracy)
{
    this->accuracy = accuracy;

    if(this->accuracy < MIN_ACCURACY_VALUE)
        this->accuracy = MIN_ACCURACY_VALUE;
    else if(this->accuracy > MAX_ACCURACY_VALUE)
        this->accuracy = MAX_ACCURACY_VALUE;
}

void DesertEagle::AddAccuracy(float amount)
{
    accuracy -= amount; //Restamos ya que a menor valor mayor punteria

    if(accuracy < MIN_ACCURACY_VALUE)
        accuracy = MIN_ACCURACY_VALUE;
}

void DesertEagle::RemoveAccuracy(float amount)
{
    accuracy += amount;//Sumamos ya que a mayor valor menor punteria

    if(accuracy > MAX_ACCURACY_VALUE)
        accuracy = MAX_ACCURACY_VALUE;
} 


void DesertEagle::SetAttackSpeed(float attackSpeed)
{
    shootMode->AttackSpeed = attackSpeed;

    if(shootMode->AttackSpeed < MIN_ATTACK_SPEED_VALUE)
        shootMode->AttackSpeed = MIN_ATTACK_SPEED_VALUE;
    else if(shootMode->AttackSpeed > MAX_ATTACK_SPEED_VALUE)
        shootMode->AttackSpeed = MAX_ATTACK_SPEED_VALUE;
}

void DesertEagle::AddAttackSpeed(float amount)
{
    shootMode->AttackSpeed -= amount; //Restamos ya que a menor valor mayor velocidad de ataque
    
    if(shootMode->AttackSpeed < MIN_ATTACK_SPEED_VALUE)
        shootMode->AttackSpeed = MIN_ATTACK_SPEED_VALUE;
}

void DesertEagle::RemoveAttackSpeed(float amount)
{
    shootMode->AttackSpeed += amount; //Sumamos ya que a menor valor mayor velocidad de ataque
    
    if(shootMode->AttackSpeed > MAX_ATTACK_SPEED_VALUE)
        shootMode->AttackSpeed = MAX_ATTACK_SPEED_VALUE;
}

void DesertEagle::SetReloadTime(float reloadTime)
{
    shootMode->ReloadTime = reloadTime;

    if(shootMode->ReloadTime > MAX_RELOAD_TIME)
        shootMode->ReloadTime = MAX_RELOAD_TIME;
    else if(shootMode->ReloadTime < MIN_RELOAD_TIME)
        shootMode->ReloadTime = MIN_RELOAD_TIME;
}

void DesertEagle::AddReloadTime(float amount)
{
    shootMode->ReloadTime += amount;

    if(shootMode->ReloadTime > MAX_RELOAD_TIME)
        shootMode->ReloadTime = MAX_RELOAD_TIME;
}

void DesertEagle::RemoveReloadTime(float amount)
{
    shootMode->ReloadTime -= amount;

    if(shootMode->ReloadTime < MIN_RELOAD_TIME)
        shootMode->ReloadTime = MIN_RELOAD_TIME;
}

//!Setters Generator

void DesertEagle::SetBulleType(BulletType type)
{
    generator.BulletType = type;
}

void DesertEagle::SetBulletDamage(float damage)
{
    generator.BulletDamage = damage;

    if(generator.BulletDamage < MIN_BULLET_DAMAGE)
        generator.BulletDamage = MIN_BULLET_DAMAGE;
    else if(generator.BulletDamage > MAX_BULLET_DAMAGE)
        generator.BulletDamage = MAX_BULLET_DAMAGE;
}

void DesertEagle::AddBulletDamage(float amount)
{
    generator.BulletDamage += amount;

    if(generator.BulletDamage > MAX_BULLET_DAMAGE)
        generator.BulletDamage = MAX_BULLET_DAMAGE;
}

void DesertEagle::RemoveBulletDamage(float amount)
{
    generator.BulletDamage -= amount;

    if(generator.BulletDamage < MIN_BULLET_DAMAGE)
        generator.BulletDamage = MIN_BULLET_DAMAGE;
}

void DesertEagle::SetBulletFriction(float friction)
{
    generator.BulletFriction = friction;

    if(generator.BulletFriction > MIN_FRICTION)
        generator.BulletFriction = MIN_FRICTION;
    else if(generator.BulletFriction < MAX_FRICTION)
        generator.BulletFriction = MAX_FRICTION;
}

void DesertEagle::AddBulletFriction(float amount)
{
    generator.BulletFriction -= amount; //Restamos ya que a menor valor mayor friccion

    if(generator.BulletFriction < MAX_FRICTION)
        generator.BulletFriction = MAX_FRICTION;
}

void DesertEagle::RemoveBulletFriction(float amount)
{
    generator.BulletFriction += amount; //Sumamos ya que a mayor valor menor friccion

    if(generator.BulletFriction > MIN_FRICTION)
        generator.BulletFriction = MIN_FRICTION;
}

void DesertEagle::SetBulletSize(float size)
{
    generator.BulletSize = size;

    if(generator.BulletSize < MIN_BULLET_SIZE)
        generator.BulletSize = MIN_BULLET_SIZE;
    else if(generator.BulletSize > MAX_BULLET_SIZE)
        generator.BulletSize = MAX_BULLET_SIZE;
}

void DesertEagle::AddBulletSize(float amount)
{
    generator.BulletSize += amount;;

    if(generator.BulletSize > MAX_BULLET_SIZE)
        generator.BulletSize = MAX_BULLET_SIZE;
}

void DesertEagle::RemoveBulletSize(float amount)
{   
    generator.BulletSize -= amount;

    if(generator.BulletSize < MIN_BULLET_SIZE)
        generator.BulletSize = MIN_BULLET_SIZE;
}


void DesertEagle::SetBulletSpeed(float speed)
{
    generator.BulletSpeed = speed;

    if(generator.BulletSpeed < MIN_BULLET_SPEED)
        generator.BulletSpeed = MIN_BULLET_SPEED;
    else if(generator.BulletSpeed > MAX_BULLET_SPEED)
        generator.BulletSpeed = MAX_BULLET_SPEED;
}

void DesertEagle::AddBulletSpeed(float amount)
{
    generator.BulletSpeed += amount;

    if(generator.BulletSpeed > MAX_BULLET_SPEED)
        generator.BulletSpeed = MAX_BULLET_SPEED;
}

void DesertEagle::RemoveBulletSpeed(float amount)
{
    generator.BulletSpeed -= amount;

    if(generator.BulletSpeed < MIN_BULLET_SPEED)
        generator.BulletSpeed = MIN_BULLET_SPEED;
}

void DesertEagle::SetBulletKnockBack(float knockback)
{
    generator.BulletKnockBack = knockback;

    if(generator.BulletKnockBack < MIN_BULLET_KNOCK_BACK)
        generator.BulletKnockBack = MIN_BULLET_KNOCK_BACK;
    else if(generator.BulletKnockBack > MAX_BULLET_KNOCK_BACK)
        generator.BulletKnockBack = MAX_BULLET_KNOCK_BACK;
}

void DesertEagle::AddBulletKnockBack(float amount)
{
    generator.BulletKnockBack += amount;

    if(generator.BulletKnockBack > MAX_BULLET_KNOCK_BACK)
        generator.BulletKnockBack = MAX_BULLET_KNOCK_BACK;
}

void DesertEagle::RemoveBulletKnockBack(float amount)
{
    generator.BulletKnockBack -= amount;

    if(generator.BulletKnockBack < MIN_BULLET_KNOCK_BACK)
        generator.BulletKnockBack = MIN_BULLET_KNOCK_BACK;
}

void DesertEagle::SetBulletMaxAliveTime(float maxAliveTime)
{
    generator.BulletMaxAliveTime = maxAliveTime;

    if(generator.BulletMaxAliveTime < MIN_BULLET_MAX_ALIVE_TIME)
        generator.BulletMaxAliveTime = MIN_BULLET_MAX_ALIVE_TIME;
    else if(generator.BulletMaxAliveTime > MAX_BULLET_MAX_ALIVE_TIME)
        generator.BulletMaxAliveTime = MAX_BULLET_MAX_ALIVE_TIME;
}

void DesertEagle::AddBulletMaxAliveTime(float amount)
{
    generator.BulletMaxAliveTime += amount;

    if(generator.BulletMaxAliveTime > MAX_BULLET_MAX_ALIVE_TIME)
        generator.BulletMaxAliveTime = MAX_BULLET_MAX_ALIVE_TIME;
}

void DesertEagle::RemoveBulletMaxAliveTime(float amount)
{
    generator.BulletMaxAliveTime -= amount;

    if(generator.BulletMaxAliveTime < MIN_BULLET_MAX_ALIVE_TIME)
        generator.BulletMaxAliveTime = MIN_BULLET_MAX_ALIVE_TIME;
}

void DesertEagle::SetBulletBaseAcceleration(sf::Vector2f baseAcceleration)
{
    generator.BulletBaseAcceleration = baseAcceleration;
}

void DesertEagle::SetBulletTexture(E2D::Texture* texture)
{
    generator.BulletTexture = texture;
}

void DesertEagle::SetBulletColor(const sf::Color& color)
{
    generator.BulletColor = color;
}

void DesertEagle::SetBulletAnimationFrames(std::vector<sf::IntRect>* aliveFrames, std::vector<sf::IntRect>* deadFrames)
{
    generator.BulletAnimationFrames[0] = aliveFrames;
    generator.BulletAnimationFrames[1] = deadFrames;
}

void DesertEagle::SetBulletAnimationDurations(float aliveDuration, float deadDuration)
{
    generator.BulletAnimationDurations[0] = aliveDuration;
    generator.BulletAnimationDurations[1] = deadDuration;
}

void DesertEagle::SetBulletTexturePadding(float padding)
{
    generator.BulletTexturePadding = padding;
}

void DesertEagle::SetBulletCollisionMask(uint32_t mask)
{
    generator.BulletCollisionMask = mask;
}

void DesertEagle::AddBulletCollisionMask(uint32_t mask)
{
    generator.BulletCollisionMask |= mask;
}

void DesertEagle::RemoveBulletCollisionMask(uint32_t mask)
{
    generator.BulletCollisionMask &= (~mask);
}

void DesertEagle::AddBulletPointGenerator(BulletPointGenerator* pointGenerator)
{
    generator.PointGenerators.push_back(pointGenerator);
}

void DesertEagle::SetShootMode(ShootMode* newShootMode)
{
    newShootMode->Copy(shootMode);
    delete shootMode;
    shootMode = newShootMode;
}

void DesertEagle::AddBulletOnCollisionWithEnemyCallback(std::function<void(Bullet*, Enemy*, const CollisionData&)>&& callback)
{
    generator.OnCollisionWithEnemyCallbacks.emplace_back(callback);
}

void DesertEagle::AddBulletOnCollisionWithTileCallback(std::function<void(Bullet*, const TileCollisionData&)>&& callback)
{
    generator.OnCollisionWithTileCallbacks.emplace_back(callback);
}

void DesertEagle::AddBulletOnReachMaxAliveTimeCallback(std::function<void(Bullet*)>&& callback)
{
    generator.OnReachMaxAliveTimeCallbacks.emplace_back(callback);
}