#include "ShootMode.h"
#include "BulletGenerator.h"
#include "../SFMLUtils.hpp"

//! ############### BASE SHOT MODE ###############
ShootMode::ShootMode() : MaxAmmo(0), CurrentAmmo(0), AttackSpeed(0.0f), ReloadTime(0.0f),
    lastShotTime(0.0f), currentReloadTime(0.0f), isReloading(false), isHoldingTrigger(false) { }

ShootMode::ShootMode(int32_t maxAmmo, float attackSpeed, float reloadTime) : 
    MaxAmmo(maxAmmo), CurrentAmmo(maxAmmo), AttackSpeed(attackSpeed), ReloadTime(reloadTime),
    lastShotTime(attackSpeed), currentReloadTime(0.0f), isReloading(false), isHoldingTrigger(false) { }

bool ShootMode::TryReload()
{
    if(!isReloading && CurrentAmmo < MaxAmmo)
    {
        isReloading = true;
        currentReloadTime = 0.0f;

        return true;
    }

    return false;
}

void ShootMode::Update(float dt)
{
    justReloaded = false;
    currentReloadTime += dt;
    lastShotTime += dt;
    
    if(isReloading && currentReloadTime >= ReloadTime)
    {
        CurrentAmmo = MaxAmmo;
        isReloading = false;
        justReloaded = true;
    }
}

void ShootMode::SetHoldingTrigger(bool value)
{
    isHoldingTrigger = value;
}

void ShootMode::Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction)
{
    generator.GenerateBullet(position + direction * generator.BulletSize, direction);
    lastShotTime = 0.0f;
    --CurrentAmmo;
}

void ShootMode::Copy(ShootMode* other)
{
    this->MaxAmmo = other->MaxAmmo; 
    this->AttackSpeed = other->AttackSpeed;
    this->ReloadTime = other->ReloadTime;
    this->CurrentAmmo = other->CurrentAmmo; 
    this->lastShotTime = other->lastShotTime; 
    this->currentReloadTime = other->currentReloadTime; 
    this->isReloading = other->isReloading;
    this->justReloaded = other->justReloaded;
    this->isHoldingTrigger = other->isHoldingTrigger;
}

//! ############### SINGLE SHOT MODE ###############
SingleShotMode::SingleShotMode() : ShootMode(), hasAlreadyShot(false) { }

void SingleShotMode::SetHoldingTrigger(bool value)
{
    if(!isHoldingTrigger && value)
        hasAlreadyShot = false;
    else
        hasAlreadyShot = true;

    isHoldingTrigger = value;
}

bool SingleShotMode::CanShoot()
{
    return !hasAlreadyShot && CurrentAmmo > 0 && !isReloading && lastShotTime >= AttackSpeed;
}

//! ############### AUTOMATIC SHOT MODE ###############

bool AutomaticShotMode::CanShoot()
{
    return isHoldingTrigger && CurrentAmmo > 0 && !isReloading && lastShotTime >= AttackSpeed;
}

//! ############### CHARGED SHOT MODE ###############

ChargedShotMode::ChargedShotMode() : ShootMode(), holdTime(0.0f), hasReleasedTrigger(false) { }

void ChargedShotMode::Update(float dt)
{
    justReloaded = false;
    currentReloadTime += dt;
    lastShotTime += dt;
    
    if(isReloading && currentReloadTime >= ReloadTime)
    {
        CurrentAmmo = MaxAmmo;
        isReloading = false;
        justReloaded = true;
        holdTime = 0.0f;
    }

    if(isHoldingTrigger)
        holdTime += dt;
}

void ChargedShotMode::SetHoldingTrigger(bool value)
{
    if(!isHoldingTrigger && value) //Resetear el tiempo que mantiene el botón pulsado una vez se vuelve a pulsar
        holdTime = 0.0f;

    if(isHoldingTrigger && !value)
        hasReleasedTrigger = true;
    else
        hasReleasedTrigger = false;

    isHoldingTrigger = value;
}

bool ChargedShotMode::CanShoot()
{
    return hasReleasedTrigger && CurrentAmmo > 0 && !isReloading;
}

void ChargedShotMode::Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction)
{
    float mult = sf::clamp(holdTime * (1.0f / AttackSpeed), 0.25f, 3.0f);
    
    float size = generator.BulletSize;
    float damage = generator.BulletDamage;

    generator.BulletSize *= mult;
    generator.BulletDamage *= mult;
    generator.GenerateBullet(position + direction * generator.BulletSize, direction);
    generator.BulletSize = size;
    generator.BulletDamage = damage;

    --CurrentAmmo;
}

//! ############### BURST SHOT MODE ###############

BurstShotMode::BurstShotMode() : ShootMode(), maxBulletsPerBurst(3), currentBurstBullets(0), 
    timeBetweenBurstBullets(0.0f), isShootingBurst(false), hasAlreadyShot(false) { }

bool BurstShotMode::TryReload()
{
    if(!isReloading && CurrentAmmo < MaxAmmo && !isShootingBurst)
    {
        isReloading = true;
        currentReloadTime = 0.0f;

        return true;
    }

    return false;
}

void BurstShotMode::Update(float dt)
{
    justReloaded = false;
    currentReloadTime += dt;
    lastShotTime += dt;
    timeBetweenBurstBullets += dt;
    
    if(isReloading && currentReloadTime >= ReloadTime)
    {
        CurrentAmmo = MaxAmmo;
        isReloading = false;
        justReloaded = true;
    }
}

void BurstShotMode::SetHoldingTrigger(bool value)
{
    if(!isHoldingTrigger && value)
        hasAlreadyShot = false;
    else
        hasAlreadyShot = true;

    isHoldingTrigger = value;
}

bool BurstShotMode::CanShoot()
{
    return (isShootingBurst && timeBetweenBurstBullets > MAX_TIME_BETWEEN_BURST_BULLETS) || 
        (!hasAlreadyShot && CurrentAmmo > 0 && !isReloading && lastShotTime >= AttackSpeed);
}

void BurstShotMode::Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction)
{
    if(isShootingBurst)
    {
        --currentBurstBullets;
        timeBetweenBurstBullets = 0.0f;
        generator.GenerateBullet(position + direction * generator.BulletSize, direction);

        if(currentBurstBullets == 0)
        {
            isShootingBurst = false;
            lastShotTime = 0.0f;
        }
    }
    else
    {
        isShootingBurst = true;
        timeBetweenBurstBullets = 0.0f;
        currentBurstBullets = maxBulletsPerBurst - 1;
        generator.GenerateBullet(position + direction * generator.BulletSize, direction);
        --CurrentAmmo;
    }
}

MiniGunShotMode::MiniGunShotMode() : ShootMode(), holdTime(0.0f) { }

void MiniGunShotMode::Update(float dt)
{
    justReloaded = false;
    currentReloadTime += dt;
    lastShotTime += dt;
    
    if(isReloading && currentReloadTime >= ReloadTime)
    {
        CurrentAmmo = MaxAmmo;
        isReloading = false;
        justReloaded = true;
        holdTime = 1.0f;
    }

    if(isHoldingTrigger)
        holdTime += dt;
}

void MiniGunShotMode::SetHoldingTrigger(bool value)
{
    if(!isHoldingTrigger && value) //Resetear el tiempo que mantiene el botón pulsado una vez se vuelve a pulsar
        holdTime = 1.0f;

    isHoldingTrigger = value;
}

bool MiniGunShotMode::CanShoot()
{
    return lastShotTime >= (AttackSpeed / (holdTime * 0.75f))  && CurrentAmmo > 0 && isHoldingTrigger && !isReloading;
}