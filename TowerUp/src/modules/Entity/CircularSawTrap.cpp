#include "CircularSawTrap.h"
#include "../Engine2D/Renderer.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/PhysicsEngine.h"
#include "../Physics/Circle.h"
#include "../SFMLUtils.hpp"

CircularSawTrap::CircularSawTrap(sf::Vector2f position, TrapDirection direction, 
    float startDelay, const std::string& framesName, float sizeMultiplier) : 
    Trap(position, direction, startDelay), hurtZone(1, KNOCKBACK, 0.0f), spinAnimation(), 
    currentDirection(0.0f, 0.0f), positionPoints(), currentPositionPoint(0), speed(0.0f)
{
    auto& physics = PhysicsEngine::Instance();

    spinAnimation.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames(framesName));
    spinAnimation.SetDuration(0.4f);
    spinAnimation.Play(sprite);

    stateMachine.SetState(State::Moving, &CircularSawTrap::UpdateMoving);
    stateMachine.SetState(State::Delay, &CircularSawTrap::UpdateDelay);

    sf::IntRect firstFrame = spinAnimation.GetFrame(0);
    sprite.setOrigin(firstFrame.width * 0.5f, firstFrame.height * 0.5f);
    sprite.setScale(sizeMultiplier, sizeMultiplier);

    Circle* collider = static_cast<Circle*>(hurtZone.GetCollider());
    collider->Radius = (firstFrame.width * 0.5f) * sizeMultiplier;
    physics.AddStaticEntity(&hurtZone);

    CalculatePositions();
    CreateSounds();
    hurtZone.Position = position;
}

void CircularSawTrap::SetDamage(int32_t damage)
{
    hurtZone.PlayerDamage = damage;
}

void CircularSawTrap::SetSpeed(float speed)
{
    this->speed = speed;
}

void CircularSawTrap::CalculatePositions()
{
    auto& physics = PhysicsEngine::Instance();
    Circle* collider = static_cast<Circle*>(hurtZone.GetCollider());

    sf::Vector2f rayDirection = sf::Vector2f(0.0f, 0.0f);
    switch (direction)
    {
        case TrapDirection::Top:    
            rayDirection.y = -1.0f;
            break; 
        case TrapDirection::Right:  
            rayDirection.x =  1.0f; 
            break; 
        case TrapDirection::Bottom: 
            rayDirection.y =  1.0f;
            break;
        case TrapDirection::Left:   
            rayDirection.x = -1.0f;
            break;
    }
    currentDirection = rayDirection;

    auto pos1Data = physics.RayCastTileMap(Position, rayDirection);
    auto pos2Data = physics.RayCastTileMap(Position,-rayDirection);

    if(pos1Data.Collision)
        positionPoints[0] = (Position + pos1Data.Distance * rayDirection) - 
            collider->Radius * rayDirection; 

    if(pos2Data.Collision)
        positionPoints[1] = (Position - pos2Data.Distance * rayDirection) + 
            collider->Radius * rayDirection; 
}

void CircularSawTrap::UpdateDelay(float dt)
{
    startDelay -= dt;
    spinAnimation.Update(sprite, dt);

    if(startDelay <= 0.0f)
        stateMachine.ChangeState(State::Moving);
}

void CircularSawTrap::UpdateMoving(float dt)
{
    LastPosition = Position;

    sf::Vector2f velocity = currentDirection * speed * dt;

    float remainingDistanceSquared = sf::lengthSquared(Position - positionPoints[currentPositionPoint]);
    if(sf::lengthSquared(velocity) > remainingDistanceSquared)
    {
        Position = positionPoints[currentPositionPoint];
        currentDirection = -currentDirection;
        currentPositionPoint = (currentPositionPoint + 1) % 2;

        PlaySound(sounds[SoundIndex::Collision]);
    }
    else
    {
        Position += velocity;
    }


    hurtZone.Position = Position;
    spinAnimation.Update(sprite, dt);
}

void CircularSawTrap::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& collision = sounds[SoundIndex::Collision];
    collision.SetBuffer(*media.GetSoundBuffer("CircularSawTrapCollision"));
    collision.SetRelativeToListener(false);
    collision.SetVolume(200.0f);
    collision.SetMinDIstance(10.5f);
    collision.SetAttenuation(15.0f);
}

void CircularSawTrap::Update(float dt)
{
    stateMachine.Update(this, dt);
}

void CircularSawTrap::Draw(float tickPercent)
{
    E2D::Renderer::Instance().Draw(GetInterpolatedPosition(tickPercent), sprite);
}

CircularSawTrap::~CircularSawTrap()
{
    PhysicsEngine::Instance().RemoveStaticEntity(&hurtZone);
}