#pragma once

#include "Engine2D/Filter.h"

class Curse
{
public:
    static constexpr float SPRITE_SIZE = 16.0f;
    static constexpr float ICON_SIZE = 32.0f;

    Curse(int32_t id);
    virtual void ApplyDebuff() = 0;
    virtual void RemoveDebuff() = 0;

    inline const sf::IntRect& GetTextureRect() const { return textureRect; }

    virtual ~Curse() = default;

protected:
    static constexpr int32_t TEXTURE_ICONS_PER_ROW = 8;

    sf::IntRect textureRect {};
    int32_t id;
};

class VisualCurse : public Curse
{
public:
    VisualCurse(E2D::FilterType filterType);

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    E2D::FilterType filterType;
};

class DoubleLifeEnemiesCurse : public Curse
{
public:
    DoubleLifeEnemiesCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    float removedDamage { 0.0f };
};

class OneHeartCurse : public Curse
{
public:
    OneHeartCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    int32_t lastMaxHealth { 0 };
    int32_t lastCurrentHealth { 0 };
};

class OneBulletCurse : public Curse
{
public:
    OneBulletCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    int32_t lastMaxAmmo { 0 };
};


class MaxDashCooldownCurse : public Curse
{
public:
    MaxDashCooldownCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    float lastDashCooldown { 0.0f };
};


class HalfMoneyCurse : public Curse
{
public:
    HalfMoneyCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;
};

class MinMoveSpeedCurse : public Curse
{
public:
    MinMoveSpeedCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    float lastMoveSpeed { 0.0f };
};

class MoreRecoilCurse : public Curse
{
public:
    MoreRecoilCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    float addedRecoil { 0.0f };
};


class MoreReloadTimeCurse : public Curse
{
public:
    MoreReloadTimeCurse();

    void ApplyDebuff() override;
    void RemoveDebuff() override;

protected:
    float addedReloadTime { 0.0f };
};
