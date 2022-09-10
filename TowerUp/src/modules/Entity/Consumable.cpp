#include "Consumable.h"

#include "../Engine2D/Renderer.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/PhysicsEngine.h"

Consumable::Consumable(sf::Vector2f position, float radius, float aliveTime) : DynamicEntity(position, radius), 
    sprite(), animations(), stateMachine(), sound(), aliveTime(aliveTime), blinkTime(0.0f)
{
    this->Friction = 1.0f;
    this->BaseAcceleration = PhysicsEngine::Instance().BaseGravity;
    this->AddCollisionType(CollisionType::ConsumableCollisionType);
    this->AddCollisionMask(CollisionType::TileCollisionType);
    this->AddCollisionMask(CollisionType::PlayerCollisionType);

    stateMachine.SetState(State::Dissapearing, &Consumable::UpdateDissapearing);
    stateMachine.SetState(State::PickedUp, &Consumable::UpdatePickedUp);
    stateMachine.SetState(State::Default, &Consumable::UpdateDefault);

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Consumables"));
}

Consumable::Consumable(sf::Vector2f position, float width, float height, float aliveTime) : DynamicEntity(position, width, height),
    sprite(), animations(), stateMachine(), aliveTime(aliveTime), blinkTime(0.0f)
{ 
    this->Friction = 1.0f;
    this->BaseAcceleration = PhysicsEngine::Instance().BaseGravity;
    this->AddCollisionType(CollisionType::ConsumableCollisionType);
    this->AddCollisionMask(CollisionType::TileCollisionType);
    this->AddCollisionMask(CollisionType::PlayerCollisionType);
    
    stateMachine.SetState(State::Dissapearing, &Consumable::UpdateDissapearing);
    stateMachine.SetState(State::PickedUp, &Consumable::UpdatePickedUp);
    stateMachine.SetState(State::Default, &Consumable::UpdateDefault);

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Consumables"));
}

void Consumable::Update(float dt)
{
    animations.CurrentAnimation().Update(sprite, dt);
    stateMachine.Update(this, dt);

    aliveTime -= dt;
}

void Consumable::UpdateDefault([[maybe_unused]] float dt)
{
    if(aliveTime <= DEFAULT_TO_DISSAPEARING_TIME)
        stateMachine.ChangeState(State::Dissapearing);
}

void Consumable::UpdateDissapearing([[maybe_unused]]float dt)
{
    if(aliveTime <= 0.0f)
        IsDead = true;
    else
    {
        blinkTime += dt;

        if(blinkTime >= BLINK_CHANGE_TIME)
        {
            blinkTime = 0.0f;
            sprite.InvertAlpha();
        }
    }
}

void Consumable::UpdatePickedUp([[maybe_unused]] float dt)
{
    if(animations.CurrentAnimation().HasFinished())
        IsDead = true;
}


void Consumable::Draw(float tickPercent)
{
    E2D::Renderer::Instance().Draw(GetInterpolatedPosition(tickPercent), sprite);
}

void Consumable::OnCollision(CollidableEntity* other, const CollisionData& data)
{
    other->OnCollision(this, data);
}

void Consumable::OnCollision([[maybe_unused]] Player* player, [[maybe_unused]] const CollisionData& data)
{
    BaseAcceleration = sf::Vector2f(0.0f, 0.0f);
    Velocity = sf::Vector2f(0.0f, 0.0f);
    CollisionType = 0;
    CollisionMask = 0;
    sprite.SetAlpha(255);
    stateMachine.ChangeState(State::PickedUp);
    animations.ChangeAnimation(sprite, AnimationIndex::PickedUpIndex);
}