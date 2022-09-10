#pragma once

#include <cstdint>
#include "SFML/Graphics.hpp"

class BulletGenerator;

class ShootMode
{
public:
    ShootMode();
    ShootMode(int32_t maxAmmo, float attackSpeed, float reloadTime);

    virtual bool TryReload();
    virtual void Update(float dt);
    virtual void SetHoldingTrigger(bool value);
    virtual bool CanShoot() = 0;
    virtual void Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction);
    void Copy(ShootMode* other);

    inline bool IsReloading() { return isReloading; }
    inline bool JustReloaded() { return justReloaded; }

    int32_t MaxAmmo { 6 };
    int32_t CurrentAmmo { 6 };
    float AttackSpeed { 0.4f };
    float ReloadTime { 1.0f };

    virtual ~ShootMode() = default;
protected:
    float lastShotTime { 0.4f };
    float currentReloadTime { 0.0f };
    bool isReloading { false };
    bool justReloaded { false };
    bool isHoldingTrigger { false }; 
};

class SingleShotMode : public ShootMode
{
public:
    SingleShotMode();
    void SetHoldingTrigger(bool value) override;
    bool CanShoot() override;

protected:
    bool hasAlreadyShot;
};

class AutomaticShotMode : public ShootMode
{
public:
    bool CanShoot() override;
};

class ChargedShotMode : public ShootMode
{
public:
    ChargedShotMode();

    void Update(float dt) override;
    void Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction) override;
    void SetHoldingTrigger(bool value) override;
    bool CanShoot() override;

protected:
    float holdTime;
    bool hasReleasedTrigger;
};

class BurstShotMode : public ShootMode
{
public:
    BurstShotMode();

    void Update(float dt) override;
    void SetHoldingTrigger(bool value) override;
    bool CanShoot() override;
    bool TryReload() override;
    void Shoot(BulletGenerator& generator, sf::Vector2f position, sf::Vector2f direction) override;

protected:
    static constexpr float MAX_TIME_BETWEEN_BURST_BULLETS = 0.06f;

    int32_t maxBulletsPerBurst;
    int32_t currentBurstBullets;
    float timeBetweenBurstBullets;
    bool isShootingBurst;
    bool hasAlreadyShot;
};

class MiniGunShotMode : public ShootMode
{
public:
    MiniGunShotMode();

    void Update(float dt) override;
    void SetHoldingTrigger(bool value) override;
    bool CanShoot() override;

protected:
    float holdTime;
};