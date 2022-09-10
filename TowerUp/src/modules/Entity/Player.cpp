#include "Player.h"
#include "../Physics/CollisionData.h"
#include "../Engine2D/MediaContent.h"
#include "../Engine2D/Renderer.h"
#include "../SFMLUtils.hpp"
#include "../Physics/PhysicsEngine.h"
#include "World.h"
#include "../Hud.h"
#include "BaseItem.h"
#include "Enemies/Enemy.h"
#include "Bullet.h"
#include "HurtZone.h"
#include "../Engine2D/LightSystem.h"


Player::Player() : DynamicEntity(sf::Vector2f{0.0f, 0.0f}, WIDTH, HEIGHT)
{
    CreateStateMachine();
    CreateAnimations();
    CreateSounds();

    this->deagle.SetPlayer(this);

    this->SetMass(20.0f);
    this->Friction = 0.001f;
    this->CollisionType = CollisionType::PlayerCollisionType;
    this->AddCollisionMask(CollisionType::TileCollisionType);    
    this->AddCollisionMask(CollisionType::ConsumableCollisionType);
    this->AddCollisionMask(CollisionType::EnemyCollisionType);
    this->AddCollisionMask(CollisionType::EnemyBulletCollisionType);
    this->AddCollisionMask(CollisionType::HurtZoneCollisionType);
    this->baseGravity = PhysicsEngine::Instance().BaseGravity;
    this->BaseAcceleration = baseGravity;

    sprite.SetTexture(*E2D::MediaContent::Instance().GetTexture("Player"));
    sprite.setOrigin(WIDTH * 0.5f, HEIGHT * 0.5f);
    Position = LastPosition = sf::Vector2f(0.0f, 0.0f);

    hud = World::Instance().GetHud();
    hud->GetHealthBar().SetMaxHealth(maxHealth);
    hud->GetHealthBar().SetCurrentHealth(currentHealth);
}

void Player::CreateStateMachine()
{
    stateMachine.AddState(Player::State::Idle,    &Player::Idle,    &Player::OnEnterIdle,    nullptr);
    stateMachine.AddState(Player::State::Running, &Player::Running, &Player::OnEnterRunning, nullptr);
    stateMachine.AddState(Player::State::Jumping, &Player::Jumping, &Player::OnEnterJump,    nullptr);
    stateMachine.AddState(Player::State::Dashing, &Player::Dashing, &Player::OnEnterDash,    &Player::OnExitDash);
    stateMachine.AddState(Player::State::Falling, &Player::Falling, &Player::OnEnterFalling, nullptr);
    stateMachine.AddState(Player::State::Dead,    &Player::Dead,    &Player::OnEnterDead,    nullptr);

    stateMachine.AddTransitionToState(Player::State::Idle, Player::State::Jumping, &Player::ToJumping);
    stateMachine.AddTransitionToState(Player::State::Idle, Player::State::Dashing, &Player::ToDashing);
    stateMachine.AddTransitionToState(Player::State::Idle, Player::State::Running, &Player::IdleToRunning);

    stateMachine.AddTransitionToState(Player::State::Running, Player::State::Jumping, &Player::ToJumping);
    stateMachine.AddTransitionToState(Player::State::Running, Player::State::Dashing, &Player::ToDashing);
    stateMachine.AddTransitionToState(Player::State::Running, Player::State::Falling, &Player::RunningToFalling);
    stateMachine.AddTransitionToState(Player::State::Running, Player::State::Idle,    &Player::RunningToIdle);

    stateMachine.AddTransitionToState(Player::State::Jumping, Player::State::Running, &Player::ToRunning);
    stateMachine.AddTransitionToState(Player::State::Jumping, Player::State::Dashing, &Player::ToDashing);
    stateMachine.AddTransitionToState(Player::State::Jumping, Player::State::Falling, &Player::JumpingToFalling);

    stateMachine.AddTransitionToState(Player::State::Dashing, Player::State::Running, &Player::DashingToRunning);
    stateMachine.AddTransitionToState(Player::State::Dashing, Player::State::Falling, &Player::DashingToFalling);

    stateMachine.AddTransitionToState(Player::State::Falling, Player::State::Dashing, &Player::ToDashing);
    stateMachine.AddTransitionToState(Player::State::Falling, Player::State::Idle,    &Player::ToIdle);
    stateMachine.AddTransitionToState(Player::State::Falling, Player::State::Running, &Player::ToRunning);

    stateMachine.AddTransitionToAllStates(Player::State::Dead, &Player::ToDead);

    stateMachine.SetCurrentState(Player::State::Idle);
}

void Player::CreateAnimations()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& idle = animations[AnimationIndex::IdleIndex];
    idle.SetFrames(media.GetAnimationFrames("PlayerIdle"));
    idle.SetDuration(4.5f);

    auto& run = animations[AnimationIndex::RunIndex];
    run.SetFrames(media.GetAnimationFrames("PlayerRun"));
    run.SetDuration(0.4f);

    auto& jump = animations[AnimationIndex::JumpIndex];
    jump.SetFrames(media.GetAnimationFrames("PlayerJump"));
    jump.SetDuration(0.2f);

    auto& fall = animations[AnimationIndex::FallIndex];
    fall.SetFrames(media.GetAnimationFrames("PlayerFall"));
    fall.SetDuration(0.2f);

    auto& dash = animations[AnimationIndex::DashIndex];
    dash.SetFrames(media.GetAnimationFrames("PlayerDash"));
    dash.SetDuration(0.2f);

    auto& dead = animations[AnimationIndex::DeadIndex];
    dead.SetFrames(media.GetAnimationFrames("PlayerDead"));
    dead.SetDuration(1.5f);

    idle.Play(this->sprite);

    animations.ChangeAnimation(this->sprite, AnimationIndex::IdleIndex);
}

void Player::CreateSounds()
{
    E2D::MediaContent& media = E2D::MediaContent::Instance();

    auto& jump = sounds[SoundIndex::JumpSound];
    jump.SetBuffer(*media.GetSoundBuffer("PlayerJump"));
    jump.SetRelativeToListener(true); 
    jump.SetVolume(35.0f);

    auto& dash = sounds[SoundIndex::DashSound];
    dash.SetBuffer(*media.GetSoundBuffer("PlayerDash"));
    dash.SetRelativeToListener(true); 
    dash.SetVolume(35.0f);

    auto& dead = sounds[SoundIndex::DeadSound];
    dead.SetBuffer(*media.GetSoundBuffer("PlayerDead"));
    dead.SetRelativeToListener(true); 
    dead.SetVolume(35.0f);

    auto& hurt = sounds[SoundIndex::HurtSound];
    hurt.SetBuffer(*media.GetSoundBuffer("PlayerHurt"));
    hurt.SetRelativeToListener(true); 
    hurt.SetVolume(35.0f);

}

void Player::HandleEvent(const sf::Event& ev)
{
    
    if(ev.type == sf::Event::KeyPressed)
    {
        if(ev.key.code == sf::Keyboard::Space)
            hasPressedSpace = true;
        else if(ev.key.code == sf::Keyboard::LShift)
            hasPressedShift = true;
    }

    deagle.HandleReload(ev);
}

void Player::UpdateLightPosition(float tickPercent)
{
    light->Position = LastPosition * (1.0f - tickPercent) + Position * tickPercent;
}

void Player::SetLight(E2D::Light* light)
{
    this->light = light;
}

void Player::Update(float dt)
{
    deagle.Update(dt);
    stateMachine.Update(this, dt);
    animations.CurrentAnimation().Update(sprite, dt);
    HandleInvulnerabilityBlink(dt);

    E2D::Listener::SetPosition(Position);

    isOnGround = false;
    isTouchingRoof = false;
    hasPressedSpace = false;
    hasPressedShift = false;
    currentInvulnerabilityTime += dt;
}

void Player::HandleInvulnerabilityBlink(float dt)
{
    if(currentInvulnerabilityTime < invulnerabilityTime)
    {
        currentInvulnerabilityBlinkTime += dt;

        if(currentInvulnerabilityBlinkTime >= INVULNERABILITY_BLINK_TIME)
        {
            currentInvulnerabilityBlinkTime = 0.0f;
            sprite.InvertAlpha();
        }
    }
    else
        sprite.SetAlpha(255);  
}

void Player::Idle(float dt)
{
    HandleHorizontalMovement(dt);
    HandleOrientation();

    dashCurrentCooldown -= dt;
}

void Player::Running(float dt)
{
    HandleHorizontalMovement(dt);
    HandleOrientation();

    if(deagle.GetAimDirection().x * Velocity.x < 0)
        animations[AnimationIndex::RunIndex].SetReversed(true);
    else
        animations[AnimationIndex::RunIndex].SetReversed(false);

    dashCurrentCooldown -= dt;
}

void Player::Jumping(float dt)
{
    HandleHorizontalMovement(dt);
    HandleOrientation();

    float jumpAmount = (jumpRemainingTime / jumpMaxTime);
    Velocity.y -= jumpAmount * jumpPower * dt;

    jumpRemainingTime -= dt;
    dashCurrentCooldown -= dt;
}

void Player::Dashing(float dt)
{
    Velocity = sf::Vector2f{dashDirection * dashPower, 0.0f};

    dashTime += dt;
}

void Player::Dead([[maybe_unused]] float dt)
{
    
}

void Player::Falling(float dt)
{
    HandleHorizontalMovement(dt);
    HandleOrientation();

    if(hasPressedSpace && currentJumpCount < maxJumps)
    {
        PlaySound(sounds[SoundIndex::JumpSound]);
        Velocity.y -= doubleJumpPower;
        ++currentJumpCount;
    }

    dashCurrentCooldown -= dt;
}


void Player::HandleHorizontalMovement(float dt)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        Velocity.x += moveSpeed * dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        Velocity.x -= moveSpeed * dt;
}

void Player::HandleOrientation()
{
    deagle.CalculateOrientation();

    if(deagle.GetAimDirection().x > 0.0f)
    {
        sprite.SetFlipHorizontally(false);
        deagle.GetSprite().SetFlipVertically(false);
    }
    else
    {
        sprite.SetFlipHorizontally(true);
        deagle.GetSprite().SetFlipVertically(true);
    }

    deagle.HandleShoot();
}

void Player::OnEnterIdle()
{
    animations.ChangeAnimation(this->sprite, AnimationIndex::IdleIndex);
    currentJumpCount = 0;
}

void Player::OnEnterRunning()
{
    animations.ChangeAnimation(this->sprite, AnimationIndex::RunIndex);
    currentJumpCount = 0;
}

void Player::OnEnterJump()
{
    animations.ChangeAnimation(this->sprite, AnimationIndex::JumpIndex);
    PlaySound(sounds[SoundIndex::JumpSound]);
    jumpRemainingTime = jumpMaxTime;
}

void Player::OnEnterDash()
{
    this->RemoveCollisionMask(CollisionType::EnemyBulletCollisionType | 
                              CollisionType::EnemyCollisionType | 
                              CollisionType::HurtZoneCollisionType);
    animations.ChangeAnimation(this->sprite, AnimationIndex::DashIndex);
    PlaySound(sounds[SoundIndex::DashSound]);
    BaseAcceleration = sf::Vector2f{ 0.0f, 0.0f };
    wasOnGround = isOnGround;
    dashTime = 0.0f;

    deagle.ResetOrientation();
    deagle.GetSprite().SetFlipVertically(false);
    
    bool dKey = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool aKey = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    //Si tiene una de las dos teclas pulsadas
    if( (dKey || aKey) && !(dKey && aKey))
    {
        if(dKey)
        {
            dashDirection =  1.0f;
            sprite.SetFlipHorizontally(false);
            deagle.GetSprite().SetFlipHorizontally(false);
        }
        else
        {
            dashDirection = -1.0f;
            sprite.SetFlipHorizontally(true);
            deagle.GetSprite().SetFlipHorizontally(true);
        }
    }
    else //Hacer el dash en la direccion del raton
    {
        sf::Vector2f dir = E2D::Renderer::Instance().GetMousePositionWithCamera() - this->Position;
        if(dir.x > 0.0f) //Derecha
        {
            dashDirection =  1.0f;
            sprite.SetFlipHorizontally(false);
        }
        else          //Izquierda
        {
            dashDirection = -1.0f;
            sprite.SetFlipHorizontally(true);
            deagle.GetSprite().SetFlipHorizontally(true);
        }
    }
}

void Player::OnExitDash()
{
    this->AddCollisionMask(CollisionType::EnemyBulletCollisionType | 
                           CollisionType::EnemyCollisionType | 
                           CollisionType::HurtZoneCollisionType);
    isOnGround = true;
    BaseAcceleration = baseGravity;
    Velocity = sf::Vector2f{ 0.0f, 0.0f };
    dashCurrentCooldown = dashCooldown;

    deagle.GetSprite().SetFlipHorizontally(false);
    deagle.ResetInterpolatedRotation();
}

void Player::OnEnterFalling()
{
    animations.ChangeAnimation(this->sprite, AnimationIndex::FallIndex);
}

void Player::OnEnterDead()
{
    this->RemoveCollisionMask(CollisionType::EnemyCollisionType);
    this->RemoveCollisionMask(CollisionType::EnemyBulletCollisionType);
    this->RemoveCollisionMask(CollisionType::HurtZoneCollisionType);
    this->RemoveCollisionMask(CollisionType::ConsumableCollisionType);
    animations.ChangeAnimation(this->sprite, AnimationIndex::DeadIndex);
    PlaySound(sounds[SoundIndex::DeadSound]);
    deagle.RemoveDraw();
    IsDead = true;
}

//!Transiciones a Idle
bool Player::ToIdle()
{
    return std::abs(Velocity.x) < IDLE_TO_RUNNING_MIN_SPEED && isOnGround; 
}

bool Player::RunningToIdle()
{
    return std::abs(Velocity.x) < IDLE_TO_RUNNING_MIN_SPEED;
}

//!Transiciones a Running

bool Player::ToRunning()
{
    return isOnGround;
}

bool Player::IdleToRunning()
{
    return std::abs(Velocity.x) > IDLE_TO_RUNNING_MIN_SPEED;
}

bool Player::DashingToRunning()
{
    return wasOnGround && dashTime >= dashMaxTime;
}

//!Transiciones a Jumping
bool Player::ToJumping()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround;
}

//!Transiciones a Falling
bool Player::RunningToFalling()
{
    return !isOnGround;  
}

bool Player::JumpingToFalling()
{
    return !isOnGround && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || jumpRemainingTime <= 0.0f || isTouchingRoof);
}

bool Player::DashingToFalling()
{
    return dashTime >= dashMaxTime;
}

bool Player::ToDashing()
{
    return hasPressedShift && dashCurrentCooldown <= 0.0f; 
}

//! Transicion a Dead
bool Player::ToDead()
{
    return currentHealth <= 0.0f;
}

void Player::Draw(float tickPercent)
{
    sf::Vector2f interpolatedPos = GetInterpolatedPosition(tickPercent);
    
    E2D::Renderer::Instance().Draw(interpolatedPos, sprite);
    deagle.Draw(tickPercent, interpolatedPos);
}

//! Setters

void Player::SetMaxHealth(int32_t health)
{
    if(health % 2 != 0)
        --health;

    maxHealth = health;
    if(maxHealth > MAX_MAX_HEALTH)
        maxHealth = MAX_MAX_HEALTH;
    else if(maxHealth < MIN_MAX_HEALTH)
        maxHealth = MIN_MAX_HEALTH;
    hud->GetHealthBar().SetMaxHealth(maxHealth);

    if(currentHealth > maxHealth)
    {
        currentHealth = maxHealth;
        hud->GetHealthBar().SetCurrentHealth(currentHealth);
    }    
}

void Player::AddMaxHealth(int32_t amount)
{
    if(amount % 2 != 0)
        ++amount;

    maxHealth += amount;
    if(maxHealth > MAX_MAX_HEALTH)
        maxHealth = MAX_MAX_HEALTH;

    hud->GetHealthBar().SetMaxHealth(maxHealth);
}

void Player::RemoveMaxHealth(int32_t amount)
{
    if(amount % 2 != 0)
        --amount;

    maxHealth -= amount;
    if(maxHealth < MIN_MAX_HEALTH)
        maxHealth = MIN_MAX_HEALTH;
    hud->GetHealthBar().SetMaxHealth(maxHealth);
    
    if(currentHealth > maxHealth)
    {
        currentHealth = maxHealth;
        hud->GetHealthBar().SetCurrentHealth(currentHealth);
    }    
}

void Player::SetCurrentHealth(int32_t health)
{
    currentHealth = health;
    if(currentHealth > maxHealth)
        currentHealth = maxHealth;

    hud->GetHealthBar().SetCurrentHealth(currentHealth);
}

void Player::AddCurrentHealth(int32_t amount)
{
    currentHealth += amount;
    if(currentHealth > maxHealth)
        currentHealth = maxHealth;

    hud->GetHealthBar().SetCurrentHealth(currentHealth);
}

void Player::RemoveCurrentHealth(int32_t amount)
{
    currentHealth -= amount;
    if(currentHealth < MIN_MAX_HEALTH)
        currentHealth = MIN_MAX_HEALTH;

    hud->GetHealthBar().SetCurrentHealth(currentHealth);
}

void Player::SetMoney(int32_t money)
{
    this->money = money;
    if(this->money > MAX_MONEY)
        this->money = MAX_MONEY;
    else if(this->money < 0)
        this->money = 0;

    hud->GetMoneyBar().SetMoney(money);
}

void Player::AddMoney(int32_t amount)
{
    money += amount;
    if(money > MAX_MONEY)
        money = MAX_MONEY;

    hud->GetMoneyBar().SetMoney(money);
}

void Player::RemoveMoney(int32_t amount)
{
    money -= amount;
    if(money < 0)
        money = 0;

    hud->GetMoneyBar().SetMoney(money);
}

void Player::SetDoubleJumps(int32_t doubleJumps)
{
    maxJumps = doubleJumps;

    if(maxJumps < MIN_DOUBLE_JUMPS)
        maxJumps = MIN_DOUBLE_JUMPS;
    else if(maxJumps > MAX_DOUBLE_JUMPS)
        maxJumps = MAX_DOUBLE_JUMPS;
}

void Player::AddDoubleJumps(int32_t amount)
{
    maxJumps += amount;

    if(maxJumps > MAX_DOUBLE_JUMPS)
        maxJumps = MAX_DOUBLE_JUMPS;
}

void Player::RemoveDoubleJumps(int32_t amount)
{
    maxJumps -= amount;

    if(maxJumps < MIN_DOUBLE_JUMPS)
        maxJumps = MIN_DOUBLE_JUMPS;
}

void Player::AddItemID(int32_t itemID)
{
    itemIDs.push_back(itemID);
}

void Player::SetDashPower(float power)
{
    dashPower = power;
    if(dashPower > MAX_DASH_POWER)
        dashPower = MAX_DASH_POWER;
    else if(dashPower < MIN_DASH_POWER)
        dashPower = MIN_DASH_POWER;
}

void Player::AddDashPower(float amount)
{
    dashPower += amount;
    if(dashPower > MAX_DASH_POWER)
        dashPower = MAX_DASH_POWER;
}

void Player::RemoveDashPower(float amount)
{
    dashPower -= amount;
    if(dashPower < MIN_DASH_POWER)
        dashPower = MIN_DASH_POWER;
}

void Player::SetDashMaxTime(float time)
{
    dashMaxTime = time;
    if(dashMaxTime > MAX_DASH_MAX_TIME)
        dashMaxTime = MAX_DASH_POWER;
    else if(dashMaxTime < MIN_DASH_MAX_TIME)
        dashMaxTime = MIN_DASH_MAX_TIME;
}

void Player::AddDashMaxTime(float amount)
{
    dashMaxTime += amount;
    if(dashMaxTime > MAX_DASH_MAX_TIME)
        dashMaxTime = MAX_DASH_MAX_TIME;
}

void Player::RemoveDashMaxTime(float amount)
{
    dashMaxTime -= amount;
    if(dashMaxTime < MIN_DASH_MAX_TIME)
        dashMaxTime = MIN_DASH_MAX_TIME;
}

void Player::SetDashCooldown(float time)
{
    dashCooldown = time;
    if(dashCooldown > MAX_DASH_COOLDOWN)
        dashCooldown = MAX_DASH_COOLDOWN;
    else if(dashCooldown < MIN_DASH_COOLDOWN)
        dashCooldown = MIN_DASH_COOLDOWN;
}

void Player::AddDashCooldown(float amount)
{
    dashCooldown += amount;
    if(dashCooldown > MAX_DASH_COOLDOWN)
        dashCooldown = MAX_DASH_COOLDOWN;
}

void Player::RemoveDashCooldown(float amount)
{
    dashCooldown -= amount;
    if(dashCooldown < MIN_DASH_COOLDOWN)
        dashCooldown = MIN_DASH_COOLDOWN;
}

void Player::SetMoveSpeed(float speed)
{
    moveSpeed = speed;
    if(moveSpeed > MAX_MOVE_SPEED)
        moveSpeed = MAX_MOVE_SPEED;
    else if(moveSpeed < MIN_MOVE_SPEED)
        moveSpeed = MIN_MOVE_SPEED;
}

void Player::AddMoveSpeed(float amount)
{
    moveSpeed += amount;
    if(moveSpeed > MAX_MOVE_SPEED)
        moveSpeed = MAX_MOVE_SPEED;
}

void Player::RemoveMoveSpeed(float amount)
{
    moveSpeed -= amount;
    if(moveSpeed < MIN_MOVE_SPEED)
        moveSpeed = MIN_MOVE_SPEED;
}

void Player::SetJumpPower(float power)
{
    jumpPower = power;
    if(jumpPower > MAX_JUMP_POWER)
        jumpPower = MAX_JUMP_POWER;
    else if(jumpPower < MIN_JUMP_POWER)
        jumpPower = MIN_JUMP_POWER;
}

void Player::AddJumpPower(float amount)
{
    jumpPower += amount;
    if(jumpPower > MAX_JUMP_POWER)
        jumpPower = MAX_JUMP_POWER;
}

void Player::RemoveJumpPower(float amount)
{
    jumpPower -= amount;
    if(jumpPower < MIN_JUMP_POWER)
        jumpPower = MIN_JUMP_POWER;
}

void Player::SetJumpMaxTime(float time)
{
    jumpMaxTime = time;
    if(jumpMaxTime > MAX_JUMP_MAX_TIME)
        jumpMaxTime = MAX_JUMP_MAX_TIME;
    else if(jumpMaxTime < MIN_JUMP_MAX_TIME)
        jumpMaxTime = MIN_JUMP_MAX_TIME;
}

void Player::AddJumpMaxTime(float amount)
{
    jumpMaxTime += amount;
    if(jumpMaxTime > MAX_JUMP_MAX_TIME)
        jumpMaxTime = MAX_JUMP_MAX_TIME;
}

void Player::RemoveJumpMaxTime(float amount)
{
    jumpMaxTime -= amount;
    if(jumpMaxTime < MIN_JUMP_MAX_TIME)
        jumpMaxTime = MIN_JUMP_MAX_TIME;
}

void Player::SetDoubleJumpPower(float power)
{
    doubleJumpPower = power;
    if(doubleJumpPower > MAX_DOUBLE_JUMP_POWER)
        doubleJumpPower = MAX_DOUBLE_JUMP_POWER;
    else if(doubleJumpPower < MIN_DOUBLE_JUMP_POWER)
        doubleJumpPower = MIN_DOUBLE_JUMP_POWER;
}

void Player::AddDoubleJumpPower(float amount)
{
    doubleJumpPower += amount;
    if(doubleJumpPower > MAX_DOUBLE_JUMP_POWER)
        doubleJumpPower = MAX_DOUBLE_JUMP_POWER;
}

void Player::RemoveDoubleJumpPower(float amount)
{
    doubleJumpPower -= amount;
    if(doubleJumpPower < MIN_DOUBLE_JUMP_POWER)
        doubleJumpPower = MIN_DOUBLE_JUMP_POWER;
}

void Player::SetInvulnerabilityTime(float time)
{
    invulnerabilityTime = time;
    if(invulnerabilityTime > MAX_INVULNERABILITY_TIME)
        invulnerabilityTime = MAX_INVULNERABILITY_TIME;
    else if(invulnerabilityTime < MIN_INVULNERABILITY_TIME)
        invulnerabilityTime = MIN_INVULNERABILITY_TIME;
}

void Player::AddInvulnerabilityTime(float amount)
{
    invulnerabilityTime += amount;
    if(invulnerabilityTime > MAX_INVULNERABILITY_TIME)
        invulnerabilityTime = MAX_INVULNERABILITY_TIME;
}

void Player::RemoveInvulnerabilityTime(float amount)
{
    invulnerabilityTime -= amount;

    if(invulnerabilityTime < MIN_INVULNERABILITY_TIME)
        invulnerabilityTime = MIN_INVULNERABILITY_TIME;
}

void Player::SetBaseGravity(sf::Vector2f gravity)
{
    baseGravity = gravity;
    this->BaseAcceleration = gravity;
}

void Player::AddOnTakeDamageCallback(std::function<void(Player*, int32_t)>&& callback)
{
    onTakeDamageCallbacks.emplace_back(callback);
}

void Player::TakeDamage(int32_t amount, sf::Vector2f normal, float knockback)
{
    if(currentInvulnerabilityTime > invulnerabilityTime && !World::Instance().CheatsOn())
    {
        PlaySound(sounds[SoundIndex::HurtSound]);
        currentInvulnerabilityTime = 0.0f;
        currentInvulnerabilityBlinkTime = 0.0f;
        RemoveCurrentHealth(amount);
        Velocity += normal * knockback;

        for(auto& c : onTakeDamageCallbacks)
            c(this, amount);
    }
}

void Player::OnVerticalTileCollision(const TileCollisionData& data)
{
    DynamicEntity::OnVerticalTileCollision(data);
    if(data.Normal.y == -1.0f)
        isOnGround = true;
    else
        isTouchingRoof = true;

    if(data.TileType == 2 && !IsDead)
    {
        TakeDamage(1, data.Normal, 500.0f);
    }
}

void Player::OnHorizontalTileCollision(const TileCollisionData& data)
{
    DynamicEntity::OnHorizontalTileCollision(data);

    if(data.TileType == 3 && !IsDead)
    {
        TakeDamage(1, data.Normal, 500.0f);
    }
}

void Player::OnCollision(Enemy* enemy, const CollisionData& data)
{
    TakeDamage(enemy->GetAttack(), data.Normal, enemy->GetKnockback());
}

void Player::OnCollision(Bullet* bullet, const CollisionData& data)
{
    TakeDamage(bullet->PlayerDamage, data.Normal, bullet->KnockBack);
}

void Player::OnCollision(HurtZone* hurtZone, const CollisionData& data)
{
    TakeDamage(hurtZone->PlayerDamage, data.Normal, hurtZone->KnockBack);
}

void Player::OnCollision(CollidableEntity* other, const CollisionData& data)
{
    other->OnCollision(this, data);
}