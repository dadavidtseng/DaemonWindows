//----------------------------------------------------------------------------------------------------
// UpgradeManager.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include <string>

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
// Upgrade Types Enumeration
// Matches original WindowKill upgrade list
//-----------------------------------------------------------------------------------------------
enum class eUpgradeType : int8_t
{
	SPEED,          // Increases player movement speed
	FIRE_RATE,      // Increases player's fire rate
	MULTI_SHOT,     // +1 bullet per shot
	HOMING,         // Makes bullets home towards enemies
	WEALTH,         // Increases coin drops from enemies
	MAX_HEALTH,     // +5 max HP and heals 5 HP
	SPLASH_DAMAGE,  // Bullets deal area damage on hit
	PIERCING,       // Bullets pierce through enemies
	HEAL,           // Heals player for 20 HP (instant, consumable)

	COUNT           // Total number of upgrade types
};

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Upgrade Data Structure
// Stores information about a single upgrade instance
//-----------------------------------------------------------------------------------------------
struct Upgrade
{
	eUpgradeType m_type     = eUpgradeType::SPEED;
	int          m_level    = 0;      // Current upgrade level (0 = not purchased)
	int          m_baseCost = 10;     // Base cost for level 1
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
	void Reset();

	// Accessors
	Upgrade const* GetUpgrade(eUpgradeType type) const;

	// Stat getters - returns the effective value for each upgrade type
	float GetSpeedBonus() const;            // Additional speed per level
	float GetFireRateMultiplier() const;    // Multiplier on bullet cooldown (lower = faster)
	int   GetProjectileCount() const;       // Number of bullets per shot
	float GetHomingStrength() const;        // Homing turn rate (degrees per second)
	float GetWealthMultiplier() const;      // Coin drop multiplier
	int   GetMaxHealthBonus() const;        // Additional max HP
	float GetSplashRadius() const;          // Splash damage radius (0 = no splash)
	int   GetPiercingCount() const;         // Number of enemies a bullet can pierce through

	// Utility
	static std::string UpgradeTypeToString(eUpgradeType type);
	static std::string UpgradeTypeToDisplayName(eUpgradeType type);

private:
	// Per-upgrade initialization
	void InitializeUpgradeDefaults();

	// Game reference
	Game* m_game = nullptr;

	// Upgrade storage
	Upgrade m_upgrades[static_cast<int>(eUpgradeType::COUNT)];

	// Cost calculation
	float m_costScaling = 1.5f; // Cost multiplier per level (50% increase)
};
