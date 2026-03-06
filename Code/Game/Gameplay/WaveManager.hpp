//----------------------------------------------------------------------------------------------------
// WaveManager.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include <vector>

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
// Enemy type identifiers for spawn weight system
//----------------------------------------------------------------------------------------------------
enum class eEnemyType
{
	// Tier 1 - Regular enemies
	TRIANGLE,
	CIRCLE,
	OCTAGON,
	SQUARE,
	PENTAGON,
	HEXAGON,

	// Tier 2 - Elite enemies (reserved for future sprints)
	// SPAWNER,
	// TELEPORTER,
	// SHIELDED,
	// KAMIKAZE,

	NUM_ENEMY_TYPES
};

//----------------------------------------------------------------------------------------------------
// Spawn weight entry: pairs an enemy type with a relative spawn weight
// Higher weight = more likely to be selected
//----------------------------------------------------------------------------------------------------
struct SpawnWeightEntry
{
	eEnemyType type   = eEnemyType::TRIANGLE;
	int        weight = 1;
};

//----------------------------------------------------------------------------------------------------
// WaveManager Class
// Manages wave-based enemy spawning system with progressive difficulty
//-----------------------------------------------------------------------------------------------
class WaveManager
{
public:
	// Constructor / Destructor
	explicit WaveManager(Game* game);
	~WaveManager();

	// Core lifecycle methods
	void Update(float deltaSeconds);
	void StartWave();
	void CompleteWave();
	void Reset();

	// Spawn weight system
	eEnemyType SelectRandomEnemyType() const;

	// Accessors / Mutators
	int  GetCurrentWaveNumber() const { return m_currentWaveNumber; }
	bool IsWaveActive() const { return m_isWaveActive; }
	bool IsBossActive() const { return m_isBossActive; }
	int  GetRemainingEnemies() const { return m_remainingEnemies; }
	int  GetEnemiesSpawned() const { return m_enemiesSpawnedThisWave; }
	int  GetTotalEnemiesInWave() const { return m_totalEnemiesInWave; }
	std::vector<SpawnWeightEntry> const& GetSpawnTable() const { return m_spawnTable; }

private:
	// Spawn table management
	void BuildSpawnTable();
	int  GetTotalSpawnWeight() const;
	int  CountAliveEnemies() const;

	// Game reference
	Game* m_game = nullptr;

	// Wave state
	int  m_currentWaveNumber      = 0;
	bool m_isWaveActive           = false;
	bool m_isBossActive           = false;
	int  m_remainingEnemies       = 0;
	int  m_totalEnemiesInWave     = 0;
	int  m_enemiesSpawnedThisWave = 0;

	// Spawn timing
	float m_spawnTimer            = 0.0f;
	float m_spawnInterval         = 1.0f;   // Time between enemy spawns (seconds)
	float m_baseSpawnInterval     = 1.0f;   // Starting spawn interval
	float m_minSpawnInterval      = 0.3f;   // Fastest possible spawn rate

	// Wave transition
	float m_waveTransitionTimer   = 0.0f;
	float m_waveTransitionDelay   = 2.0f;   // Pause between waves (seconds)
	bool  m_isInTransition        = false;

	// Wave configuration
	int   m_baseEnemiesPerWave    = 5;      // Base number of enemies per wave
	float m_difficultyScaling     = 1.5f;   // Multiplier for difficulty increase per wave

	// Spawn weight table for current wave
	std::vector<SpawnWeightEntry> m_spawnTable;
};
