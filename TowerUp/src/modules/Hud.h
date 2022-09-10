#pragma once
#include "HealthBar.h"
#include "AmmoBar.h"
#include "ItemBar.h"
#include "MoneyBar.h"
#include "BossHealthBar.h"

class Player;

class Hud
{
public:
    Hud();

    void Update(float dt);
    void SetPlayerData(Player* player);
    void AddBossHealthBar(BossHealthBar* bossHealthBar);
    void RemoveBossHealthBar(BossHealthBar* bossHealthBar);
    void Draw(float tickPercent);

    inline HealthBar& GetHealthBar() { return healthBar; }
    inline AmmoBar& GetAmmoBar() { return ammoBar; }
    inline MoneyBar& GetMoneyBar() { return moneyBar; }
    inline ItemBar& GetItemBar() { return items; }

protected:
    HealthBar healthBar;
    AmmoBar ammoBar;
    MoneyBar moneyBar;
    ItemBar items;
    std::vector<BossHealthBar*> bossesHealthBars;;
};