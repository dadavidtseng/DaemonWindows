//----------------------------------------------------------------------------------------------------
// UpgradeManager.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/UpgradeManager.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"


//----------------------------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------------------
UpgradeManager::UpgradeManager(Game* game)
	: m_game(game)
{
	// Initialize all upgrades with their types
	for (int i = 0; i < static_cast<int>(eUpgradeType::COUNT); ++i)
	{
		m_upgrades[i].m_type = static_cast<eUpgradeType>(i);
		m_upgrades[i].m_level = 0;
		m_upgrades[i].m_maxLevel = 5;
		m_upgrades[i].m_baseCost = 100;
	}
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

	// TODO: Implement upgrade update logic
	// - Handle upgrade effects that need per-frame updates
	// - Update visual effects for active upgrades
	// - Process upgrade-related events
}

//-----------------------------------------------------------------------------------------------
// UpgradeTypeToString - Converts upgrade type enum to string for event args
//-----------------------------------------------------------------------------------------------
static char const* UpgradeTypeToString(eUpgradeType type)
{
	switch (type)
	{
	case eUpgradeType::FIRE_RATE:        return "FireRate";
	case eUpgradeType::DAMAGE:           return "Damage";
	case eUpgradeType::PROJECTILE_COUNT: return "ProjectileCount";
	case eUpgradeType::BULLET_SPREAD:    return "BulletSpread";
	case eUpgradeType::BULLET_SIZE:      return "BulletSize";
	case eUpgradeType::PIERCING:         return "Piercing";
	case eUpgradeType::HOMING:           return "Homing";
	default:                             return "Unknown";
	}
}

//-----------------------------------------------------------------------------------------------
// PurchaseUpgrade - Attempts to purchase an upgrade level
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::PurchaseUpgrade(eUpgradeType type)
{
	// TODO: Implement full purchase logic (coin check, deduction, effect application)
	// For now, only the event firing is implemented as part of Task 1.11

	int index = static_cast<int>(type);
	if (index < 0 || index >= static_cast<int>(eUpgradeType::COUNT))
	{
		return false;
	}

	Upgrade& upgrade = m_upgrades[index];
	if (upgrade.m_level >= upgrade.m_maxLevel)
	{
		return false;
	}

	// Fire OnUpgradePurchased event
	EventArgs args;
	args.SetValue("upgradeType", UpgradeTypeToString(type));
	args.SetValue("newLevel", std::to_string(upgrade.m_level + 1));
	args.SetValue("cost", std::to_string(GetUpgradeCost(type)));
	g_eventSystem->FireEvent("OnUpgradePurchased", args);

	return false; // TODO: Return true once full purchase logic is implemented
}

//-----------------------------------------------------------------------------------------------
// GetUpgradeCost - Calculates the cost for the next level of an upgrade
//-----------------------------------------------------------------------------------------------
int UpgradeManager::GetUpgradeCost(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return 0;
	}

	// TODO: Implement cost calculation
	// - Use formula: baseCost * (costScaling ^ currentLevel)
	// - Return 0 if upgrade is maxed
	// - Example: Level 0->1 = 100, Level 1->2 = 150, Level 2->3 = 225

	UNUSED(upgrade)
	return 0;
}

//-----------------------------------------------------------------------------------------------
// IsUpgradeAvailable - Checks if an upgrade can be purchased
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::IsUpgradeAvailable(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return false;
	}

	// TODO: Implement availability check
	// - Return false if upgrade is maxed
	// - Check if player has enough coins for next level
	// - Check if any prerequisite upgrades are met
	// - Return true if upgrade can be purchased

	UNUSED(upgrade)
	return false;
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
// IsUpgradeMaxed - Checks if an upgrade has reached maximum level
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::IsUpgradeMaxed(eUpgradeType type) const
{
	Upgrade const* upgrade = GetUpgrade(type);
	if (!upgrade)
	{
		return true;
	}

	return upgrade->m_level >= upgrade->m_maxLevel;
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
