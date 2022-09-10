#pragma once

#include "SFML/Window/Event.hpp"
#include "BulletGenerator.h"
#include "ShootMode.h"
#include "../Engine2D/Sprite.h"
#include "../Engine2D/Sound.h"
#include "../AnimationPack.h"
#include "../Engine2D/Sound.h"
#include <functional>

class Hud;
class Player;

namespace E2D
{

class Light;

} 


class DesertEagle
{
public:
    static constexpr float WIDTH = 34.0f;
    static constexpr float HEIGHT = 26.0f;


    DesertEagle();

    void InitAnimations();
    void CreateSounds();
    void CreateBulletGenerator();
    void Update(float dt);
    void CalculateOrientation();
    void HandleShoot();
    void HandleReload(const sf::Event& ev);
    void Draw(float tickPercent, sf::Vector2f position);
    void ResetOrientation();
    void ResetInterpolatedRotation();
    void RemoveDraw();

    inline E2D::Sprite& GetSprite() { return sprite; }
    inline sf::Vector3f GetLightColor() { return lightColor; }
    inline sf::Vector2f GetAimDirection() const { return aimDirection; }
    inline int32_t GetMaxAmmo() const { return shootMode->MaxAmmo; }
    inline int32_t GetCurrentAmmo() const { return shootMode->CurrentAmmo; }
    inline float GetAccuracy() const { return accuracy; }
    inline float GetRecoil() const { return recoil; }
    inline float GetAttackSpeed() const { return shootMode->AttackSpeed; }
    inline float GetReloadTime() const { return shootMode->ReloadTime; }
    inline BulletType GetBulletType() const { return generator.BulletType; }
    inline float GetBulletDamage() const { return generator.BulletDamage; }
    inline float GetBulletSize() const { return generator.BulletSize; }
    inline float GetBulletSpeed() const { return generator.BulletSpeed; } 
    inline float GetBulletKnockBack() const  { return generator.BulletKnockBack; }
    inline float GetBulletMaxAliveTime() const { return generator.BulletMaxAliveTime; }
    inline sf::Vector2f GetBulletBaseAcceleration() const { return generator.BulletBaseAcceleration; }
    inline sf::Color GetBulletColor() const { return generator.BulletColor; }
    inline BulletGenerator& GetGenerator() { return generator; }

    //Setters deagle
    void SetPlayer(Player* player);
    void SetLight(E2D::Light* light);
    void SetLightColor(uint8_t red, uint8_t green, uint8_t blue);

    void SetMaxAmmo(int32_t ammo);
    void AddMaxAmmo(int32_t amount);
    void RemoveMaxAmmo(int32_t amount);
    void SetRecoil(float recoil);
    void AddRecoil(float amount);
    void RemoveRecoil(float amount);
    void SetAccuracy(float accuracy);
    void AddAccuracy(float amount);
    void RemoveAccuracy(float accuracy);
    void SetAttackSpeed(float attackSpeed);
    void AddAttackSpeed(float amount);
    void RemoveAttackSpeed(float amount);
    void SetReloadTime(float reloadTime);
    void AddReloadTime(float amount);
    void RemoveReloadTime(float amount);

    //Setters generator
    void SetBulleType(BulletType type);
    void SetBulletDamage(float damage);
    void AddBulletDamage(float amount);
    void RemoveBulletDamage(float amount);
    void SetBulletFriction(float friction);
    void AddBulletFriction(float amount);
    void RemoveBulletFriction(float amount);
    void SetBulletSize(float size);
    void AddBulletSize(float amount);
    void RemoveBulletSize(float amount);
    void SetBulletSpeed(float speed);
    void AddBulletSpeed(float amount);
    void RemoveBulletSpeed(float amount);
    void SetBulletKnockBack(float knockback);
    void AddBulletKnockBack(float amount);
    void RemoveBulletKnockBack(float amount);
    void SetBulletMaxAliveTime(float maxAliveTime);
    void AddBulletMaxAliveTime(float amount);
    void RemoveBulletMaxAliveTime(float amount);
    void SetBulletBaseAcceleration(sf::Vector2f baseAcceleration);
    void SetBulletTexture(E2D::Texture* texture);
    void SetBulletColor(const sf::Color& color);
    void SetBulletAnimationFrames(std::vector<sf::IntRect>* aliveFrames, std::vector<sf::IntRect>* deadFrames);
    void SetBulletAnimationDurations(float aliveDuration, float deadDuration);
    void SetBulletTexturePadding(float padding);
    void SetBulletCollisionMask(uint32_t mask);
    void AddBulletCollisionMask(uint32_t mask);
    void RemoveBulletCollisionMask(uint32_t mask);
    void AddBulletPointGenerator(BulletPointGenerator* pointGenerator);
    void SetShootMode(ShootMode* newShootMode);
    void AddBulletOnCollisionWithEnemyCallback(std::function<void(Bullet*, Enemy*, const CollisionData&)>&& callback);
    void AddBulletOnCollisionWithTileCallback(std::function<void(Bullet*, const TileCollisionData&)>&& callback);
    void AddBulletOnReachMaxAliveTimeCallback(std::function<void(Bullet*)>&& callback);
    
protected:
    static constexpr int32_t MIN_AMMO = 1;
    static constexpr int32_t MAX_AMMO = 34;
    static constexpr float MIN_RECOIL = 0.0f;
    static constexpr float MAX_RECOIL = 2000.0f;
    static constexpr float MIN_ACCURACY_VALUE = 0.000000001f;
    static constexpr float MAX_ACCURACY_VALUE = 1.0f;
    static constexpr float MIN_ATTACK_SPEED_VALUE = 0.005f;
    static constexpr float MAX_ATTACK_SPEED_VALUE = 5.0f;
    static constexpr float MIN_RELOAD_TIME = 0.05f;
    static constexpr float MAX_RELOAD_TIME = 5.0f;
    static constexpr float MIN_BULLET_DAMAGE = 1.0f;
    static constexpr float MAX_BULLET_DAMAGE = 10000.0f;
    static constexpr float MAX_FRICTION = 0.0000000001f;
    static constexpr float MIN_FRICTION = 1.0f;
    static constexpr float MIN_BULLET_SIZE = 6.0f;
    static constexpr float MAX_BULLET_SIZE = 32.0f;
    static constexpr float MIN_BULLET_SPEED = 50.0f;
    static constexpr float MAX_BULLET_SPEED = 1000.0f;
    static constexpr float MIN_BULLET_KNOCK_BACK = 1.0f;
    static constexpr float MAX_BULLET_KNOCK_BACK = 2000.0f;
    static constexpr float MIN_BULLET_MAX_ALIVE_TIME = 0.5f;
    static constexpr float MAX_BULLET_MAX_ALIVE_TIME = 30.0f;

    enum AnimationIndex
    {
        ShootIndex = 0,
        ReloadIndex = 1
    };

    enum SoundIndex
    {
        ShootSound = 0,
        ReloadSound
    };

    E2D::Sprite sprite {};
    AnimationPack<3> animations {};
    BulletGenerator generator{};
    Player* player {};
    Hud* hud{};
    ShootMode* shootMode {};
    E2D::Light* light;
    sf::Vector3f lightColor{ 1.0f, 0.7843f , 0.7843f };
    sf::Vector2f aimDirection { 0.0f, 0.0f };
    E2D::Sound sounds[2];
    sf::Vector2f gunBarrelOffset { 0.0f, 0.0f };
    sf::Vector2f positionRelativeToPlayer { 0.0f, 10.0f };
    float rotation { 0.0f };
    float lastRotation { 0.0f };
    float recoil { 200.0f };
    float accuracy { 0.05f };
};