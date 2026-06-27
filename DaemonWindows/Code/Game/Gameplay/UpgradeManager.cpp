//----------------------------------------------------------------------------------------------------
// UpgradeManager.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/UpgradeManager.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"


//----------------------------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------------------
UpgradeManager::UpgradeManager(Game* game)
	: m_game(game)
{
	InitializeUpgradeDefaults();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------
UpgradeManager::~UpgradeManager()
{
}

//-----------------------------------------------------------------------------------------------
// Update - Called every frame to manage upgrade system
//-----------------------------------------------------------------------------------------------
void UpgradeManager::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//-----------------------------------------------------------------------------------------------
// UpgradeTypeToString - Converts upgrade type enum to string for event args
//-----------------------------------------------------------------------------------------------
std::string UpgradeManager::UpgradeTypeToString(eUpgradeType type)
{
	switch (type)
	{
	case eUpgradeType::SPEED:         return "Speed";
	case eUpgradeType::FIRE_RATE:     return "FireRate";
	case eUpgradeType::MULTI_SHOT:    return "MultiShot";
	case eUpgradeType::HOMING:        return "Homing";
	case eUpgradeType::WEALTH:        return "Wealth";
	case eUpgradeType::MAX_HEALTH:    return "MaxHealth";
	case eUpgradeType::SPLASH_DAMAGE: return "SplashDamage";
	case eUpgradeType::PIERCING:      return "Piercing";
	case eUpgradeType::HEAL:          return "Heal";
	default:                          return "Unknown";
	}
}

//-----------------------------------------------------------------------------------------------
// UpgradeTypeToDisplayName - Returns human-readable display name for shop UI
//-----------------------------------------------------------------------------------------------
std::string UpgradeManager::UpgradeTypeToDisplayName(eUpgradeType type)
{
	switch (type)
	{
	case eUpgradeType::SPEED:         return "speed";
	case eUpgradeType::FIRE_RATE:     return "fire rate";
	case eUpgradeType::MULTI_SHOT:    return "+1 multi\nshot";
	case eUpgradeType::HOMING:        return "homing";
	case eUpgradeType::WEALTH:        return "wealth";
	case eUpgradeType::MAX_HEALTH:    return "max\nhealth";
	case eUpgradeType::SPLASH_DAMAGE: return "splash\ndamage";
	case eUpgradeType::PIERCING:      return "piercing";
	case eUpgradeType::HEAL:          return "heal 20";
	default:                          return "unknown";
	}
}

//-----------------------------------------------------------------------------------------------
// PurchaseUpgrade - Attempts to purchase an upgrade level
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::PurchaseUpgrade(eUpgradeType type)
{
	int index = static_cast<int>(type);
	if (index < 0 || index >= static_cast<int>(eUpgradeType::COUNT))
	{
		return false;
	}

	Upgrade& upgrade = m_upgrades[index];

	// Check player has enough coins
	Player* player = m_game->GetPlayer();
	if (!player)
	{
		return false;
	}

	int cost = GetUpgradeCost(type);
	if (player->m_coin < cost)
	{
		return false;
	}

	// Deduct coins and increment level
	player->m_coin -= cost;
	player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));
	upgrade.m_level++;

	// Special handling for Heal: instant effect, then reset level back to 0
	if (type == eUpgradeType::HEAL)
	{
		int healAmount = 20;
		player->m_health = (player->m_health + healAmount > player->m_maxHealth)
			? player->m_maxHealth
			: player->m_health + healAmount;
		player->m_healthWidget->SetText(Stringf("Health=%d/%d", player->m_health, player->m_maxHealth));
		upgrade.m_level--;  // Heal is consumable, doesn't stack levels
	}

	// Special handling for Max Health: also heals 5 HP
	if (type == eUpgradeType::MAX_HEALTH)
	{
		player->m_maxHealth += 5;
		int healAmount = 5;
		player->m_health = (player->m_health + healAmount > player->m_maxHealth)
			? player->m_maxHealth
			: player->m_health + healAmount;
		player->m_healthWidget->SetText(Stringf("Health=%d/%d", player->m_health, player->m_maxHealth));
	}

	// Fire OnUpgradePurchased event
	EventArgs args;
	args.SetValue("upgradeType", UpgradeTypeToString(type));
	args.SetValue("newLevel", std::to_string(upgrade.m_level));
	args.SetValue("cost", std::to_string(cost));
	g_eventSystem->FireEvent("OnUpgradePurchased", args);

	return true;
}

//-----------------------------------------------------------------------------------------------
// GetUpgradeCost - Calculates the cost for the next level of an upgrade
// No max level: cost scales infinitely with baseCost * (costScaling ^ currentLevel)
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetUpgradeCost(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return 0;
	}

	// Cost formula: baseCost * (costScaling ^ currentLevel)
	float cost = static_cast<float>(upgrade->m_baseCost);
	for (int i = 0; i < upgrade->m_level; ++i)
	{
		cost *= m_costScaling;
	}
	return static_cast<int>(cost);
}

//-----------------------------------------------------------------------------------------------
// IsUpgradeAvailable - Checks if an upgrade can be purchased (always available if enough coins)
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::IsUpgradeAvailable(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return false;
	}

	Player* player = m_game->GetPlayer();
	if (!player)
	{
		return false;
	}

	return player->m_coin >= GetUpgradeCost(type);
}

//-----------------------------------------------------------------------------------------------
// GetUpgradeLevel - Returns the current level of an upgrade
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetUpgradeLevel(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return 0;
	}

	return upgrade->m_level;
}

//-----------------------------------------------------------------------------------------------
// GetUpgrade - Returns a pointer to the upgrade data
//-----------------------------------------------------------------------------------------------
Upgrade const* UpgradeManager::GetUpgrade(eUpgradeType type) const
{
	int index = static_cast<int>(type);
	if (index < 0 || index >= static_cast<int>(eUpgradeType::COUNT))
	{
		return nullptr;
	}

	return &m_upgrades[index];
}

//-----------------------------------------------------------------------------------------------
// Reset - Resets all upgrades to level 0
//-----------------------------------------------------------------------------------------------
void UpgradeManager::Reset()
{
	InitializeUpgradeDefaults();
}

//-----------------------------------------------------------------------------------------------
// InitializeUpgradeDefaults - Sets per-upgrade base cost (matching original WindowKill costs)
//-----------------------------------------------------------------------------------------------
void UpgradeManager::InitializeUpgradeDefaults()
{
	for (int i = 0; i < static_cast<int>(eUpgradeType::COUNT); ++i)
	{
		m_upgrades[i].m_type  = static_cast<eUpgradeType>(i);
		m_upgrades[i].m_level = 0;
	}

	// Base costs matching original WindowKill (from screenshot: speed=10, splash=42, multi=50)
	m_upgrades[static_cast<int>(eUpgradeType::SPEED)].m_baseCost         = 10;
	m_upgrades[static_cast<int>(eUpgradeType::FIRE_RATE)].m_baseCost     = 15;
	m_upgrades[static_cast<int>(eUpgradeType::MULTI_SHOT)].m_baseCost    = 50;
	m_upgrades[static_cast<int>(eUpgradeType::HOMING)].m_baseCost        = 40;
	m_upgrades[static_cast<int>(eUpgradeType::WEALTH)].m_baseCost        = 20;
	m_upgrades[static_cast<int>(eUpgradeType::MAX_HEALTH)].m_baseCost    = 25;
	m_upgrades[static_cast<int>(eUpgradeType::SPLASH_DAMAGE)].m_baseCost = 42;
	m_upgrades[static_cast<int>(eUpgradeType::PIERCING)].m_baseCost      = 35;
	m_upgrades[static_cast<int>(eUpgradeType::HEAL)].m_baseCost          = 30;
}

//-----------------------------------------------------------------------------------------------
// GetSpeedBonus - Returns additional speed per level (+15 per level)
//-----------------------------------------------------------------------------------------------
float UpgradeManager::GetSpeedBonus() const
{
	return 15.0f * static_cast<float>(GetUpgradeLevel(eUpgradeType::SPEED));
}

//-----------------------------------------------------------------------------------------------
// GetFireRateMultiplier - Returns cooldown multiplier (lower = faster firing)
// Each level reduces cooldown by 10%: 0.9^level
//-----------------------------------------------------------------------------------------------
float UpgradeManager::GetFireRateMultiplier() const
{
	int level = GetUpgradeLevel(eUpgradeType::FIRE_RATE);
	float multiplier = 1.0f;
	for (int i = 0; i < level; ++i)
	{
		multiplier *= 0.9f;
	}
	return multiplier;
}

//-----------------------------------------------------------------------------------------------
// GetProjectileCount - Returns number of bullets per shot
// Base 1, +1 per level
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetProjectileCount() const
{
	return 1 + GetUpgradeLevel(eUpgradeType::MULTI_SHOT);
}

//-----------------------------------------------------------------------------------------------
// GetHomingStrength - Returns homing turn rate in degrees per second
// 0 at base (no homing), +60 degrees/sec per level
//-----------------------------------------------------------------------------------------------
float UpgradeManager::GetHomingStrength() const
{
	return 60.0f * static_cast<float>(GetUpgradeLevel(eUpgradeType::HOMING));
}

//-----------------------------------------------------------------------------------------------
// GetWealthMultiplier - Returns coin drop multiplier
// Base 1.0, +25% per level
//-----------------------------------------------------------------------------------------------
float UpgradeManager::GetWealthMultiplier() const
{
	return 1.0f + 0.25f * static_cast<float>(GetUpgradeLevel(eUpgradeType::WEALTH));
}

//-----------------------------------------------------------------------------------------------
// GetMaxHealthBonus - Returns additional max HP from upgrades
// +5 per level (applied in PurchaseUpgrade directly)
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetMaxHealthBonus() const
{
	return 5 * GetUpgradeLevel(eUpgradeType::MAX_HEALTH);
}

//-----------------------------------------------------------------------------------------------
// GetSplashRadius - Returns splash damage radius
// 0 at base (no splash), +20 per level
//-----------------------------------------------------------------------------------------------
float UpgradeManager::GetSplashRadius() const
{
	return 20.0f * static_cast<float>(GetUpgradeLevel(eUpgradeType::SPLASH_DAMAGE));
}

//-----------------------------------------------------------------------------------------------
// GetPiercingCount - Returns number of enemies a bullet can pierce through
// 0 at base (bullet dies on first hit), +1 per level
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetPiercingCount() const
{
	return GetUpgradeLevel(eUpgradeType::PIERCING);
}
