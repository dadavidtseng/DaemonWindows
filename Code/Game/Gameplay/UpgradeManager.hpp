//----------------------------------------------------------------------------------------------------
// UpgradeManager.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
// Upgrade Types Enumeration
// Defines all available upgrade types that can be purchased in the shop
//-----------------------------------------------------------------------------------------------
enum class eUpgradeType : int8_t
{
	FIRE_RATE,        // Increases player's fire rate
	DAMAGE,           // Increases bullet damage
	PROJECTILE_COUNT, // Increases number of bullets per shot
	BULLET_SPREAD,    // Modifies bullet spread pattern
	BULLET_SIZE,      // Increases bullet size
	PIERCING,         // Allows bullets to pierce through enemies
	HOMING,           // Makes bullets home towards enemies
	
	COUNT             // Total number of upgrade types
};

//-----------------------------------------------------------------------------------------------
// Upgrade Data Structure
// Stores information about a single upgrade instance
//-----------------------------------------------------------------------------------------------
struct Upgrade
{
	eUpgradeType m_type;
	int          m_level = 0;      // Current upgrade level (0 = not purchased)
	int          m_maxLevel = 5;   // Maximum upgrade level
	int          m_baseCost = 100; // Base cost for level 1
};

//-----------------------------------------------------------------------------------------------
// UpgradeManager Class
// Manages player upgrades and progression system
//-----------------------------------------------------------------------------------------------
class UpgradeManager
{
public:
	// Constructor / Destructor
	explicit UpgradeManager(Game* game);
	~UpgradeManager();

	// Core lifecycle methods
	void Update(float deltaSeconds);

	// Upgrade management
	bool PurchaseUpgrade(eUpgradeType type);
	int  GetUpgradeCost(eUpgradeType type) const;
	bool IsUpgradeAvailable(eUpgradeType type) const;
	int  GetUpgradeLevel(eUpgradeType type) const;
	bool IsUpgradeMaxed(eUpgradeType type) const;

	// Accessors
	Upgrade const* GetUpgrade(eUpgradeType type) const;

private:
	// Game reference
	Game* m_game = nullptr;

	// Upgrade storage
	Upgrade m_upgrades[static_cast<int>(eUpgradeType::COUNT)];

	// Cost calculation
	float m_costScaling = 1.5f; // Cost multiplier per level (50% increase)
};
