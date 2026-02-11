#include "Game/Gameplay/UpgradeManager.hpp"

#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
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
// PurchaseUpgrade - Attempts to purchase an upgrade level
//-----------------------------------------------------------------------------------------------
bool UpgradeManager::PurchaseUpgrade(eUpgradeType type)
{
	UNUSED(type)

	// TODO: Implement upgrade purchase logic
	// - Check if upgrade is available (not maxed)
	// - Check if player has enough coins
	// - Deduct cost from player coins
	// - Increment upgrade level
	// - Apply upgrade effects to player
	// - Fire "UpgradePurchased" event
	// - Return true if successful, false otherwise

	return false;
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
