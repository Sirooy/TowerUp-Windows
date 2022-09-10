#pragma once

#include "../TransitionStateMachine.h"
#include "InteractiveElement.h"
#include "DynamicEntity.h"
#include "../Engine2D/Sprite.h"
#include "../Engine2D/Sound.h"
#include "../AnimationPack.h"
#include "DesertEagle.h"
#include "SFML/Graphics.hpp"
#include <functional>
//#include "Engine2D/LightSystem.h"

namespace E2D
{

class Light;

}

class BaseItem;

class Player : public DynamicEntity
{
public:
    static constexpr float WIDTH = 48.0f;
    static constexpr float HEIGHT = 56.0f;

    enum class State
    {
        Idle,
        Running,
        Jumping,
        Dashing,
        Falling,
        Dead
    };

    Player();

    void OnCollision(CollidableEntity* other, const CollisionData& data) override;
    void OnCollision(Enemy* enemy, const CollisionData& data) override;
    void OnCollision(Bullet* bullet, const CollisionData& data) override;
    void OnCollision(HurtZone* bullet, const CollisionData& data) override;
    void OnVerticalTileCollision(const TileCollisionData& data) override;
    void OnHorizontalTileCollision(const TileCollisionData& data) override;
    void HandleEvent(const sf::Event& ev);
    void UpdateLightPosition(float tickPercent);
    void SetLight(E2D::Light* light);
    void Update(float dt) override;
    void Draw(float tickPercent) override;

    //StateMachine Methods
    void Idle(float dt);
    void Running(float dt);
    void Jumping(float dt);
    void Dashing(float dt);
    void Falling(float dt);
    void Dead(float dt);
    void HandleHorizontalMovement(float dt);
    void HandleOrientation();
    void OnEnterIdle();
    void OnEnterRunning();
    void OnEnterJump();
    void OnEnterDash();
    void OnEnterFalling();
    void OnEnterDead();
    void OnExitDash();
    bool ToIdle();         
    bool RunningToIdle();  
    bool ToRunning();      
    bool IdleToRunning();  
    bool DashingToRunning();
    bool ToJumping();      
    bool ToDashing();      
    bool RunningToFalling(); 
    bool JumpingToFalling(); 
    bool DashingToFalling(); 
    bool ToDead();

    inline int32_t GetMaxHealth() const { return maxHealth; }
    inline int32_t GetCurrentHealth() const { return currentHealth; }
    inline int32_t GetMoney() const { return money; }
    inline DesertEagle& GetDeagle() { return deagle; }
    inline const DesertEagle& GetDeagle() const { return deagle; }
    inline float GetDashCooldown() const { return dashCooldown; }
    inline float GetMoveSpeed() const { return moveSpeed; }
    inline bool IsOnGround() const { return isOnGround; }
    inline bool IsInvulnerable() const { return currentInvulnerabilityTime < invulnerabilityTime; }
    inline sf::Vector2f GetBaseGravity() const { return baseGravity; }

    void TakeDamage(int32_t amount, sf::Vector2f normal, float knockback);
    void SetMaxHealth(int32_t health);
    void AddMaxHealth(int32_t amount);
    void RemoveMaxHealth(int32_t amount);
    void SetCurrentHealth(int32_t health);
    void AddCurrentHealth(int32_t amount);
    void RemoveCurrentHealth(int32_t amount);
    void SetMoney(int32_t money);
    void AddMoney(int32_t amount);
    void RemoveMoney(int32_t amount);
    void SetDoubleJumps(int32_t doubleJumps);
    void AddDoubleJumps(int32_t amount);
    void RemoveDoubleJumps(int32_t amount);
    void AddItemID(int32_t itemID);
    void SetDashPower(float power);
    void AddDashPower(float amount);
    void RemoveDashPower(float amount);
    void SetDashMaxTime(float time);
    void AddDashMaxTime(float amount);
    void RemoveDashMaxTime(float amount);
    void SetDashCooldown(float time);
    void AddDashCooldown(float amount);
    void RemoveDashCooldown(float amount);
    void SetMoveSpeed(float speed);
    void AddMoveSpeed(float amount);
    void RemoveMoveSpeed(float amount);
    void SetJumpPower(float power);
    void AddJumpPower(float amount);
    void RemoveJumpPower(float amount);
    void SetJumpMaxTime(float time);
    void AddJumpMaxTime(float amount);
    void RemoveJumpMaxTime(float amount);
    void SetDoubleJumpPower(float power);
    void AddDoubleJumpPower(float amount);
    void RemoveDoubleJumpPower(float amount);
    void SetMaxJumps(int32_t jumps);
    void AddMaxJumps(int32_t amount);
    void RemoveMaxJumps(int32_t amount);
    void SetInvulnerabilityTime(float time);
    void AddInvulnerabilityTime(float amount);
    void RemoveInvulnerabilityTime(float amount);
    void SetBaseGravity(sf::Vector2f gravity);
    void AddOnTakeDamageCallback(std::function<void(Player*, int32_t)>&& callback);

    inline void ResetInterpolatedPosition() override { LastPosition = Position; deagle.ResetInterpolatedRotation(); } 
    
protected:
    static constexpr float IDLE_TO_RUNNING_MIN_SPEED = 1.0f;
    static constexpr int32_t MIN_MAX_HEALTH = 0;
    static constexpr int32_t MAX_MAX_HEALTH = 20;
    static constexpr int32_t MAX_MONEY = 1000;
    static constexpr int32_t MIN_DOUBLE_JUMPS = 0;
    static constexpr int32_t MAX_DOUBLE_JUMPS = 5;
    static constexpr float MIN_INVULNERABILITY_TIME = 0.25f;
    static constexpr float MAX_INVULNERABILITY_TIME = 3.0f;
    static constexpr float MIN_DASH_POWER = 600.0f;
    static constexpr float MAX_DASH_POWER = 1000.0f;
    static constexpr float MIN_DASH_MAX_TIME = 0.2f;
    static constexpr float MAX_DASH_MAX_TIME = 0.4f;
    static constexpr float MIN_DASH_COOLDOWN = 0.2f;
    static constexpr float MAX_DASH_COOLDOWN = 10.0f;
    static constexpr float MIN_MOVE_SPEED = 1000.0f;
    static constexpr float MAX_MOVE_SPEED = 2000.0f;
    static constexpr float MIN_JUMP_POWER = 10000.0f;
    static constexpr float MAX_JUMP_POWER = 20000.0f;
    static constexpr float MIN_JUMP_MAX_TIME = 0.6f;
    static constexpr float MAX_JUMP_MAX_TIME = 2.0f;
    static constexpr float MIN_DOUBLE_JUMP_POWER = 800.0f;
    static constexpr float MAX_DOUBLE_JUMP_POWER = 1400.0f;
    static constexpr float INVULNERABILITY_BLINK_TIME = 0.05f;
    
    DesertEagle deagle {};
    TransitionStateMachine<Player::State, Player> stateMachine {};
    AnimationPack<6> animations {}; 
    E2D::Sound sounds[4];
    E2D::Sprite sprite {};
    std::vector<int32_t> itemIDs {};
    std::vector<std::function<void(Player*, int32_t)>> onTakeDamageCallbacks;
    Hud* hud { nullptr };
    E2D::Light* light { nullptr };
    int32_t maxHealth { 10 };
    int32_t currentHealth { 10 };
    int32_t money { 0 };

    float moveSpeed { 1600.0f };
    float jumpPower { 10000.0f }; // 9000
    float jumpRemainingTime { 0.0f };
    float jumpMaxTime { 0.7f }; // 0.8

    float dashPower { 800.0f }; //1000.0f
    float dashDirection { 0.0f };
    float dashCooldown { 0.6f };
    float dashCurrentCooldown { 0.0f };
    float dashTime { 0.0f };
    float dashMaxTime { 0.3f }; //0.2f

    float doubleJumpPower { 1200.0f };
    int32_t currentJumpCount { 0 };
    int32_t maxJumps { 1 };
    bool hasPressedSpace { false };
    bool hasPressedShift { false };

    bool wasOnGround { false };
    bool isOnGround { false };
    bool isTouchingRoof { false };

    float currentInvulnerabilityTime { 1.0f };
    float invulnerabilityTime { 1.0f };
    float currentInvulnerabilityBlinkTime { 0.0f };

    sf::Vector2f baseGravity { 0.0f, 0.0f };

    enum AnimationIndex
    {
        IdleIndex = 0,
        RunIndex,
        JumpIndex,
        FallIndex,
        DashIndex,
        DeadIndex
    };

    enum SoundIndex
    {
        JumpSound = 0,
        DashSound,
        DeadSound,
        HurtSound
    };

    void CreateStateMachine();
    void CreateAnimations();
    void CreateSounds();
    void HandleInvulnerabilityBlink(float dt);
};