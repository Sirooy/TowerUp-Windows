#include "ProximitySpikeTrap.h"
#include "../Engine2D/MediaContent.h"
#include "../Physics/PhysicsEngine.h"
#include "../Physics/AABB.h"
#include "Player.h"
#include "World.h"

ProximitySpikeTrap::ProximitySpikeTrap(sf::Vector2f position, TrapDirection direction, 
    const std::string& framesName, float triggerTime, float deactivateTime, int32_t damage) : 
    WallTrap(position, direction, 0.0f), triggerAnimation(), stateMachine(), hurtZone(damage, 500.0f, 0.0f, 0.0f), 
    player(0), triggerTime(triggerTime), currentTriggerTime(0.0f), deactivateTime(deactivateTime), 
    currentDeactivateTime(0.0f), halfWidth(0), halfHeight(0)
{
    triggerAnimation.SetFrames(E2D::MediaContent::Instance().GetAnimationFrames(framesName));
    triggerAnimation.SetDuration(0.2f);
    triggerAnimation.SetLoop(false);
    triggerAnimation.Play(sprite);

    SetTrapIntoWall(triggerAnimation.GetFrame(0), direction);

    stateMachine.SetState(State::TriggeredOff, &ProximitySpikeTrap::UpdateTriggeredOff);
    stateMachine.SetState(State::GettingTriggered, &ProximitySpikeTrap::UpdateGettingTriggered);
    stateMachine.SetState(State::DetectPlayer, &ProximitySpikeTrap::UpdateDetectPlayer);

    player = World::Instance().GetPlayer();

    PhysicsEngine::Instance().AddStaticEntity(&hurtZone);
    hurtZone.Position = Position;
    hurtZone.IsActive = false;

    CalculateSize();
    CreateSounds();
    
}

void ProximitySpikeTrap::CalculateSize()
{
    AABB* collider = static_cast<AABB*>(hurtZone.GetCollider());

    if(direction == TrapDirection::Top || direction == TrapDirection::Bottom)
    {
        collider->Width = triggerAnimation.GetFrame(0).width * 0.8f;
        collider->Height = triggerAnimation.GetFrame(0).height;
    }
    else
    {
        collider->Width = triggerAnimation.GetFrame(0).height;
        collider->Height =  triggerAnimation.GetFrame(0).width * 0.8f;
    }

    halfWidth = collider->Width * 0.5f;
    halfHeight = collider->Height * 0.5f;
}

void ProximitySpikeTrap::Update(float dt)
{
    stateMachine.Update(this, dt);
}

void ProximitySpikeTrap::UpdateDetectPlayer([[maybe_unused]] float dt)
{
    if(Position.x + halfWidth > player->Position.x - Player::WIDTH * 0.5f && 
       Position.x - halfWidth < player->Position.x + Player::WIDTH * 0.5f &&
       Position.y + halfHeight > player->Position.y - Player::HEIGHT * 0.5f && 
       Position.y - halfHeight < player->Position.y + Player::HEIGHT * 0.5f)
    {
        currentTriggerTime = 0.0f;
        triggerAnimation.SetCurrentFrameIndex(1, sprite);
        stateMachine.ChangeState(State::GettingTriggered);
        PlaySound(sounds[SoundIndex::TriggerSound]);
    }
}

void ProximitySpikeTrap::UpdateGettingTriggered(float dt)
{
    if(currentTriggerTime >= triggerTime)
    {
        hurtZone.IsActive = true;
        currentDeactivateTime = 0.0f;
        stateMachine.ChangeState(State::TriggeredOff);
        PlaySound(sounds[SoundIndex::ActivateSound]);
    }

    currentTriggerTime += dt;
}

void ProximitySpikeTrap::UpdateTriggeredOff(float dt)
{
    triggerAnimation.Update(sprite, dt);

    if(currentDeactivateTime >= deactivateTime)
    {
        hurtZone.IsActive = false;
        triggerAnimation.Play(sprite);
        stateMachine.ChangeState(State::DetectPlayer);
    }

    currentDeactivateTime += dt;
}

void ProximitySpikeTrap::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& trigger = sounds[SoundIndex::TriggerSound];
    trigger.SetBuffer(*media.GetSoundBuffer("ProximitySpikeTrapTrigger"));
    trigger.SetRelativeToListener(true);  
    trigger.SetVolume(35.0f);

    auto& activate = sounds[SoundIndex::ActivateSound];
    activate.SetBuffer(*media.GetSoundBuffer("ProximitySpikeTrapActivate"));
    activate.SetRelativeToListener(true);  
    activate.SetVolume(35.0f);
  
}

ProximitySpikeTrap::~ProximitySpikeTrap()
{
    PhysicsEngine::Instance().RemoveStaticEntity(&hurtZone);
}

