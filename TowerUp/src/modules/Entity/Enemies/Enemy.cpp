#include "Enemy.h"
#include "../../Physics/CollisionData.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../Engine2D/Renderer.h"
#include "../../CollisionLayer.h"
#include "../../Random.h"
#include "../Consumable.h"
#include "../Bullet.h"
#include "../World.h"
#include "../Player.h"
#include "../HurtZone.h"

Enemy::Enemy(sf::Vector2f position, float size, float health, int32_t attack) : 
    DynamicEntity(position, size), sprite(), physics(PhysicsEngine::Instance()), 
    player(*World::Instance().GetPlayer()), health(health), attack(attack), knockback(200.0f), armor(1.0f), knockbackResistance(1.0f)
{
    this->CollisionType = CollisionType::EnemyCollisionType;
    AddCollisionMask(CollisionType::TileCollisionType);
    AddCollisionMask(CollisionType::PlayerBulletCollisionType);
    AddCollisionMask(CollisionType::ExplosionCollisionType);
}

Enemy::Enemy(sf::Vector2f position, float width, float height, float health, int32_t attack) : 
    DynamicEntity(position, width, height),  sprite(), physics(PhysicsEngine::Instance()), 
    player(*World::Instance().GetPlayer()), health(health), attack(attack), knockback(200.0f), armor(1.0f), knockbackResistance(1.0f)
{
    this->CollisionType = CollisionType::EnemyCollisionType;
    AddCollisionMask(CollisionType::TileCollisionType);
    AddCollisionMask(CollisionType::PlayerBulletCollisionType);
    AddCollisionMask(CollisionType::ExplosionCollisionType);
}

bool Enemy::IsCloseToPlayer(float detectionDistSquared)
{
    return sf::lengthSquared(player.Position - this->Position) < detectionDistSquared;
}

bool Enemy::HasVisionOfPlayer(float detectionDistSquared)
{
    sf::Vector2f dir = player.Position - this->Position;
    float distanceToPlayerSquared = sf::lengthSquared(dir);

    auto rayCastData = physics.RayCastTileMap(Position, dir);
    return distanceToPlayerSquared < rayCastData.Distance * rayCastData.Distance && 
           distanceToPlayerSquared < detectionDistSquared; 
}

bool Enemy::HasVisionOfPlayer(float detectionDistSquared, sf::Vector2f viewDir)
{
    sf::Vector2f dir = player.Position - this->Position;
    float distanceToPlayerSquared = sf::lengthSquared(dir);

    if(sf::dot(viewDir, dir) <= 0.0f)
        return false;

    auto rayCastData = physics.RayCastTileMap(Position, dir);
    return rayCastData.Distance * rayCastData.Distance > distanceToPlayerSquared && 
        distanceToPlayerSquared < detectionDistSquared; 
}

bool Enemy::IsNearWall(float direction, float distance)
{
    //if(direction != 0.0f)
    //    direction /= direction;

    auto collisionLayer = physics.GetCollisionLayer();
    sf::Vector2f coords = Position;
    coords.x += direction * distance;
    
    int32_t x = collisionLayer->GetScaledCoord(coords.x);
    int32_t y = collisionLayer->GetScaledCoord(coords.y);

    return collisionLayer->Collides(x, y);
}

bool Enemy::IsNearEdge(float halfHeight, float direction, float distance)
 {
    //if(direction != 0.0f)
    //   direction /= direction;

    halfHeight += 1.0f;
    auto collisionLayer = physics.GetCollisionLayer();
    sf::Vector2f edgeCoords = Position + sf::Vector2f(direction * distance, halfHeight);
    sf::Vector2f floorCoors = Position + sf::Vector2f(0.0f, halfHeight);

    int32_t edgeX = collisionLayer->GetScaledCoord(edgeCoords.x);
    int32_t edgeY = collisionLayer->GetScaledCoord(edgeCoords.y);
    int32_t floorX = collisionLayer->GetScaledCoord(floorCoors.x);
    int32_t floorY = collisionLayer->GetScaledCoord(floorCoors.y);

    return !collisionLayer->Collides(edgeX, edgeY) && collisionLayer->Collides(floorX, floorY);
}

sf::Vector2f Enemy::GetRandomMapPosition()
{
    auto collisionLayer = physics.GetCollisionLayer();
    std::vector<sf::Vector2i> emptyPositions;

    for(int32_t x = 0;x < collisionLayer->GetCurrentWidth(); ++x)
    {
        for(int32_t y = 0;y < collisionLayer->GetCurrentHeight(); ++y)
        {
            if(collisionLayer->Get(x, y) == 0)
                emptyPositions.push_back(sf::Vector2i(x, y));
        }
    }

    if(emptyPositions.size() > 0)
    {
        sf::Vector2i pos = emptyPositions[Random::GlobalInstance.NextInt(emptyPositions.size())];

        return sf::Vector2f(pos.x * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f,
                            pos.y * E2D::TileMap::TILE_SIZE + E2D::TileMap::TILE_SIZE * 0.5f);
    }

    return sf::Vector2f(0.0f, 0.0f);
}

void Enemy::SetSpriteOrientation(float dir)
{
    if(dir < 0)
        sprite.SetFlipHorizontally(true);
    else
        sprite.SetFlipHorizontally(false);
}

void Enemy::SetSpriteOrientation(sf::Vector2f viewDir)
{
    if(viewDir.x < 0)
        sprite.SetFlipHorizontally(true);
    else
        sprite.SetFlipHorizontally(false);
}

void Enemy::SetBulletGeneratorBaseData(BulletGenerator* generator)
{
    generator->BulletCollisionType = CollisionType::EnemyBulletCollisionType;
    generator->BulletCollisionMask = CollisionType::PlayerCollisionType | CollisionType::TileCollisionType;
}

void Enemy::Kill()
{
    IsDead = true;

    TryDropConsumable();
}

void Enemy::TryDropConsumable()
{
    if (Random::GlobalInstance.NextFloat() <= DROP_BASE_CHANGE + World::Instance().GetGlobalDropProbability())
    {
        int32_t dropType = DROP_RATIO[Random::GlobalInstance.NextInt(DROP_RATIO.size())];
        
        ConsumableType consumableType;
        if(dropType == 0) //Moneda
            consumableType = COIN_DROP_RATIO[Random::GlobalInstance.NextInt(COIN_DROP_RATIO.size())];
        else
            consumableType = HEART_DROP_RATIO[Random::GlobalInstance.NextInt(HEART_DROP_RATIO.size())];
        
        Consumable* c = World::Instance().CreateConsumable(consumableType, Position - sf::Vector2f(0.0f, 4.0f));

        //Lanzar el corazon en una dirección aleatoria
        float throwAngle = Random::GlobalInstance.NextFloat(sf::PI_OVER_4, sf::PI - sf::PI_OVER_4);
        sf::Vector2f direction = sf::Vector2f(std::cos(throwAngle), -std::sin(throwAngle));
        c->Velocity = direction * DROP_FORCE;
    }
}


void Enemy::Draw(float tickPercent)
{
    E2D::Renderer::Instance().Draw(GetInterpolatedPosition(tickPercent), sprite);
}

void Enemy::OnCollision(CollidableEntity* other, const CollisionData& data)
{
    other->OnCollision(this, data);
}

void Enemy::OnCollision(Bullet* bullet, [[maybe_unused]] const CollisionData& data)
{
    if(!bullet->HasCollided)
    {
        bullet->HasCollided = true;
        Velocity += bullet->Direction * bullet->KnockBack * knockbackResistance;
        TakeDamage(bullet->Damage);
    }
}

void Enemy::OnCollision(HurtZone* hurtZone, const CollisionData& data)
{
    Velocity += data.Normal * hurtZone->KnockBack * knockbackResistance;
    TakeDamage(hurtZone->Damage);
}

void Enemy::TakeDamage(float amount)
{
    health -= amount * armor;
}