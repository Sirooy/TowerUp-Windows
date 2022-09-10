#pragma once

#include "Boss.h"
#include "../../TransitionStateMachine.h"
#include "../../ActionQueue.h"
#include "../../AnimationPack.h"
#include "../HurtZone.h"

struct TileRayCollisionData;
class RogueKnightFalseClone;

class RogueKnight : public Boss
{
public:
    static constexpr float SPRITE_WIDTH = 26.0f;
    static constexpr float SPRITE_HEIGHT = 60.0f;
    static constexpr float SIZE_MULTIPLIER = 2.0f;
    static constexpr float BASE_FRICTION = 0.01f;

    //!DURACION BASE DE LAS ANIMACIONES
    static constexpr float IDLE_ANIMATION_DURATION = 0.6f;
    static constexpr float RUN_ANIMATION_DURATION = 0.5f;
    static constexpr float JUMP_ANIMATION_DURATION = 1.0f;
    static constexpr float MAGIC_ATTACK_ANIMATION_DURATION = 1.0f;
    static constexpr float SLASH_ATTACK_ANIMATION_DURATION = 0.5f;
    static constexpr float DASH_ANIMATION_DURATION = 0.6f;
    static constexpr float SIT_DOWN_ANIMATION_DURATION = 2.0f;
    static constexpr float STAND_UP_ANIMATION_DURATION = 2.0f;
    static constexpr float FLY_ANIMATION_DURATION = 0.6f;

    static constexpr float BASE_HEALTH = 3000.0f;
    static constexpr float SECOND_PHASE_MIN_HEALTH = BASE_HEALTH * 0.7f;
    static constexpr float THIRD_PHASE_MIN_HEALTH = BASE_HEALTH * 0.3f;
    static constexpr float CLOSE_RANGE_MAX_ATTACK_SQUARED_DISTANCE = 250.0f * 250.0f;
    static constexpr float MID_RANGE_MAX_ATTACK_SQUARED_DISTANCE = 500.0f * 500.0f;
    static constexpr float HURTZONE_DISTANCE = SPRITE_WIDTH * SIZE_MULTIPLIER;
    static constexpr float DISSAPEAR_REAPEAR_DURATION = 1.5f;
    static constexpr float FAST_DISSAPEAR_REAPEAR_DURATION = 0.5f;
    static constexpr float INSTANT_DISSAPEAR_REAPEAR_DURATION = 0.25f; 
    static constexpr float WAIT_DURATION = 2.0f;
    static constexpr float RUN_TO_PLAYER_MIN_DISTANCE_SQUARED = 160.0f * 160.0f;
    static constexpr float RUN_AWAY_MAX_DISTANCE_SQUARED = 500.0f * 500.0f;
    static constexpr float DASH_TO_PLAYER_MIN_DISTANCE_SQUARED = 100.0f * 100.0f;
    static constexpr float DASH_AWAY_MAX_DISTANCE_SQUARED = 600.0f * 600.0f;
    static constexpr float MAX_RUN_DURUATION = 2.0f;
    static constexpr float MAX_DASH_DURATION = 1.0f;
    static constexpr float SLASH_BASE_FORCE = 750.0f;
    static constexpr float SPAWN_ENEMY_FORCE = 500.0f;

    static constexpr int32_t FIRST_SLASH_HITBOX_FIRST_FRAME = 3;
    static constexpr int32_t FIRST_SLASH_HITBOX_LAST_FRAME = 5;
    static constexpr int32_t FIRST_SLASH_LAST_FRAME = 6;

    static constexpr int32_t SECOND_SLASH_HITBOX_FIRST_FRAME = 7;
    static constexpr int32_t SECOND_SLASH_HITBOX_LAST_FRAME = 9;
    static constexpr int32_t SECOND_SLASH_LAST_FRAME = 11;

    static constexpr int32_t THIRD_SLASH_HITBOX_FIRST_FRAME = 12;
    static constexpr int32_t THIRD_SLASH_HITBOX_LAST_FRAME = 14;

    static constexpr float TELEPORT_AWAY_DISTANCE = 500.0f;
    static constexpr float TELEPORT_INTO_PLAYER_DISTANCE = 150.0f;
    static constexpr float TELEPORT_INTO_CORNER_DISTANCE = 150.0f;

    static constexpr int32_t MAX_SPAWNED_ENEMY_TYPES = 2; 

    RogueKnight(sf::Vector2f position);

    void Update(float dt) override;
    void Draw(float tickPercent) override;
    void PreparingAction(float dt);
    void PerformingAction(float dt);
    void Dying(float dt);

    void OnEnterPreparingAction();
    void OnEnterSecondPhase();
    void OnEnterThirdPhase();
    void OnEnterDying();

    bool ToPreparingAction();
    bool ToPerformingAction();
    bool ToSecondPhase();
    bool ToThirdPhase();
    bool ToDying();

    ~RogueKnight() override;

    //!Acciones base
    template<int32_t T>
    bool PlayAnimation([[maybe_unused]] float dt)
    {
        animations.ChangeAnimation(sprite, T);
        return true;
    }
    
    template<uint32_t T>
    bool AddCollisionMaskAction([[maybe_unused]] float dt)
    {
        this->AddCollisionMask(T);

        return true;
    }

    template<uint32_t T>
    bool RemoveCollisionMaskAction([[maybe_unused]] float dt)
    {
        this->RemoveCollisionMask(T);

        return true;
    }

    template<int32_t T>
    bool PauseAtFrame([[maybe_unused]] float dt)
    {
        bool result = animations.CurrentAnimation().GetCurrentFrameIndex() == T;

        if(result)
            animations.CurrentAnimation().Pause();     
        
        return result;
    }

    bool PauseAnimation(float dt);
    bool ResumeAnimation(float dt);
    bool WaitAnimationEnd(float dt);
    bool SetOrientationToPlayer(float dt);
    bool ResetActionTimer(float dt);
    bool SetSpawnEnemiesVariables(float dt);
    bool SetShootToPlayerVariables(float dt);

    bool Dissapear(float dt);
    bool Reapear(float dt);
    bool FastDissapear(float dt);
    bool FastReapear(float dt);
    bool InstantDissapear(float dt);
    bool InstantReapear(float dt);

    bool WaitSlash(float dt);
    bool WaitSlow(float dt);
    bool WaitFast(float dt);
    bool WaitEnemiesDeath(float dt);
    bool EnableHurtZone(float dt);
    bool DisableHurtZone(float dt);

    bool RunAway(float dt);
    bool RunToPlayer(float dt);

    bool DashAway(float dt);
    bool DashToPlayer(float dt);

    bool FirstSlash(float dt);
    bool SecondSlash(float dt);
    bool ThirdSlash(float dt);
    bool PerformSlashDash(float dt);
    bool PerformReverseSlashDash(float dt);

    bool SpawnEnemies(float dt);

    bool TeleportAway(float dt);
    bool TeleportAwayToCorner(float dt);
    bool TeleportToFarthestCorner(float dt);
    bool TeleportInFrontOfPlayerAim(float dt);
    bool TeleportBehindPlayerAim(float dt);
    bool TeleportBehindPlayer(float dt);
    bool TeleportAwayBehindPlayer(float dt);

    bool ChangeColorToSecondPhase(float dt);
    bool ChangeColorToThirdPhase(float dt);

    bool ShootToPlayer(float dt);
    bool CreateClone(float dt);

    //!Acciones compuestas
    bool StandUp(float dt);
    bool SitDown(float dt);

    //! FASE 1
    bool Phase1CloseRange1(float dt);
    bool Phase1CloseRange2(float dt);
    bool Phase1CloseRange3(float dt);
    bool Phase1CloseRange4(float dt);
    bool Phase1MidRange1(float dt);
    bool Phase1MidRange2(float dt);
    bool Phase1MidRange3(float dt);
    bool Phase1MidRange4(float dt);
    bool Phase1LongRange1(float dt);
    bool Phase1LongRange2(float dt);
    bool Phase1LongRange3(float dt);

    //! FASE 2
    bool Phase2CloseRange1(float dt);
    bool Phase2CloseRange2(float dt);
    bool Phase2CloseRange3(float dt);
    bool Phase2CloseRange4(float dt);
    bool Phase2MidRange1(float dt);
    bool Phase2MidRange2(float dt);
    bool Phase2MidRange3(float dt);
    bool Phase2MidRange4(float dt);
    bool Phase2LongRange1(float dt);
    bool Phase2LongRange2(float dt);
    bool Phase2LongRange3(float dt);
    bool Phase2Invisible1(float dt);
    bool Phase2Invisible2(float dt);
    bool Phase2Invisible3(float dt);

    //! FASE 3
    bool Phase3CloseRange1(float dt);
    bool Phase3CloseRange2(float dt);
    bool Phase3CloseRange3(float dt);
    bool Phase3CloseRange4(float dt);
    bool Phase3MidRange1(float dt);
    bool Phase3MidRange2(float dt);
    bool Phase3MidRange3(float dt);
    bool Phase3MidRange4(float dt);
    bool Phase3LongRange1(float dt);
    bool Phase3LongRange2(float dt);
    bool Phase3LongRange3(float dt);
    bool Phase3Invisible1(float dt);
    bool Phase3Invisible2(float dt);
    bool Phase3Invisible3(float dt);
    bool Phase3Invisible4(float dt);
    
protected:
    friend class RogueKnightFalseClone;

    enum class State
    {
        StartingBattle,
        PreparingAction,
        PerformingAction,
        EnteringSecondPhase,
        EnteringThirdPhase,
        Dying
    };

    enum class Action
    {
        //Acciones Base
        PlayIdleAnim,
        PlayRunAnim,
        PlayJumpAnim,
        PlayMagicAttackAnim,
        PlaySlashAttackAnim,
        PlaySlashComboAttackAnim,
        PlayDash1Anim,
        PlayDash2Anim,
        PlaySitDownAnim,   
        PlayStandUpAnim,
        PlayFlyAnim,
        PauseAtFrame7, //Para el ataque magico
        PauseAnimation,
        ResumeAnimation,
        WaitAnimationEnd,

        AddPlayerBulletCollisionMask,
        AddExplosionCollisionMask,
        RemovePlayerBulletCollisionMask,
        RemoveExplosionCollisionMask,
        SetOrientationToPlayer,
        ResetActionTimer,
        SetSpawnEnemiesVariables,
        SetShootToPlayerVariables,

        Dissapear,
        Reapear,
        FastDissapear,
        FastReapear,
        InstantDissapear,
        InstantReapear,

        WaitSlash,
        WaitSlow,
        WaitFast,
        WaitEnemiesDeath,

        EnableHurtZone,
        DisableHurtZone,

        RunAway,
        RunToPlayer,
        DashAway,
        DashToPlayer,

        FirstSlash,
        SecondSlash,
        ThirdSlash,
        PerformSlashDash,
        PerformReverseSlashDash,

        SpawnEnemies,
        TeleportInFrontOfPlayerAim,
        TeleportBehindPlayerAim,
        TeleportBehindPlayer,
        TeleportAway,
        TeleportAwayToCorner,
        TeleportToFarthestCorner,
        TeleportAwayBehindPlayer,

        ChangeColorToSecondPhase,
        ChangeColorToThirdPhase,

        ShootToPlayer,
        CreateClone,

        //Acciones Compuestas
        StandUp,
        SitDown,

        //! FASE 1
        Phase1CloseRange1,
        Phase1CloseRange2,
        Phase1CloseRange3,
        Phase1CloseRange4,
        Phase1MidRange1,
        Phase1MidRange2,
        Phase1MidRange3,
        Phase1MidRange4,
        Phase1LongRange1,
        Phase1LongRange2,
        Phase1LongRange3,

        //! FASE 2
        Phase2CloseRange1,
        Phase2CloseRange2,
        Phase2CloseRange3,
        Phase2CloseRange4,
        Phase2MidRange1,
        Phase2MidRange2,
        Phase2MidRange3,
        Phase2MidRange4,
        Phase2LongRange1,
        Phase2LongRange2,
        Phase2LongRange3,
        Phase2Invisible1,
        Phase2Invisible2,
        Phase2Invisible3,

        //! FASE 3
        Phase3CloseRange1,
        Phase3CloseRange2,
        Phase3CloseRange3,
        Phase3CloseRange4,
        Phase3MidRange1,
        Phase3MidRange2,
        Phase3MidRange3,
        Phase3MidRange4,
        Phase3LongRange1,
        Phase3LongRange2,
        Phase3LongRange3,
        Phase3Invisible1,
        Phase3Invisible2,
        Phase3Invisible3,
        Phase3Invisible4
    };

    enum AnimationIndex
    {
        IdleIndex = 0,
        RunIndex,
        JumpIndex,
        MagicAttackIndex,
        SlashAttackIndex,
        SlashComboAttackIndex,
        Dash1Index,
        Dash2Index,
        SitDownIndex,
        StandUpIndex,
        FlyIndex
    };

    enum ActionIndex
    {
        CloseRangeActionIndex = 0,
        MidRangeActionIndex = 1,
        LongRangeActionIndex = 2,
        InvisibilityActionIndex = 3
    };

    enum SoundIndex
    {
        ShootSound = 0,
        BladeSound,
        DieSound,
        SummonSound
    };

    TransitionStateMachine<RogueKnight::State, RogueKnight> stateMachine {};
    ActionQueue<RogueKnight::Action, RogueKnight> actions {};
    AnimationPack<11> animations {};
    BulletGenerator* defaultBulletGenerator {};
    std::vector<RogueKnightFalseClone> clones;
    HurtZone hurtZone;
    E2D::Sound sounds[4];

    float runSpeed { 1400.0f };
    float dashSpeed { 2400.0f };
    float prepareActionTimer { 0.0f };
    float actionTimer { 0.0f };
    bool isInvisible { false };

    int32_t currentEnemiesToSpawn { 0 };
    int32_t minEnemiesToSpawn { 1 };
    int32_t maxEnemiesToSpawn { 2 };
    
    int32_t currentBulletsToShoot { 0 };

    int32_t currentActionsToPerform { 0 };
    int32_t minActionsPerWait { 1 };
    int32_t maxActionsPerWait { 2 };
    int32_t currentPhase { 0 };

    //Tiempos de espera modificables
    float slashDelayWaitTime { 0.6f };
    float slowWaitTime { 2.5f };
    float fastWaitTime { 1.0f };
    float spawnEnemyTime { 1.0f };
    float spawnBulletTime { 0.5f }; 

    EnemyType spawnedEnemyTypes[MAX_SPAWNED_ENEMY_TYPES] = { EnemyType::BlueFlyingEye, EnemyType::BlueBatEye };

    std::vector<Action> phaseActions[3][4] { };

    void CreateSounds();
    void CreateAnimations();
    void CreateStateMachine();
    void CreateActionStateMachine();
    void AddTransitionsToDying();
    void CreateActionVectors();
    void SetHurtZonePosition();
    void SetSlashDirection(float xForce, float yForce);
    void TeleportToPlayerIntoDirection(const TileRayCollisionData& wall, float dir, float tpDistance);
    void CreateDefaultBulletGenerator();
    void EnqueuePhaseAction(const std::vector<Action>& vec);
    void UpdateClones(float dt);
    void DrawClones(float tickPercent);
    void RemoveDeadClones();
};

class RogueKnightFalseClone
{
public:
    static constexpr float MAX_CLONE_DURATION = 2.0f;
    static constexpr float COLOR_MULTIPLIER = 0.75f;

    RogueKnightFalseClone(RogueKnight& rogueKnight);

    void Update(float dt);
    void Draw(float tickPercent);

    void (RogueKnightFalseClone::*UpdateFunc) (float);
    bool IsDead { false };
protected:
    E2D::Sprite sprite {};
    Animation animation;
    sf::Vector2f position {};
    sf::Vector2f lastPosition {};
    float veloctiy {}; //Solo se mueve en el eje x
    float speed { 0.0f };
    float direction { 0.0f };
    float timer { 0.0f };

    void UpdatePhysics(float dt);
    void UpdateSlashAttack(float dt);
    void UpdateRunningDashing(float dt);
    void UpdateDefault(float dt);
    void UpdateFadeOut(float dt);
};