#include "RogueKnight.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../Physics/CollisionData.h"
#include "../../Engine2D/MediaContent.h"
#include "../../Engine2D/Renderer.h"
#include "../../CollisionLayer.h"
#include "../../SFMLUtils.hpp"
#include "../../Random.h"
#include "../Player.h"
#include "../World.h"

//! BORRAR
#include <iostream>

// BOSS
RogueKnight::RogueKnight(sf::Vector2f position) : 
    Boss(position, SPRITE_WIDTH * SIZE_MULTIPLIER, SPRITE_HEIGHT * SIZE_MULTIPLIER, BASE_HEALTH, 2), 
    hurtZone{2, 600.0f, SPRITE_WIDTH * SIZE_MULTIPLIER}
{
    CreateAnimations();
    CreateSounds();
    CreateStateMachine();
    CreateActionStateMachine();
    CreateDefaultBulletGenerator();
    CreateActionVectors();

    BaseAcceleration = PhysicsEngine::BaseGravity;
    this->Friction = 0.01f;
    this->RemoveCollisionMask(PlayerBulletCollisionType);
    this->RemoveCollisionMask(ExplosionCollisionType);
    knockbackResistance = 0.0f;
    physics.AddStaticEntity(&hurtZone);
    

    sf::IntRect spriteRect = animations.CurrentAnimation().GetFrame(0);
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("RogueKnight"));
    sprite.setOrigin(spriteRect.width * 0.5f, spriteRect.height * 0.5f);
    sprite.setScale(SIZE_MULTIPLIER, SIZE_MULTIPLIER);

    if(player.Position.x < Position.x)
    {
        sprite.SetFlipHorizontally(true);
    }
}

void RogueKnight::CreateDefaultBulletGenerator()
{
    defaultBulletGenerator = new BulletGenerator();
    SetBulletGeneratorBaseData(defaultBulletGenerator);

    auto& media = E2D::MediaContent::Instance();

    defaultBulletGenerator->BulletTexture = media.GetTexture("Bullets");
    defaultBulletGenerator->BulletAnimationFrames[0] = media.GetAnimationFrames("BigSpellBulletAlive");
    defaultBulletGenerator->BulletAnimationFrames[1] = media.GetAnimationFrames("KamekamehaBulletDead");
    defaultBulletGenerator->BulletAnimationDurations[0] = 0.5f;
    defaultBulletGenerator->BulletAnimationDurations[1] = 0.5f;
    defaultBulletGenerator->BulletSpeed = 600.0f;
    defaultBulletGenerator->BulletSize = 12.0f;
    defaultBulletGenerator->BulletKnockBack = 500.0f;
    defaultBulletGenerator->BulletMaxAliveTime = 12.0f;
    defaultBulletGenerator->BulletType = BulletType::Default;
    defaultBulletGenerator->BulletPlayerDamage = 1;
    defaultBulletGenerator->BulletTexturePadding = 18.0f;
}

void RogueKnight::UpdateClones(float dt)
{
    for(size_t i = 0;i < clones.size(); ++i)
        clones[i].Update(dt);
}

void RogueKnight::DrawClones(float tickPercent)
{
    for(size_t i = 0;i < clones.size(); ++i)
        clones[i].Draw(tickPercent);
}

void RogueKnight::RemoveDeadClones()
{
    size_t i = 0;
    while(i < clones.size())
    {
        if(clones[i].IsDead)
            clones.erase(clones.begin() + i);
        else
            ++i;
    }
}

void RogueKnight::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::IdleIndex];
    idle.SetFrames(media.GetAnimationFrames("RogueKnightIdle"));
    idle.SetDuration(IDLE_ANIMATION_DURATION);

    auto& run = animations[AnimationIndex::RunIndex];
    run.SetFrames(media.GetAnimationFrames("RogueKnightRun"));
    run.SetDuration(RUN_ANIMATION_DURATION);

    auto& jump = animations[AnimationIndex::JumpIndex];
    jump.SetFrames(media.GetAnimationFrames("RogueKnightJump"));
    jump.SetDuration(JUMP_ANIMATION_DURATION);

    auto& magicAttack = animations[AnimationIndex::MagicAttackIndex];
    magicAttack.SetFrames(media.GetAnimationFrames("RogueKnightMagicAttack"));
    magicAttack.SetDuration(MAGIC_ATTACK_ANIMATION_DURATION);
    magicAttack.SetLoop(false);

    auto& singleAttack = animations[AnimationIndex::SlashAttackIndex];
    singleAttack.SetFrames(media.GetAnimationFrames("RogueKnightSingleSwordAttack"));
    singleAttack.SetDuration(SLASH_ATTACK_ANIMATION_DURATION);

    auto& comboAttack = animations[AnimationIndex::SlashComboAttackIndex];
    comboAttack.SetFrames(media.GetAnimationFrames("RogueKnightComboSwordAttack"));
    comboAttack.SetDuration((comboAttack.GetNumFrames() * SLASH_ATTACK_ANIMATION_DURATION) / singleAttack.GetNumFrames());

    auto& dash1 = animations[AnimationIndex::Dash1Index];
    dash1.SetFrames(media.GetAnimationFrames("RogueKnightDash1"));
    dash1.SetDuration(DASH_ANIMATION_DURATION);

    auto& dash2 = animations[AnimationIndex::Dash2Index];
    dash2.SetFrames(media.GetAnimationFrames("RogueKnightDash2"));
    dash2.SetDuration(DASH_ANIMATION_DURATION);

    auto& sitDown = animations[AnimationIndex::SitDownIndex];
    sitDown.SetFrames(media.GetAnimationFrames("RogueKnightDeath"));
    sitDown.SetDuration(SIT_DOWN_ANIMATION_DURATION);
    sitDown.SetLoop(false);

    auto& standUp = animations[AnimationIndex::StandUpIndex];
    standUp.SetFrames(media.GetAnimationFrames("RogueKnightDeath"));
    standUp.SetDuration(STAND_UP_ANIMATION_DURATION);
    standUp.SetLoop(false);
    standUp.SetReversed(true);

    auto& fly = animations[AnimationIndex::FlyIndex];
    fly.SetFrames(media.GetAnimationFrames("RogueKnightFly"));
    fly.SetDuration(FLY_ANIMATION_DURATION);

    animations.ChangeAnimation(sprite, AnimationIndex::StandUpIndex);
}

void RogueKnight::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& die = sounds[SoundIndex::DieSound];
    die.SetBuffer(*media.GetSoundBuffer("RogueKnightDie"));
    die.SetRelativeToListener(true);
    die.SetVolume(35.0f);

    auto& blade = sounds[SoundIndex::BladeSound];
    blade.SetBuffer(*media.GetSoundBuffer("RogueKnightBlade"));
    blade.SetRelativeToListener(true);
    blade.SetVolume(35.0f);

    auto& summon = sounds[SoundIndex::SummonSound];
    summon.SetBuffer(*media.GetSoundBuffer("RogueKnightSummon"));
    summon.SetRelativeToListener(true);
    summon.SetVolume(35.0f);

    auto& shoot = sounds[SoundIndex::ShootSound];
    shoot.SetBuffer(*media.GetSoundBuffer("RogueKnightShoot"));
    shoot.SetRelativeToListener(true);
    shoot.SetVolume(35.0f);


}

void RogueKnight::CreateStateMachine()
{
    stateMachine.AddState(State::PreparingAction, &RogueKnight::PreparingAction, 
        &RogueKnight::OnEnterPreparingAction, nullptr);
    stateMachine.AddState(State::PerformingAction, &RogueKnight::PerformingAction, nullptr, nullptr);
    stateMachine.AddState(State::EnteringSecondPhase, nullptr, &RogueKnight::OnEnterSecondPhase, nullptr);
    stateMachine.AddState(State::EnteringThirdPhase, nullptr, &RogueKnight::OnEnterThirdPhase, nullptr);
    stateMachine.AddState(State::Dying, &RogueKnight::Dying, &RogueKnight::OnEnterDying, nullptr);

    stateMachine.AddTransitionToState(State::PreparingAction, State::PerformingAction, 
        &RogueKnight::ToPerformingAction);
    stateMachine.AddTransitionToState(State::PerformingAction, State::PreparingAction,
        &RogueKnight::ToPreparingAction);

    stateMachine.AddTransitionToAllStates(State::Dying, &RogueKnight::ToDying);
    stateMachine.AddTransitionToState(State::PreparingAction, State::EnteringThirdPhase, &RogueKnight::ToThirdPhase);
    stateMachine.AddTransitionToState(State::PreparingAction, State::EnteringSecondPhase, &RogueKnight::ToSecondPhase);
    stateMachine.AddTransitionToState(State::PerformingAction, State::EnteringThirdPhase, &RogueKnight::ToThirdPhase);
    stateMachine.AddTransitionToState(State::PerformingAction, State::EnteringSecondPhase, &RogueKnight::ToSecondPhase);

    stateMachine.SetCurrentState(State::PerformingAction);
}

void RogueKnight::EnqueuePhaseAction(const std::vector<Action>& vec)
{
    Action action = vec[Random::GlobalInstance.NextInt(vec.size())];
    actions.EnqueueAction(action);
}

void RogueKnight::CreateActionStateMachine()
{
    //ACCIONES BASE
    actions.AddAction(Action::PlayIdleAnim, &RogueKnight::PlayAnimation<IdleIndex>);
    actions.AddAction(Action::PlayRunAnim, &RogueKnight::PlayAnimation<RunIndex>);
    actions.AddAction(Action::PlayJumpAnim, &RogueKnight::PlayAnimation<JumpIndex>);
    actions.AddAction(Action::PlayMagicAttackAnim, &RogueKnight::PlayAnimation<MagicAttackIndex>);
    actions.AddAction(Action::PlaySlashAttackAnim, &RogueKnight::PlayAnimation<SlashAttackIndex>);
    actions.AddAction(Action::PlaySlashComboAttackAnim, &RogueKnight::PlayAnimation<SlashComboAttackIndex>);
    actions.AddAction(Action::PlayDash1Anim, &RogueKnight::PlayAnimation<Dash1Index>);
    actions.AddAction(Action::PlayDash2Anim, &RogueKnight::PlayAnimation<Dash2Index>);
    actions.AddAction(Action::PlaySitDownAnim, &RogueKnight::PlayAnimation<SitDownIndex>);
    actions.AddAction(Action::PlayStandUpAnim, &RogueKnight::PlayAnimation<StandUpIndex>);
    actions.AddAction(Action::PlayFlyAnim, &RogueKnight::PlayAnimation<FlyIndex>);
    actions.AddAction(Action::PauseAtFrame7, &RogueKnight::PauseAtFrame<7>);

    actions.AddAction(Action::PauseAnimation, &RogueKnight::PauseAnimation);
    actions.AddAction(Action::ResumeAnimation, &RogueKnight::ResumeAnimation);
    actions.AddAction(Action::SetOrientationToPlayer, & RogueKnight::SetOrientationToPlayer);
    actions.AddAction(Action::WaitAnimationEnd, &RogueKnight::WaitAnimationEnd);
    actions.AddAction(Action::ResetActionTimer, &RogueKnight::ResetActionTimer);
    actions.AddAction(Action::SetSpawnEnemiesVariables, &RogueKnight::SetSpawnEnemiesVariables);
    actions.AddAction(Action::SetShootToPlayerVariables, &RogueKnight::SetShootToPlayerVariables);

    actions.AddAction(Action::AddPlayerBulletCollisionMask, &RogueKnight::AddCollisionMaskAction<PlayerBulletCollisionType>);
    actions.AddAction(Action::AddExplosionCollisionMask, &RogueKnight::AddCollisionMaskAction<ExplosionCollisionType>);
    actions.AddAction(Action::RemovePlayerBulletCollisionMask, &RogueKnight::RemoveCollisionMaskAction<PlayerBulletCollisionType>);
    actions.AddAction(Action::RemoveExplosionCollisionMask, &RogueKnight::RemoveCollisionMaskAction<ExplosionCollisionType>);

    actions.AddAction(Action::Dissapear, &RogueKnight::Dissapear);
    actions.AddAction(Action::Reapear, &RogueKnight::Reapear);
    actions.AddAction(Action::FastDissapear, &RogueKnight::FastDissapear);
    actions.AddAction(Action::FastReapear, &RogueKnight::FastReapear);
    actions.AddAction(Action::InstantDissapear, &RogueKnight::InstantDissapear);
    actions.AddAction(Action::InstantReapear, &RogueKnight::InstantReapear);
    
    actions.AddAction(Action::WaitSlash, &RogueKnight::WaitSlash);
    actions.AddAction(Action::WaitSlow, &RogueKnight::WaitSlow);
    actions.AddAction(Action::WaitFast, &RogueKnight::WaitFast);
    actions.AddAction(Action::WaitEnemiesDeath, &RogueKnight::WaitEnemiesDeath);
    actions.AddAction(Action::EnableHurtZone , &RogueKnight::EnableHurtZone);
    actions.AddAction(Action::DisableHurtZone, &RogueKnight::DisableHurtZone);

    actions.AddAction(Action::RunAway, &RogueKnight::RunAway);
    actions.AddAction(Action::RunToPlayer, &RogueKnight::RunToPlayer);
    actions.AddAction(Action::DashAway, &RogueKnight::DashAway);
    actions.AddAction(Action::DashToPlayer, &RogueKnight::DashToPlayer);

    actions.AddAction(Action::FirstSlash, &RogueKnight::FirstSlash);
    actions.AddAction(Action::SecondSlash, &RogueKnight::SecondSlash);
    actions.AddAction(Action::ThirdSlash, &RogueKnight::ThirdSlash);
    actions.AddAction(Action::PerformSlashDash, &RogueKnight::PerformSlashDash);
    actions.AddAction(Action::PerformReverseSlashDash, &RogueKnight::PerformReverseSlashDash);

    actions.AddAction(Action::SpawnEnemies, &RogueKnight::SpawnEnemies);

    actions.AddAction(Action::TeleportInFrontOfPlayerAim, &RogueKnight::TeleportInFrontOfPlayerAim);
    actions.AddAction(Action::TeleportBehindPlayerAim, &RogueKnight::TeleportBehindPlayerAim);
    actions.AddAction(Action::TeleportBehindPlayer, &RogueKnight::TeleportBehindPlayer);
    actions.AddAction(Action::TeleportAway, &RogueKnight::TeleportAway);
    actions.AddAction(Action::TeleportAwayBehindPlayer, &RogueKnight::TeleportAwayBehindPlayer);
    actions.AddAction(Action::TeleportAwayToCorner, &RogueKnight::TeleportAwayToCorner);
    actions.AddAction(Action::TeleportToFarthestCorner, &RogueKnight::TeleportToFarthestCorner);

    actions.AddAction(Action::ChangeColorToSecondPhase, &RogueKnight::ChangeColorToSecondPhase);
    actions.AddAction(Action::ChangeColorToThirdPhase, &RogueKnight::ChangeColorToThirdPhase);

    actions.AddAction(Action::ShootToPlayer, &RogueKnight::ShootToPlayer);
    actions.AddAction(Action::CreateClone, &RogueKnight::CreateClone);

    //ACCIONES COMPUESTAS
    actions.AddAction(Action::StandUp, &RogueKnight::StandUp);
    actions.AddAction(Action::SitDown, &RogueKnight::SitDown);

    //! FASE 1
    //CORTO ALCANCE
    actions.AddAction(Action::Phase1CloseRange1, &RogueKnight::Phase1CloseRange1);
    actions.AddAction(Action::Phase1CloseRange2, &RogueKnight::Phase1CloseRange2);
    actions.AddAction(Action::Phase1CloseRange3, &RogueKnight::Phase1CloseRange3);
    actions.AddAction(Action::Phase1CloseRange4, &RogueKnight::Phase1CloseRange4);
    //MEDIO ALCANCE
    actions.AddAction(Action::Phase1MidRange1, &RogueKnight::Phase1MidRange1);
    actions.AddAction(Action::Phase1MidRange2, &RogueKnight::Phase1MidRange2);
    actions.AddAction(Action::Phase1MidRange3, &RogueKnight::Phase1MidRange3);
    actions.AddAction(Action::Phase1MidRange4, &RogueKnight::Phase1MidRange4);
    //LARGO ALCANCE
    actions.AddAction(Action::Phase1LongRange1, &RogueKnight::Phase1LongRange1);
    actions.AddAction(Action::Phase1LongRange2, &RogueKnight::Phase1LongRange2);
    actions.AddAction(Action::Phase1LongRange3, &RogueKnight::Phase1LongRange3);

    //! FASE 2
    //CORTO ALCANCE
    actions.AddAction(Action::Phase2CloseRange1, &RogueKnight::Phase2CloseRange1);
    actions.AddAction(Action::Phase2CloseRange2, &RogueKnight::Phase2CloseRange2);
    actions.AddAction(Action::Phase2CloseRange3, &RogueKnight::Phase2CloseRange3);
    actions.AddAction(Action::Phase2CloseRange4, &RogueKnight::Phase2CloseRange4);
    //MEDIO ALCANCE
    actions.AddAction(Action::Phase2MidRange1, &RogueKnight::Phase2MidRange1);
    actions.AddAction(Action::Phase2MidRange2, &RogueKnight::Phase2MidRange2);
    actions.AddAction(Action::Phase2MidRange3, &RogueKnight::Phase2MidRange3);
    actions.AddAction(Action::Phase2MidRange4, &RogueKnight::Phase2MidRange4);
    //LARGO ALCANCE
    actions.AddAction(Action::Phase2LongRange1, &RogueKnight::Phase2LongRange1);
    actions.AddAction(Action::Phase2LongRange2, &RogueKnight::Phase2LongRange2);
    actions.AddAction(Action::Phase2LongRange3, &RogueKnight::Phase2LongRange3);
    //INVISIBILIDAD
    actions.AddAction(Action::Phase2Invisible1, &RogueKnight::Phase2Invisible1);
    actions.AddAction(Action::Phase2Invisible2, &RogueKnight::Phase2Invisible2);
    actions.AddAction(Action::Phase2Invisible3, &RogueKnight::Phase2Invisible3);

    //! FASE 3
    //CORTO ALCANCE
    actions.AddAction(Action::Phase3CloseRange1, &RogueKnight::Phase3CloseRange1);
    actions.AddAction(Action::Phase3CloseRange2, &RogueKnight::Phase3CloseRange2);
    actions.AddAction(Action::Phase3CloseRange3, &RogueKnight::Phase3CloseRange3);
    actions.AddAction(Action::Phase3CloseRange4, &RogueKnight::Phase3CloseRange4);
    //MEDIO ALCANCE
    actions.AddAction(Action::Phase3MidRange1, &RogueKnight::Phase3MidRange1);
    actions.AddAction(Action::Phase3MidRange2, &RogueKnight::Phase3MidRange2);
    actions.AddAction(Action::Phase3MidRange3, &RogueKnight::Phase3MidRange3);
    actions.AddAction(Action::Phase3MidRange4, &RogueKnight::Phase3MidRange4);
    //LARGO ALCANCE
    actions.AddAction(Action::Phase3LongRange1, &RogueKnight::Phase3LongRange1);
    actions.AddAction(Action::Phase3LongRange2, &RogueKnight::Phase3LongRange2);
    actions.AddAction(Action::Phase3LongRange3, &RogueKnight::Phase3LongRange3);
    //INVISIBILIDAD
    actions.AddAction(Action::Phase3Invisible1, &RogueKnight::Phase3Invisible1);
    actions.AddAction(Action::Phase3Invisible2, &RogueKnight::Phase3Invisible2);
    actions.AddAction(Action::Phase3Invisible3, &RogueKnight::Phase3Invisible3);
    actions.AddAction(Action::Phase3Invisible4, &RogueKnight::Phase3Invisible4);

    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::AddPlayerBulletCollisionMask);
    actions.EnqueueAction(Action::AddExplosionCollisionMask);
}

void RogueKnight::CreateActionVectors()
{
    //! FASE 1
    auto phase1Actions = phaseActions[0];
    auto& phase1CloseRange = phase1Actions[ActionIndex::CloseRangeActionIndex];
    phase1CloseRange.reserve(4);
    phase1CloseRange.push_back(Action::Phase1CloseRange1);
    phase1CloseRange.push_back(Action::Phase1CloseRange2);
    phase1CloseRange.push_back(Action::Phase1CloseRange3);
    phase1CloseRange.push_back(Action::Phase1CloseRange4);

    auto& phase1MidRange = phase1Actions[ActionIndex::MidRangeActionIndex];
    phase1MidRange.reserve(4);
    phase1MidRange.push_back(Action::Phase1MidRange1);
    phase1MidRange.push_back(Action::Phase1MidRange2);
    phase1MidRange.push_back(Action::Phase1MidRange3);
    phase1MidRange.push_back(Action::Phase1MidRange4);

    auto& phase1LongRange = phase1Actions[ActionIndex::LongRangeActionIndex];
    phase1LongRange.reserve(3);
    phase1LongRange.push_back(Action::Phase1LongRange1);
    phase1LongRange.push_back(Action::Phase1LongRange2);
    phase1LongRange.push_back(Action::Phase1LongRange3);

    //! FASE 2
    auto phase2Actions = phaseActions[1];
    auto& phase2CloseRange = phase2Actions[ActionIndex::CloseRangeActionIndex];
    phase2CloseRange.reserve(4);
    phase2CloseRange.push_back(Action::Phase2CloseRange1);
    phase2CloseRange.push_back(Action::Phase2CloseRange2);
    phase2CloseRange.push_back(Action::Phase2CloseRange3);
    phase2CloseRange.push_back(Action::Phase2CloseRange4);

    auto& phase2MidRange = phase2Actions[ActionIndex::MidRangeActionIndex];
    phase2MidRange.reserve(4);
    phase2MidRange.push_back(Action::Phase2MidRange1);
    phase2MidRange.push_back(Action::Phase2MidRange2);
    phase2MidRange.push_back(Action::Phase2MidRange3);
    phase2MidRange.push_back(Action::Phase2MidRange4);

    auto& phase2LongRange = phase2Actions[ActionIndex::LongRangeActionIndex];
    phase2LongRange.reserve(3);
    phase2LongRange.push_back(Action::Phase2LongRange1);
    phase2LongRange.push_back(Action::Phase2LongRange2);
    phase2LongRange.push_back(Action::Phase2LongRange3);

    auto& phase2Invisible = phase2Actions[ActionIndex::InvisibilityActionIndex];
    phase2Invisible.reserve(3);
    phase2Invisible.push_back(Action::Phase2Invisible1);
    phase2Invisible.push_back(Action::Phase2Invisible2);
    phase2Invisible.push_back(Action::Phase2Invisible3);

    //! FASE 3
    auto phase3Actions = phaseActions[2];
    auto& phase3CloseRange = phase3Actions[ActionIndex::CloseRangeActionIndex];
    phase3CloseRange.reserve(4);
    phase3CloseRange.push_back(Action::Phase3CloseRange1);
    phase3CloseRange.push_back(Action::Phase3CloseRange2);
    phase3CloseRange.push_back(Action::Phase3CloseRange3);
    phase3CloseRange.push_back(Action::Phase3CloseRange4);

    auto& phase3MidRange = phase3Actions[ActionIndex::MidRangeActionIndex];
    phase3MidRange.reserve(4);
    phase3MidRange.push_back(Action::Phase3MidRange1);
    phase3MidRange.push_back(Action::Phase3MidRange2);
    phase3MidRange.push_back(Action::Phase3MidRange3);
    phase3MidRange.push_back(Action::Phase3MidRange4);

    auto& phase3LongRange = phase3Actions[ActionIndex::LongRangeActionIndex];
    phase3LongRange.reserve(3);
    phase3LongRange.push_back(Action::Phase3LongRange1);
    phase3LongRange.push_back(Action::Phase3LongRange2);
    phase3LongRange.push_back(Action::Phase3LongRange3);

    auto& phase3Invisible = phase3Actions[ActionIndex::InvisibilityActionIndex];
    phase3Invisible.reserve(3);
    phase3Invisible.push_back(Action::Phase3Invisible1);
    phase3Invisible.push_back(Action::Phase3Invisible2);
    phase3Invisible.push_back(Action::Phase3Invisible3);
    phase3Invisible.push_back(Action::Phase3Invisible4);
}

void RogueKnight::SetHurtZonePosition()
{
    if(sprite.GetFlipHorizontally()) //Mira hacia la izquierda
        hurtZone.Position = Position - sf::Vector2f(HURTZONE_DISTANCE, 0.0f);
    else
        hurtZone.Position = Position + sf::Vector2f(HURTZONE_DISTANCE, 0.0f);
}

void RogueKnight::Update(float dt)
{
    stateMachine.Update(this, dt);
    UpdateClones(dt);

    animations.CurrentAnimation().Update(sprite, dt);

    RemoveDeadClones();
}

//! ############ ESTADOS ############
void RogueKnight::PreparingAction(float dt)
{
    SetSpriteOrientation(player.Position.x - Position.x);

    prepareActionTimer += dt;
    if(prepareActionTimer >= 1.25f || currentActionsToPerform > 0)
    {
        if(prepareActionTimer >= 1.25f)
        {
            prepareActionTimer = 0.0f;
            //Establecemos el nuevo numero de acciones que tiene que realizar
            currentActionsToPerform = Random::GlobalInstance.RangeInt(minActionsPerWait, maxActionsPerWait);
        }
        --currentActionsToPerform;

        auto currentPhaseActions = phaseActions[currentPhase];
        float distSquared = sf::lengthSquared(player.Position - Position);
        if(isInvisible)
        {
            EnqueuePhaseAction(currentPhaseActions[ActionIndex::InvisibilityActionIndex]);
        }
        else if(distSquared <= CLOSE_RANGE_MAX_ATTACK_SQUARED_DISTANCE)
        {
            EnqueuePhaseAction(currentPhaseActions[ActionIndex::CloseRangeActionIndex]);
        }
        else if(distSquared <= MID_RANGE_MAX_ATTACK_SQUARED_DISTANCE)
        {
            EnqueuePhaseAction(currentPhaseActions[ActionIndex::MidRangeActionIndex]);
        }
        else //Long Range
        {
            EnqueuePhaseAction(currentPhaseActions[ActionIndex::LongRangeActionIndex]);
        }
    }
}

void RogueKnight::PerformingAction(float dt)
{
    actions.Update(this, dt, actionTimer);
}

void RogueKnight::Dying(float dt)
{
    if(animations.CurrentAnimation().HasFinished())
    {
        if(sprite.FadeOut(3.0f, dt))
            Kill();
    }
}

void RogueKnight::OnEnterPreparingAction()
{
    animations.ChangeAnimation(sprite, AnimationIndex::IdleIndex);
    prepareActionTimer = 0.0f;
}

void RogueKnight::OnEnterDying()
{
    RemoveCollisionMask(PlayerBulletCollisionType);
    RemoveCollisionMask(ExplosionCollisionType);
    animations.ChangeAnimation(sprite, AnimationIndex::SitDownIndex);
    sprite.SetAlpha(255);
    hurtZone.IsActive = false;
    PlaySound(sounds[SoundIndex::DieSound]);
}

void RogueKnight::OnEnterSecondPhase()
{
    ++currentPhase;

    sprite.SetAlpha(255);
    actions.ClearQueue();
    actions.EnqueueAction(Action::RemovePlayerBulletCollisionMask);
    actions.EnqueueAction(Action::RemoveExplosionCollisionMask);
    actions.EnqueueAction(Action::PlaySitDownAnim);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::ChangeColorToSecondPhase);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::StandUp);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::AddPlayerBulletCollisionMask);
    actions.EnqueueAction(Action::AddExplosionCollisionMask);

    hurtZone.IsActive = false;
    minActionsPerWait = 2;
    maxActionsPerWait = 3;
    currentActionsToPerform = Random::GlobalInstance.RangeInt(minActionsPerWait, maxActionsPerWait);
    prepareActionTimer = 0.0f;
    spawnBulletTime = 0.35f;
    
    stateMachine.GetState(State::PreparingAction)->Transitions.clear();
    stateMachine.GetState(State::PerformingAction)->Transitions.clear();

    stateMachine.AddTransitionToState(State::PreparingAction, State::PerformingAction, 
        &RogueKnight::ToPerformingAction);
    stateMachine.AddTransitionToState(State::PerformingAction, State::PreparingAction,
        &RogueKnight::ToPreparingAction);
    stateMachine.AddTransitionToAllStates(State::Dying, &RogueKnight::ToDying);
    stateMachine.AddTransitionToState(State::PreparingAction, State::EnteringThirdPhase, &RogueKnight::ToThirdPhase);
    stateMachine.AddTransitionToState(State::PerformingAction, State::EnteringThirdPhase, &RogueKnight::ToThirdPhase);

    stateMachine.ChangeCurrentState(this, State::PerformingAction);
}

void RogueKnight::OnEnterThirdPhase()
{
    ++currentPhase;
    
    sprite.SetAlpha(255);
    actions.ClearQueue();
    actions.EnqueueAction(Action::RemovePlayerBulletCollisionMask);
    actions.EnqueueAction(Action::RemoveExplosionCollisionMask);
    actions.EnqueueAction(Action::PlaySitDownAnim);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::ChangeColorToThirdPhase);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::StandUp);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::AddPlayerBulletCollisionMask);
    actions.EnqueueAction(Action::AddExplosionCollisionMask);

    hurtZone.IsActive = false;
    hurtZone.PlayerDamage = 3;
    minActionsPerWait = 3;
    maxActionsPerWait = 4;
    currentActionsToPerform = Random::GlobalInstance.RangeInt(minActionsPerWait, maxActionsPerWait);
    prepareActionTimer = 0.0f;
    spawnBulletTime = 0.25f;
    
    stateMachine.GetState(State::PreparingAction)->Transitions.clear();
    stateMachine.GetState(State::PerformingAction)->Transitions.clear();

    stateMachine.AddTransitionToState(State::PreparingAction, State::PerformingAction, 
        &RogueKnight::ToPerformingAction);
    stateMachine.AddTransitionToState(State::PerformingAction, State::PreparingAction,
        &RogueKnight::ToPreparingAction);
    stateMachine.AddTransitionToAllStates(State::Dying, &RogueKnight::ToDying);

    stateMachine.ChangeCurrentState(this, State::PerformingAction);
}


bool RogueKnight::ToPreparingAction()
{
    return actions.IsEmpty();
}

bool RogueKnight::ToPerformingAction()
{
    return !actions.IsEmpty();
}

bool RogueKnight::ToDying()
{
    return health <= 0.0f;
}

bool RogueKnight::ToSecondPhase()
{
    return health <= SECOND_PHASE_MIN_HEALTH;
}

bool RogueKnight::ToThirdPhase()
{
    return health <= THIRD_PHASE_MIN_HEALTH;
}

//! ############ ACCIONES BASE ############
bool RogueKnight::WaitAnimationEnd([[maybe_unused]] float dt)
{
    return animations.CurrentAnimation().HasFinished();
}

bool RogueKnight::PauseAnimation([[maybe_unused]] float dt)
{
    animations.CurrentAnimation().Pause();
    return true;
}

bool RogueKnight::ResumeAnimation([[maybe_unused]] float dt)
{
    animations.CurrentAnimation().Resume();
    return true;
}

bool RogueKnight::SetOrientationToPlayer([[maybe_unused]] float dt)
{
    float direction = player.Position.x - this->Position.x;

    SetSpriteOrientation(direction);

    return true;
}

bool RogueKnight::ResetActionTimer([[maybe_unused]] float dt)
{
    actionTimer = 0.0f;
    return true;
}

bool RogueKnight::SetSpawnEnemiesVariables([[maybe_unused]] float dt)
{
    currentEnemiesToSpawn = Random::GlobalInstance.RangeInt(minEnemiesToSpawn, maxEnemiesToSpawn);
    return true;
}

bool RogueKnight::SetShootToPlayerVariables([[maybe_unused]] float dt)
{
    currentBulletsToShoot = Random::GlobalInstance.RangeInt(3, 6);
    return true;
}

bool RogueKnight::Dissapear([[maybe_unused]] float dt)
{
    isInvisible = true;
    RemoveCollisionMask(PlayerBulletCollisionType);
    RemoveCollisionMask(ExplosionCollisionType);
    return sprite.FadeOut(DISSAPEAR_REAPEAR_DURATION, dt);
}

bool RogueKnight::Reapear([[maybe_unused]] float dt)
{
    isInvisible = false;
    
    bool fadedIn = sprite.FadeIn(DISSAPEAR_REAPEAR_DURATION, dt);
    if(fadedIn)
    {
        AddCollisionMask(PlayerBulletCollisionType);
        AddCollisionMask(ExplosionCollisionType);
    }

    return fadedIn; 
}

bool RogueKnight::FastDissapear([[maybe_unused]] float dt)
{
    isInvisible = true;
    RemoveCollisionMask(PlayerBulletCollisionType);
    RemoveCollisionMask(ExplosionCollisionType);
    return sprite.FadeOut(FAST_DISSAPEAR_REAPEAR_DURATION, dt);
}

bool RogueKnight::FastReapear([[maybe_unused]] float dt)
{
    isInvisible = false;

    bool fadedIn = sprite.FadeIn(FAST_DISSAPEAR_REAPEAR_DURATION, dt);
    if(fadedIn)
    {
        AddCollisionMask(PlayerBulletCollisionType);
        AddCollisionMask(ExplosionCollisionType);
    }

    return fadedIn;
}

bool RogueKnight::InstantDissapear(float dt)
{
    isInvisible = true;
    RemoveCollisionMask(PlayerBulletCollisionType);
    RemoveCollisionMask(ExplosionCollisionType);
    return sprite.FadeOut(INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
}

bool RogueKnight::InstantReapear(float dt)
{
    isInvisible = false;

    bool fadedIn = sprite.FadeIn(INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
    if(fadedIn)
    {
        AddCollisionMask(PlayerBulletCollisionType);
        AddCollisionMask(ExplosionCollisionType);
    }

    return fadedIn;
}


bool RogueKnight::WaitSlash(float dt)
{
    actionTimer += dt;

    return actionTimer >= slashDelayWaitTime;
}

bool RogueKnight::WaitSlow(float dt)
{
    actionTimer += dt;

    return actionTimer >= slowWaitTime;
}

bool RogueKnight::WaitFast([[maybe_unused]] float dt)
{
    actionTimer += dt;

    return actionTimer >= fastWaitTime;
}

bool RogueKnight::WaitEnemiesDeath([[maybe_unused]] float dt)
{
    return World::Instance().GetEnemies().size() == 1; //Cuando solo queda el boss, devuelve verdadero
}

bool RogueKnight::EnableHurtZone([[maybe_unused]] float dt)
{
    hurtZone.IsActive = true;

    return true;
}

bool RogueKnight::DisableHurtZone([[maybe_unused]] float dt)
{
    hurtZone.IsActive = false;

    return true;
}

bool RogueKnight::RunAway(float dt)
{
    float xDistance = this->Position.x - player.Position.x;
    float xDirection = sf::sign(xDistance);

    this->Velocity.x += xDirection * runSpeed * dt;

    SetSpriteOrientation(xDirection);

    actionTimer += dt;

    return actionTimer >= MAX_RUN_DURUATION || xDistance * xDistance >= RUN_AWAY_MAX_DISTANCE_SQUARED;
}

bool RogueKnight::RunToPlayer(float dt)
{
    float xDistance = player.Position.x - this->Position.x;
    float xDirection = sf::sign(xDistance);

    this->Velocity.x += xDirection * runSpeed * dt;

    SetSpriteOrientation(xDirection);

    actionTimer += dt;

    return actionTimer >= MAX_RUN_DURUATION || xDistance * xDistance <= RUN_TO_PLAYER_MIN_DISTANCE_SQUARED;
}

bool RogueKnight::DashAway(float dt)
{
    float xDistance = this->Position.x - player.Position.x;
    float xDirection = sf::sign(xDistance);

    this->Velocity.x += xDirection * dashSpeed * dt;

    SetSpriteOrientation(xDirection);

    actionTimer += dt;

    if(actionTimer >= MAX_DASH_DURATION || xDistance * xDistance >= DASH_AWAY_MAX_DISTANCE_SQUARED)
    {
        Velocity.x = 0.0f;

        return true;
    }

    return false;
}

bool RogueKnight::DashToPlayer(float dt)
{
    float xDistance = player.Position.x - this->Position.x;
    float xDirection = sf::sign(xDistance);

    this->Velocity.x += xDirection * dashSpeed * dt;

    SetSpriteOrientation(xDirection);

    actionTimer += dt;

    if(actionTimer >= MAX_DASH_DURATION || xDistance * xDistance <= DASH_TO_PLAYER_MIN_DISTANCE_SQUARED)
    {
        Velocity.x = 0.0f;

        return true;
    }

    return false;
}

bool RogueKnight::FirstSlash([[maybe_unused]] float dt)
{
    SetHurtZonePosition();

    int32_t frameIndex = animations.CurrentAnimation().GetCurrentFrameIndex();
    if(frameIndex >= FIRST_SLASH_HITBOX_FIRST_FRAME && frameIndex <= FIRST_SLASH_HITBOX_LAST_FRAME)
    {
        if(animations.CurrentAnimation().HasFinishedFrame())
            PlaySound(sounds[SoundIndex::BladeSound]);
        hurtZone.IsActive = true;
        return false;
    }

    hurtZone.IsActive = false;

    return frameIndex >= FIRST_SLASH_LAST_FRAME;
}

bool RogueKnight::SecondSlash([[maybe_unused]] float dt)
{
    SetHurtZonePosition();

    int32_t frameIndex = animations.CurrentAnimation().GetCurrentFrameIndex();
    if(frameIndex >= SECOND_SLASH_HITBOX_FIRST_FRAME && frameIndex <= SECOND_SLASH_HITBOX_LAST_FRAME)
    {
        if(animations.CurrentAnimation().HasFinishedFrame())
            PlaySound(sounds[SoundIndex::BladeSound]);

        hurtZone.IsActive = true;
        return false;
    }

    hurtZone.IsActive = false;

    return frameIndex >= SECOND_SLASH_LAST_FRAME;
}

bool RogueKnight::ThirdSlash([[maybe_unused]] float dt)
{
    SetHurtZonePosition();

    int32_t frameIndex = animations.CurrentAnimation().GetCurrentFrameIndex();
    if(frameIndex >= THIRD_SLASH_HITBOX_FIRST_FRAME && frameIndex <= THIRD_SLASH_HITBOX_LAST_FRAME)
    {
        if(animations.CurrentAnimation().HasFinishedFrame())
            PlaySound(sounds[SoundIndex::BladeSound]);

        hurtZone.IsActive = true;
        return false;
    }

    hurtZone.IsActive = false;

    return animations.CurrentAnimation().HasFinished();
}

bool RogueKnight::PerformSlashDash([[maybe_unused]] float dt)
{
    /*
    float xDistance = player.Position.x - this->Position.x;
    float xDirection = sf::sign(xDistance);
    Velocity.x += xDirection * force;
    */

    sf::Vector2f direction = sf::getNormalized(player.Position - this->Position);
    Velocity.x += sf::sign(direction.x) * SLASH_BASE_FORCE;

    if(!player.IsOnGround())
        Velocity.y += direction.y * SLASH_BASE_FORCE * 2.0f;

    SetSpriteOrientation(Velocity.x);

    return true;
}

bool RogueKnight::PerformReverseSlashDash([[maybe_unused]] float dt)
{
    sf::Vector2f direction = sf::getNormalized(player.Position - this->Position);
    Velocity.x -= sf::sign(direction.x) * SLASH_BASE_FORCE;

    if(!player.IsOnGround())
        Velocity.y += direction.y * SLASH_BASE_FORCE * 2.0f;

    SetSpriteOrientation(Velocity.x);

    return true;
}

bool RogueKnight::SpawnEnemies(float dt)
{
    actionTimer += dt;

    SetOrientationToPlayer(dt);

    if(actionTimer >= spawnEnemyTime)
    {
        PlaySound(sounds[SoundIndex::SummonSound]);
        actionTimer = 0.0f;
        --currentEnemiesToSpawn;

        sf::Vector2f offset = sf::Vector2f(SPRITE_WIDTH * 0.4f, 0.0f);
        if(!sprite.GetFlipHorizontally())
            offset.x = -offset.x;

        EnemyType type = spawnedEnemyTypes[Random::GlobalInstance.NextInt(MAX_SPAWNED_ENEMY_TYPES)];
        Enemy* enemy = World::Instance().CreateEnemy(type, Position + offset);
        enemy->Velocity += sf::getNormalized(offset - Position) * SPAWN_ENEMY_FORCE;

        return currentEnemiesToSpawn == 0;
    }

    return false;
}

bool RogueKnight::TeleportAway([[maybe_unused]] float dt)
{
    auto rightWall = physics.RayCastTileMapHorizontal(player.Position, 1);
    auto leftWall = physics.RayCastTileMapHorizontal(player.Position, -1);

    if(rightWall.Distance > leftWall.Distance)
        Position.x = player.Position.x + TELEPORT_AWAY_DISTANCE;
    else
        Position.x = player.Position.x - TELEPORT_AWAY_DISTANCE;
    
    return true;
}

bool RogueKnight::TeleportAwayToCorner([[maybe_unused]] float dt)
{
    auto rightWall = physics.RayCastTileMapHorizontal(player.Position, 1);
    auto leftWall = physics.RayCastTileMapHorizontal(player.Position, -1);

    if(rightWall.Distance > leftWall.Distance)
    {
        float distance = rightWall.Distance + (player.Position.x - Position.x);
        Position.x = Position.x + distance - (SPRITE_WIDTH * SIZE_MULTIPLIER) - TELEPORT_INTO_CORNER_DISTANCE;
    }
    else
    {
        float distance = leftWall.Distance + (Position.x - player.Position.x);
        Position.x = Position.x - distance + (SPRITE_WIDTH * SIZE_MULTIPLIER) + TELEPORT_INTO_CORNER_DISTANCE;
    }

    return true;
}

bool RogueKnight::TeleportToFarthestCorner([[maybe_unused]] float dt)
{
    auto rightWall = physics.RayCastTileMapHorizontal(Position, 1);
    auto leftWall = physics.RayCastTileMapHorizontal(Position, -1);

    if(rightWall.Distance > leftWall.Distance)
        Position.x = Position.x + rightWall.Distance - (SPRITE_WIDTH * SIZE_MULTIPLIER) - TELEPORT_INTO_CORNER_DISTANCE;
    else
        Position.x = Position.x - leftWall.Distance + (SPRITE_WIDTH * SIZE_MULTIPLIER) + TELEPORT_INTO_CORNER_DISTANCE;

    return true;
}

bool RogueKnight::TeleportInFrontOfPlayerAim([[maybe_unused]] float dt)
{
    sf::Vector2f aimDir = player.GetDeagle().GetAimDirection();
    float dir = sf::sign(aimDir.x);
    auto wall = physics.RayCastTileMapHorizontal(player.Position, static_cast<int32_t>(dir));

    TeleportToPlayerIntoDirection(wall, dir, TELEPORT_INTO_PLAYER_DISTANCE);

    return true;
}

bool RogueKnight::TeleportBehindPlayerAim([[maybe_unused]] float dt)
{
    sf::Vector2f aimDir = player.GetDeagle().GetAimDirection();
    float dir = sf::sign(-aimDir.x);
    auto wall = physics.RayCastTileMapHorizontal(player.Position, static_cast<int32_t>(dir));

    TeleportToPlayerIntoDirection(wall, dir, TELEPORT_INTO_PLAYER_DISTANCE);

    return true;
}

bool RogueKnight::TeleportBehindPlayer([[maybe_unused]] float dt)
{
    float dir = sf::sign(player.Position.x - Position.x);
    auto wall = physics.RayCastTileMapHorizontal(player.Position, static_cast<int32_t>(dir));

    TeleportToPlayerIntoDirection(wall, dir, TELEPORT_INTO_PLAYER_DISTANCE);

    return true;
}

bool RogueKnight::TeleportAwayBehindPlayer([[maybe_unused]] float dt)
{
    float dir = sf::sign(player.Position.x - Position.x);
    auto wall = physics.RayCastTileMapHorizontal(player.Position, static_cast<int32_t>(dir));

    TeleportToPlayerIntoDirection(wall, dir, TELEPORT_AWAY_DISTANCE);

    return true;
}

void RogueKnight::TeleportToPlayerIntoDirection(const TileRayCollisionData& wall, float dir, float tpDistance)
{
    if(dir > 0.0f)
    {
        if(wall.Distance < tpDistance + (SPRITE_WIDTH * SIZE_MULTIPLIER))
            Position.x = player.Position.x + wall.Distance - (SPRITE_WIDTH * SIZE_MULTIPLIER);
        else
            Position.x = player.Position.x + tpDistance;
    }
    else
    {
        if(wall.Distance < tpDistance + (SPRITE_WIDTH * SIZE_MULTIPLIER))
            Position.x = player.Position.x - wall.Distance + (SPRITE_WIDTH * SIZE_MULTIPLIER);
        else
            Position.x = player.Position.x - tpDistance;
    }

    SetOrientationToPlayer(0.0f);
}

bool RogueKnight::ChangeColorToSecondPhase([[maybe_unused]] float dt)
{
    sprite.SetColor(sf::Color(255, 0, 0));
    return true;
}

bool RogueKnight::ChangeColorToThirdPhase([[maybe_unused]] float dt)
{
    sprite.SetColor(sf::Color(255, 0, 255));
    return true;
}


bool RogueKnight::ShootToPlayer(float dt)
{
    actionTimer += dt;

    if(actionTimer >= spawnBulletTime)
    {
        PlaySound(sounds[SoundIndex::ShootSound]);
        actionTimer = 0.0f;
        --currentBulletsToShoot;

        sf::Vector2f offset = sf::Vector2f(SPRITE_WIDTH * 0.4f, 0.0f);
        if(!sprite.GetFlipHorizontally())
            offset.x = -offset.x;
        sf::Vector2f pos = Position + offset;

        defaultBulletGenerator->GenerateBullet(pos, sf::getNormalized(player.Position - pos));

        return currentBulletsToShoot == 0;
    }

    return false;
}

bool RogueKnight::CreateClone([[maybe_unused]] float dt)
{
    clones.emplace_back(*this);
    
    return true;
}

//! ############ ACCIONES COMPUESTAS ############

bool RogueKnight::StandUp([[maybe_unused]] float dt)
{
    animations.ChangeAnimation(sprite, AnimationIndex::StandUpIndex);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::AddPlayerBulletCollisionMask);
    actions.EnqueueAction(Action::AddExplosionCollisionMask);

    return true;
}

bool RogueKnight::SitDown([[maybe_unused]] float dt)
{
    RemoveCollisionMask(CollisionType::PlayerBulletCollisionType);
    animations.ChangeAnimation(sprite, AnimationIndex::SitDownIndex);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    
    return true;
}

//! ############ FASE 1 ############

//Espadazo normal
bool RogueKnight::Phase1CloseRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Espadazo normal con delay y slash dash
bool RogueKnight::Phase1CloseRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Combo con primer y ultimo ataque con delay y slash dash
bool RogueKnight::Phase1CloseRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Combo cancelado en segundo espadazo y alejarse
bool RogueKnight::Phase1CloseRange4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunAway);

    return true;
}

//Correr hacia el jugador, espadazo y volver con dash
bool RogueKnight::Phase1MidRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashAway);

    return true;
}

//Correr hacia el jugador y triple combo con delay en el ultimo
bool RogueKnight::Phase1MidRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Dash hacia el jugador y triple combo con delay en el segundo y slash dash inverso 
bool RogueKnight::Phase1MidRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformReverseSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Igual que el anterior pero el slash dash inverso es en el tercero
bool RogueKnight::Phase1MidRange4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PerformReverseSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Dash hacia el jugador y de 1 a 4 espadazos sin delay
bool RogueKnight::Phase1LongRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    int32_t numTimes = Random::GlobalInstance.RangeInt(1, 4);
    for(int32_t i = 0;i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::PlaySlashAttackAnim);
        actions.EnqueueAction(Action::PerformSlashDash);
        actions.EnqueueAction(Action::FirstSlash);
    }
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Spawnear enemigos
bool RogueKnight::Phase1LongRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
    actions.EnqueueAction(Action::SpawnEnemies);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Disparar al jugador
bool RogueKnight::Phase1LongRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//! ############ FASE 2 ############

//Correr, Primer espadazo a invisibilidad, teletransporte detras y continuacion del combo
bool RogueKnight::Phase2CloseRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Invisible, Teletransporte aleatorio, Primer espadazo y segundo espadazo, Teletransporte lejos y animacion idle
bool RogueKnight::Phase2CloseRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    else
        actions.EnqueueAction(Action::TeleportInFrontOfPlayerAim);

    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::TeleportAway);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::FastReapear);

    return true;
}

//Alejarse con la magia
bool RogueKnight::Phase2CloseRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::Dissapear);
    actions.EnqueueAction(Action::TeleportAwayToCorner);
    actions.EnqueueAction(Action::WaitSlow);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::Reapear);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//3 espadazos a veces con delay hacia insivilidad
bool RogueKnight::Phase2CloseRange4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
    }
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
    }
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
    }
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastDissapear);

    return true;
}

//Dash a invisibilidad, teletransporte, combo de 3 a invisibilidad
bool RogueKnight::Phase2MidRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    actions.EnqueueAction(Action::FastDissapear);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    else
        actions.EnqueueAction(Action::TeleportInFrontOfPlayerAim);

    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);

    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::Dissapear);

    return true;
}

//Alejarse, invisible a teletransporte detras y espadazo a invisible y teletrasnporte lejos
bool RogueKnight::Phase2MidRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunAway);
    actions.EnqueueAction(Action::FastDissapear);

    actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::TeleportAway);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::FastReapear);

    return true;
}

//Tres espadazos a veces con slash dash a invisible
bool RogueKnight::Phase2MidRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);

    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
        actions.EnqueueAction(Action::PerformSlashDash);
    }
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastDissapear);

    return true;
}

//Dashear al jugador y Dobles dobles espadazos 
bool RogueKnight::Phase2MidRange4([[maybe_unused]] float dt)
{
    for(int i = 0;i < 2; ++i)
    {
        actions.EnqueueAction(Action::PlayDash1Anim);
        actions.EnqueueAction(Action::DashToPlayer);
        actions.EnqueueAction(Action::PlaySlashComboAttackAnim);

        actions.EnqueueAction(Action::PerformSlashDash);
        actions.EnqueueAction(Action::FirstSlash);

        if(Random::GlobalInstance.RangeInt(0, 1) == 1)
            actions.EnqueueAction(Action::PerformSlashDash);
        else
            actions.EnqueueAction(Action::PerformReverseSlashDash);
        actions.EnqueueAction(Action::SecondSlash);
    }
    actions.EnqueueAction(Action::PlayIdleAnim);

    return true;
}

//Spawnear 2 veces enemigos y esperar a que el jugador los mate
bool RogueKnight::Phase2LongRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
    actions.EnqueueAction(Action::SpawnEnemies);
    actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
    actions.EnqueueAction(Action::SpawnEnemies);
    actions.EnqueueAction(Action::Dissapear);
    actions.EnqueueAction(Action::WaitEnemiesDeath);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::Reapear);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Magia a invisibilidad, teletransporte detras o delante, slash y volver lejos
bool RogueKnight::Phase2LongRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::Dissapear);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    else
        actions.EnqueueAction(Action::TeleportInFrontOfPlayerAim);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::TeleportAway);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//Disparar al jugador, teletransporte a la esquina contraria y dispara al jugador, hacia invisibilidad
bool RogueKnight::Phase2LongRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);
    actions.EnqueueAction(Action::FastDissapear);
    actions.EnqueueAction(Action::TeleportToFarthestCorner);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);
    actions.EnqueueAction(Action::FastDissapear);

    return true;
}

//Volver de invisibilidad, 2 espadazos, y dashear a invisibilidad
bool RogueKnight::Phase2Invisible1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportInFrontOfPlayerAim);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);

    //A veces reverse
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformReverseSlashDash);
    else 
        actions.EnqueueAction(Action::PerformSlashDash);

    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashAway);
    actions.EnqueueAction(Action::FastDissapear);

    return true;
}

//Teletransporte detras, visibilidad, invisibilidad y aparecer en una esquina
bool RogueKnight::Phase2Invisible2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayToCorner);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::FastReapear);

    return true;
}

//Teletransporte a esquina y spawnear enemigo
bool RogueKnight::Phase2Invisible3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportAwayToCorner);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::FastReapear);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
    actions.EnqueueAction(Action::SpawnEnemies);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

//! FASE 3

//Invis, teletransporte detras con clon y vuelta
bool RogueKnight::Phase3CloseRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    
    return true;
}

bool RogueKnight::Phase3CloseRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAway);
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::InstantReapear);

    return true;
}

//Multiples teleports delante y detras aleatorios hasta uno que de golpe y luego clon que corre lejos detras
bool RogueKnight::Phase3CloseRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    int32_t numTimes = Random::GlobalInstance.RangeInt(1, 3);
    for(int i = 0; i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::TeleportBehindPlayer);
        actions.EnqueueAction(Action::InstantReapear);
        actions.EnqueueAction(Action::InstantDissapear);
    }
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::CreateClone);

    return true;
}

bool RogueKnight::Phase3CloseRange4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
    }
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::TeleportBehindPlayerAim);
        actions.EnqueueAction(Action::CreateClone);
        actions.EnqueueAction(Action::TeleportBehindPlayer);
    }
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::SecondSlash);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformReverseSlashDash);
    else
        actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::CreateClone);

    return true;
}

bool RogueKnight::Phase3MidRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash1Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::WaitFast);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::WaitFast);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);

    return true;
}

bool RogueKnight::Phase3MidRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);

    int32_t numTimes = Random::GlobalInstance.RangeInt(1, 3);
    for(int32_t i = 0;i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::TeleportBehindPlayer);
        actions.EnqueueAction(Action::InstantReapear);
        actions.EnqueueAction(Action::InstantDissapear);
        
        if(Random::GlobalInstance.RangeInt(1, 3) == 1)
            actions.EnqueueAction(Action::CreateClone);
    }
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformSlashDash);
    else
        actions.EnqueueAction(Action::PerformReverseSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayIdleAnim);

    return true;
}

bool RogueKnight::Phase3MidRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);

    int32_t numTimes = Random::GlobalInstance.RangeInt(1, 3);
    for(int32_t i = 0;i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::TeleportBehindPlayer);
        actions.EnqueueAction(Action::InstantReapear);
        actions.EnqueueAction(Action::InstantDissapear);
        
        if(Random::GlobalInstance.RangeInt(1, 3) == 1)
            actions.EnqueueAction(Action::CreateClone);
    }
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::SecondSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::WaitSlash);
    actions.EnqueueAction(Action::ResumeAnimation);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        actions.EnqueueAction(Action::PerformSlashDash);
    else
        actions.EnqueueAction(Action::PerformReverseSlashDash);
    actions.EnqueueAction(Action::ThirdSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayIdleAnim);

    return true;
}

bool RogueKnight::Phase3MidRange4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayDash2Anim);
    actions.EnqueueAction(Action::DashToPlayer);
    int32_t numTimes = Random::GlobalInstance.RangeInt(2, 4);
    for(int i = 0;i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
        actions.EnqueueAction(Action::PerformSlashDash);
        actions.EnqueueAction(Action::CreateClone);
        actions.EnqueueAction(Action::FirstSlash);
        actions.EnqueueAction(Action::PerformSlashDash);
        actions.EnqueueAction(Action::CreateClone);
        actions.EnqueueAction(Action::SecondSlash);
        actions.EnqueueAction(Action::PerformSlashDash);
        actions.EnqueueAction(Action::ThirdSlash);
        actions.EnqueueAction(Action::CreateClone);
    }
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::WaitFast);
        actions.EnqueueAction(Action::TeleportBehindPlayer);
        actions.EnqueueAction(Action::PlaySlashAttackAnim);
    }
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::WaitFast);
        actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
        actions.EnqueueAction(Action::PlayRunAnim);
        actions.EnqueueAction(Action::CreateClone);
    }
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::WaitFast);
        actions.EnqueueAction(Action::TeleportInFrontOfPlayerAim);
        actions.EnqueueAction(Action::PlayMagicAttackAnim);
        actions.EnqueueAction(Action::CreateClone);
    }

    return true;
}

bool RogueKnight::Phase3LongRange1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    int32_t numTimes = Random::GlobalInstance.RangeInt(1, 3);
    for(int32_t i = 0;i < numTimes; ++i)
    {   
        actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
        actions.EnqueueAction(Action::SpawnEnemies);
        actions.EnqueueAction(Action::InstantDissapear);
        actions.EnqueueAction(Action::CreateClone);
        actions.EnqueueAction(Action::TeleportToFarthestCorner);
        actions.EnqueueAction(Action::InstantReapear);
    }
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::WaitEnemiesDeath);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::WaitAnimationEnd);

    return true;
}

bool RogueKnight::Phase3LongRange2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::CreateClone);

    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::SetSpawnEnemiesVariables);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::SpawnEnemies);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayIdleAnim);

    return true;
}

bool RogueKnight::Phase3LongRange3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayToCorner);
    actions.EnqueueAction(Action::SetOrientationToPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);

    int32_t numTimes = Random::GlobalInstance.RangeInt(0, 3);
    for(int i = 0;i < numTimes; ++i)
    {
        actions.EnqueueAction(Action::SetOrientationToPlayer);
        actions.EnqueueAction(Action::InstantReapear);
        actions.EnqueueAction(Action::SetShootToPlayerVariables);
        actions.EnqueueAction(Action::ShootToPlayer);
        actions.EnqueueAction(Action::InstantDissapear);
        actions.EnqueueAction(Action::TeleportToFarthestCorner);
    }

    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::InstantDissapear);

    return true;
}

//Acaba en invisible
bool RogueKnight::Phase3Invisible1([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::RunToPlayer);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::PauseAnimation);
        actions.EnqueueAction(Action::WaitSlash);
        actions.EnqueueAction(Action::ResumeAnimation);
    }
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayDash2Anim);
    actions.EnqueueAction(Action::CreateClone);

    return true;
}

//Acaba en visible
bool RogueKnight::Phase3Invisible2([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportAwayToCorner);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportToFarthestCorner);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::PlayMagicAttackAnim);
    actions.EnqueueAction(Action::PauseAtFrame7);
    actions.EnqueueAction(Action::SetShootToPlayerVariables);
    actions.EnqueueAction(Action::ShootToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::PlayIdleAnim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);

    return true;
}

//Acabe en visible (fumada total de copias)
bool RogueKnight::Phase3Invisible3([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportBehindPlayerAim);
    actions.EnqueueAction(Action::PlaySlashComboAttackAnim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayDash2Anim);
    actions.EnqueueAction(Action::CreateClone);
    
    int numTimes = Random::GlobalInstance.RangeInt(1, 4);
    for(int32_t i = 0;i < numTimes; ++i)
    {
        if(Random::GlobalInstance.RangeInt(0, 1) == 1)
        {
            actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
            if(Random::GlobalInstance.RangeInt(0, 1) == 1)
                actions.EnqueueAction(Action::PlayDash2Anim);
            else
                actions.EnqueueAction(Action::PlayRunAnim);
            actions.EnqueueAction(Action::CreateClone);
        }
        else
        {
            actions.EnqueueAction(Action::TeleportBehindPlayer);
            actions.EnqueueAction(Action::PlaySlashAttackAnim);
            actions.EnqueueAction(Action::CreateClone);
        }

        if(Random::GlobalInstance.RangeInt(0, 1) == 1)
            actions.EnqueueAction(Action::WaitFast);
    }
    
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::WaitAnimationEnd);
    actions.EnqueueAction(Action::PlayIdleAnim);

    return true;
}

bool RogueKnight::Phase3Invisible4([[maybe_unused]] float dt)
{
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::PlayDash2Anim);
    actions.EnqueueAction(Action::CreateClone);
    actions.EnqueueAction(Action::TeleportAwayBehindPlayer);
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::DashToPlayer);
    actions.EnqueueAction(Action::InstantDissapear);
    actions.EnqueueAction(Action::TeleportBehindPlayer);
    actions.EnqueueAction(Action::PlaySlashAttackAnim);
    actions.EnqueueAction(Action::PauseAnimation);
    if(Random::GlobalInstance.RangeInt(0, 1) == 1)
    {
        actions.EnqueueAction(Action::CreateClone);
        actions.EnqueueAction(Action::TeleportBehindPlayer);
        actions.EnqueueAction(Action::WaitFast);
    }
    actions.EnqueueAction(Action::InstantReapear);
    actions.EnqueueAction(Action::ResumeAnimation);
    actions.EnqueueAction(Action::PerformSlashDash);
    actions.EnqueueAction(Action::FirstSlash);
    actions.EnqueueAction(Action::PlayRunAnim);
    actions.EnqueueAction(Action::RunAway);

    return true;
}


#include "../../Physics/Circle.h"
void RogueKnight::Draw(float tickPercent)
{
    /*
    sf::Vector2f pos = GetInterpolatedPosition(tickPercent);
    auto layer = physics.GetCollisionLayer();
    sf::Vector2f center = sf::Vector2f( layer->GetCurrentWidth() * 64.0f * 0.5f, layer->GetCurrentHeight() * 64.0f * 0.5f);
    //sf::Vector2f center = player.GetInterpolatedPosition(tickPercent);
    float mirrorX = center.x - pos.x;
    sf::Vector2f mirror = sf::Vector2f(center.x + mirrorX, pos.y);

    E2D::Renderer::Instance().Draw(pos, sprite);
    bool hor = sprite.GetFlipHorizontally();
    sprite.SetFlipHorizontally(!hor);
    //E2D::Renderer::Instance().Draw(mirror, sprite);
    sprite.SetFlipHorizontally(hor);
    */

   E2D::Renderer::Instance().Draw(GetInterpolatedPosition(tickPercent), sprite);
   DrawClones(tickPercent);

    //! BORRAR
    /*
    Circle* c = static_cast<Circle*>(hurtZone.GetCollider());
    
    sf::CircleShape circle (c->Radius);
    circle.setPosition(hurtZone.Position);
    circle.setOrigin(c->Radius, c->Radius);
    if(hurtZone.IsActive)
        circle.setFillColor(sf::Color::Red);
    //E2D::Renderer::Instance().activeRenderTexture->draw(circle);
    circle.setRadius(5.0f);
    circle.setPosition(pos);
    circle.setOrigin(5.0f, 5.0f);
    */
    //E2D::Renderer::Instance().activeRenderTexture->draw(circle);
}

RogueKnight::~RogueKnight()
{
    physics.RemoveStaticEntity(&hurtZone);
    delete defaultBulletGenerator;
}

//CLONES
RogueKnightFalseClone::RogueKnightFalseClone(RogueKnight& rogueKnight)
{
    lastPosition = position = rogueKnight.Position;

    animation = rogueKnight.animations.CurrentAnimation();
    sf::IntRect spriteRect = animation.GetFrame(0);
    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("RogueKnight"));
    sprite.setOrigin(spriteRect.width * 0.5f, spriteRect.height * 0.5f);
    sprite.setScale(RogueKnight::SIZE_MULTIPLIER, RogueKnight::SIZE_MULTIPLIER);
    sf::Color color = rogueKnight.sprite.GetColor();
    color.r = static_cast<uint8_t>(static_cast<float>(color.r) * COLOR_MULTIPLIER);
    color.g = static_cast<uint8_t>(static_cast<float>(color.g) * COLOR_MULTIPLIER);
    color.b = static_cast<uint8_t>(static_cast<float>(color.b) * COLOR_MULTIPLIER);
    sprite.SetColor(color);
    sprite.SetAlpha(0);
    animation.Play(sprite);

    if(rogueKnight.player.Position.x < position.x) //Mira hacia la izquierda
    {
        sprite.SetFlipHorizontally(true);
        direction = -1.0f;
    }
    else
    {
        direction =  1.0f;
    }

    switch(rogueKnight.animations.GetCurrentAnimationIndex())
    {
        case RogueKnight::RunIndex:
            speed = rogueKnight.runSpeed;
            UpdateFunc = &RogueKnightFalseClone::UpdateRunningDashing;
            break;
        case RogueKnight::Dash1Index:
        case RogueKnight::Dash2Index:
            speed = rogueKnight.dashSpeed;
            UpdateFunc = &RogueKnightFalseClone::UpdateRunningDashing;
            break;
        case RogueKnight::SlashAttackIndex:
        case RogueKnight::SlashComboAttackIndex:
            UpdateFunc = &RogueKnightFalseClone::UpdateSlashAttack;
            animation.SetLoop(false);
            break;
        case RogueKnight::MagicAttackIndex:
            UpdateFunc = &RogueKnightFalseClone::UpdateDefault;
            animation.SetLoop(false);
        default:
            UpdateFunc = &RogueKnightFalseClone::UpdateDefault;
            break;
    }
}

void RogueKnightFalseClone::UpdatePhysics(float dt)
{
    lastPosition = position;
    position.x += veloctiy * dt;

    veloctiy *= std::pow(RogueKnight::BASE_FRICTION, dt);
}

void RogueKnightFalseClone::UpdateSlashAttack(float dt)
{
    if(sprite.IsFadedIn())
    {
        if(timer == 0.0f) //Hacer el slash dash solo la primera vez
        {
            veloctiy += direction * RogueKnight::SLASH_BASE_FORCE;
        }

        animation.Update(sprite, dt);
        timer += dt;

        if(timer >= RogueKnightFalseClone::MAX_CLONE_DURATION)
            UpdateFunc = &RogueKnightFalseClone::UpdateFadeOut;

        UpdatePhysics(dt);
    }
    else
        sprite.FadeIn(RogueKnight::INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
}

void RogueKnightFalseClone::UpdateRunningDashing(float dt)
{
    animation.Update(sprite, dt);
    if(sprite.IsFadedIn())
    {
        veloctiy += direction * speed * dt;
        timer += dt;

        UpdatePhysics(dt);

        if(timer >= RogueKnightFalseClone::MAX_CLONE_DURATION)
            UpdateFunc = &RogueKnightFalseClone::UpdateFadeOut;
    }
    else
        sprite.FadeIn(RogueKnight::INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
}

void RogueKnightFalseClone::UpdateDefault(float dt)
{
    animation.Update(sprite, dt);
    if(sprite.IsFadedIn())
    {
        timer += dt;

        if(timer >= MAX_CLONE_DURATION)
            UpdateFunc = &RogueKnightFalseClone::UpdateFadeOut;
    }
    else
        sprite.FadeIn(RogueKnight::INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
}

void RogueKnightFalseClone::UpdateFadeOut(float dt)
{
    IsDead = sprite.FadeOut(RogueKnight::INSTANT_DISSAPEAR_REAPEAR_DURATION, dt);
}

void RogueKnightFalseClone::Update(float dt)
{
    (this->*UpdateFunc)(dt);
}

void RogueKnightFalseClone::Draw(float tickPercent)
{
    sf::Vector2f pos = lastPosition * (1.0f - tickPercent) + position * tickPercent;

    E2D::Renderer::Instance().Draw(pos, sprite);
}
