//----------------------------------------------------------------------------------------------------
// WaveManager.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

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

	// Accessors / Mutators
	int  GetCurrentWaveNumber() const { return m_currentWaveNumber; }
	bool IsWaveActive() const { return m_isWaveActive; }
	bool IsBossActive() const { return m_isBossActive; }
	int  GetRemainingEnemies() const { return m_remainingEnemies; }

private:
	// Game reference
	Game* m_game = nullptr;

	// Wave state
	int  m_currentWaveNumber   = 0;
	bool m_isWaveActive        = false;
	bool m_isBossActive        = false;
	int  m_remainingEnemies    = 0;
	int  m_totalEnemiesInWave  = 0;

	// Spawn timing
	float m_spawnTimer         = 0.0f;
	float m_spawnInterval      = 10.0f;  // Time between enemy spawns

	// Wave configuration
	int  m_baseEnemiesPerWave  = 3;      // Base number of enemies per wave
	float m_difficultyScaling  = 1.2f;   // Multiplier for difficulty increase per wave
};
