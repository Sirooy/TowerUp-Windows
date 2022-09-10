#pragma once

#include "DynamicEntity.h"
#include "../Engine2D/Sprite.h"
#include "../Engine2D/MediaContent.h"
#include "../AnimationPack.h"
#include "InteractiveElement.h"

class BulletGenerator;

class Bullet : public DynamicEntity
{
public:
    Bullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f velocity, bool executeCallbacks);

    void Update(float dt) override;
    void Draw(float tickPercent) override;

    void OnCollision(CollidableEntity* other, const CollisionData& data) override;
    void OnCollision(Enemy* enemy, const CollisionData& data) override;
    void OnCollision(Player* player, const CollisionData& data) override;
    void OnCollision(Bullet* other, const CollisionData& data) override;
    void OnHorizontalTileCollision(const TileCollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;
    void Collision(sf::Vector2f normal, float penetration);

    void UpdateAlive(float dt);
    void UpdateDead(float dt);

    virtual ~Bullet() = default;

    BulletGenerator* Generator;
protected:
    enum AnimationIndex
    {
        AliveIndex = 0,
        DeadIndex
    };

    enum SoundIndex
    {
        CollisionTile = 0,
        CollisionEnemy,
        CollisionMagic
    };

    void (Bullet::*updateFunc)(float dt);
    AnimationPack<2> animations;
    E2D::Sprite sprite;
    //E2D::Sound sounds[3];

    void CreateSounds();
    void KillBulet();
public:
    sf::Vector2f Direction;
    float Damage;
    int32_t PlayerDamage;
    float MaxAliveTime;
    float AliveTime;
    float KnockBack;
    float Size;

    bool HasCollided;

protected:
    bool executeCallbacks;
};

class RubberBullet : public Bullet
{
public:
    RubberBullet(BulletGenerator* generator, sf::Vector2f position, sf::Vector2f velocity, bool executeCallbacks);

    void OnHorizontalTileCollision(const TileCollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;
protected:
    static constexpr float BOUNCE_SEPARATION = 4.0f;

    int32_t numBounces;
};

//Estadisticas de la bala
//Daño
//Daño al jugador
//Tamaño
//Tiempo maximo vida
//Tiempo actual de vida
//Collision Mask | Collision Type
//Knockback
//Velocidad (Direccion)
//Textura * 
//Frames de animacion *
//Sprite
//Sonido de choque *
//2 Animation
//Aceleracion base

//Estadisticas del arma
//Precision
//Velocidad de recarga
//Retroceso
//Velocidad de ataque
//Numero maximo de balas
//Numero actual de balas
//Sonido de disparo