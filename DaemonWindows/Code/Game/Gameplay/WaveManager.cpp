//----------------------------------------------------------------------------------------------------
// WaveManager.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/WaveManager.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"


//----------------------------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------------------
WaveManager::WaveManager(Game* game)
	: m_game(game)
{
}

//-----------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------
WaveManager::~WaveManager()
{
}

//-----------------------------------------------------------------------------------------------
// Update - Called every frame to manage wave progression
//-----------------------------------------------------------------------------------------------
void WaveManager::Update(float deltaSeconds)
{
	// Wave transition: pause between waves
	if (m_isInTransition)
	{
		m_waveTransitionTimer += deltaSeconds;
		if (m_waveTransitionTimer >= m_waveTransitionDelay)
		{
			m_isInTransition = false;
			m_waveTransitionTimer = 0.0f;
			StartWave();
		}
		return;
	}

	// Auto-start first wave
	if (m_currentWaveNumber == 0)
	{
		StartWave();
		return;
	}

	if (!m_isWaveActive) return;

	// Boss phase: no regular enemy spawning, just wait for all entities to die
	if (m_isBossActive)
	{
		int const aliveEnemies = CountAliveEnemies();
		m_remainingEnemies = aliveEnemies;

		if (aliveEnemies == 0)
		{
			CompleteWave();
		}
		return;
	}

	// Spawn enemies on timer until all enemies for this wave have been spawned
	if (m_enemiesSpawnedThisWave < m_totalEnemiesInWave)
	{
		m_spawnTimer += deltaSeconds;
		if (m_spawnTimer >= m_spawnInterval)
		{
			m_spawnTimer = 0.0f;
			m_game->SpawnEnemyByType(SelectRandomEnemyType());
			++m_enemiesSpawnedThisWave;
		}
	}

	// Check for wave completion: all enemies spawned AND all killed
	int const aliveEnemies = CountAliveEnemies();
	m_remainingEnemies = aliveEnemies;

	if (m_enemiesSpawnedThisWave >= m_totalEnemiesInWave && aliveEnemies == 0)
	{
		CompleteWave();
	}
}

//-----------------------------------------------------------------------------------------------
// StartWave - Begins a new wave
//-----------------------------------------------------------------------------------------------
void WaveManager::StartWave()
{
	++m_currentWaveNumber;
	m_isWaveActive           = true;
	m_spawnTimer             = 0.0f;
	m_enemiesSpawnedThisWave = 0;

	// Build spawn weight table for this wave
	BuildSpawnTable();

	// Calculate enemies for this wave: linear growth with hard cap
	m_totalEnemiesInWave = m_baseEnemiesPerWave + (m_currentWaveNumber - 1) * m_enemiesPerWaveGrowth;
	if (m_totalEnemiesInWave > m_maxEnemiesPerWave)
	{
		m_totalEnemiesInWave = m_maxEnemiesPerWave;
	}
	m_remainingEnemies = m_totalEnemiesInWave;

	// Scale spawn interval: faster spawns in later waves (min capped)
	m_spawnInterval = m_baseSpawnInterval / (1.0f + 0.1f * static_cast<float>(m_currentWaveNumber - 1));
	if (m_spawnInterval < m_minSpawnInterval)
	{
		m_spawnInterval = m_minSpawnInterval;
	}

	// Check if this is a boss wave (every 5 waves)
	constexpr int BOSS_WAVE_INTERVAL = 5;
	m_isBossActive = (m_currentWaveNumber % BOSS_WAVE_INTERVAL == 0);

	// Boss wave: spawn boss, no regular enemies
	if (m_isBossActive)
	{
		m_totalEnemiesInWave     = 0;  // no regular enemies during boss phase
		m_enemiesSpawnedThisWave = 0;

		eEnemyType const bossType = SelectBossType();
		m_game->SpawnEnemyByType(bossType);

		// Fire OnBossSpawn event with boss type info
		EventArgs bossArgs;
		bossArgs.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
		bossArgs.SetValue("bossType", std::to_string(static_cast<int>(bossType)));
		g_eventSystem->FireEvent("OnBossSpawn", bossArgs);
	}

	// Fire OnWaveStart event
	EventArgs args;
	args.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
	args.SetValue("totalEnemies", std::to_string(m_totalEnemiesInWave));
	args.SetValue("isBossWave", m_isBossActive ? "true" : "false");
	g_eventSystem->FireEvent("OnWaveStart", args);
}

//-----------------------------------------------------------------------------------------------
// CompleteWave - Ends the current wave
//-----------------------------------------------------------------------------------------------
void WaveManager::CompleteWave()
{
	// Capture boss state before clearing it
	bool const wasBossWave = m_isBossActive;

	m_isWaveActive = false;
	m_isBossActive = false;

	// Begin wave transition (pause before next wave)
	m_isInTransition      = true;
	m_waveTransitionTimer = 0.0f;

	// Fire OnWaveComplete event
	EventArgs args;
	args.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
	args.SetValue("totalEnemies", std::to_string(m_totalEnemiesInWave));
	args.SetValue("wasBossWave", wasBossWave ? "true" : "false");
	g_eventSystem->FireEvent("OnWaveComplete", args);
}

//-----------------------------------------------------------------------------------------------
// Reset - Resets all wave state for a new game session
//-----------------------------------------------------------------------------------------------
void WaveManager::Reset()
{
	m_currentWaveNumber      = 0;
	m_isWaveActive           = false;
	m_isBossActive           = false;
	m_remainingEnemies       = 0;
	m_totalEnemiesInWave     = 0;
	m_enemiesSpawnedThisWave = 0;
	m_spawnTimer             = 0.0f;
	m_spawnInterval          = m_baseSpawnInterval;
	m_isInTransition         = false;
	m_waveTransitionTimer    = 0.0f;
	m_spawnTable.clear();
	m_bossSpawnCount = 0;
}

//-----------------------------------------------------------------------------------------------
// CountAliveEnemies - Counts living enemies (excludes player, bullets, coins, shop, debris)
//-----------------------------------------------------------------------------------------------
int WaveManager::CountAliveEnemies() const
{
	int count = 0;
	for (Entity* entity : m_game->m_entityList)
	{
		if (entity && !entity->IsDead() && Game::IsEnemy(entity))
		{
			++count;
		}
	}
	return count;
}

//-----------------------------------------------------------------------------------------------
// BuildSpawnTable - Configures spawn weights based on current wave number
// Wave 1: Only Triangle, Circle, Octagon (basic enemies)
// Wave 2: Adds Square (tanky)
// Wave 3: Adds Pentagon (fast zigzag)
// Wave 4+: All Tier 1 enemies including Hexagon (splitter)
//-----------------------------------------------------------------------------------------------
void WaveManager::BuildSpawnTable()
{
	m_spawnTable.clear();

	int const wave = m_currentWaveNumber;

	// Basic enemies: high weight early, taper off as harder enemies appear
	// Triangle - basic chaser, always available
	int const triangleWeight = (wave <= 3) ? 30 : 20;
	m_spawnTable.push_back({eEnemyType::TRIANGLE, triangleWeight});

	// Circle - orbiter, always available
	int const circleWeight = (wave <= 3) ? 25 : 15;
	m_spawnTable.push_back({eEnemyType::CIRCLE, circleWeight});

	// Octagon - ranged shooter, always available
	int const octagonWeight = (wave <= 3) ? 20 : 15;
	m_spawnTable.push_back({eEnemyType::OCTAGON, octagonWeight});

	// Square - tanky slow chaser, introduced from wave 2
	if (wave >= 2)
	{
		int const squareWeight = (wave >= 6) ? 15 : 10;
		m_spawnTable.push_back({eEnemyType::SQUARE, squareWeight});
	}

	// Pentagon - fast zigzag, introduced from wave 3
	if (wave >= 3)
	{
		int const pentagonWeight = (wave >= 6) ? 20 : 12;
		m_spawnTable.push_back({eEnemyType::PENTAGON, pentagonWeight});
	}

	// Hexagon - splits on death, introduced from wave 4
	if (wave >= 4)
	{
		int const hexagonWeight = (wave >= 8) ? 15 : 8;
		m_spawnTable.push_back({eEnemyType::HEXAGON, hexagonWeight});
	}
}

//-----------------------------------------------------------------------------------------------
// GetTotalSpawnWeight - Sums all weights in the current spawn table
//-----------------------------------------------------------------------------------------------
int WaveManager::GetTotalSpawnWeight() const
{
	int totalWeight = 0;
	for (SpawnWeightEntry const& entry : m_spawnTable)
	{
		totalWeight += entry.weight;
	}
	return totalWeight;
}

//-----------------------------------------------------------------------------------------------
// SelectRandomEnemyType - Weighted random selection from the spawn table
// Returns a random enemy type based on configured spawn weights
//-----------------------------------------------------------------------------------------------
eEnemyType WaveManager::SelectRandomEnemyType() const
{
	int const totalWeight = GetTotalSpawnWeight();
	if (totalWeight <= 0 || m_spawnTable.empty())
	{
		return eEnemyType::TRIANGLE;  // Fallback to basic enemy
	}

	int roll = g_rng->RollRandomIntInRange(1, totalWeight);

	for (SpawnWeightEntry const& entry : m_spawnTable)
	{
		roll -= entry.weight;
		if (roll <= 0)
		{
			return entry.type;
		}
	}

	// Should never reach here, but fallback to last entry
	return m_spawnTable.back().type;
}

//-----------------------------------------------------------------------------------------------
// SelectBossType - Rotates through boss types: Spiker -> Wyrm -> Slimest -> repeat
//-----------------------------------------------------------------------------------------------
eEnemyType WaveManager::SelectBossType()
{
	constexpr eEnemyType BOSS_ROTATION[] = {
		eEnemyType::SPIKER,
		eEnemyType::WYRM,
		eEnemyType::SLIMEST,
	};
	constexpr int NUM_BOSSES = 3;

	eEnemyType const bossType = BOSS_ROTATION[m_bossSpawnCount % NUM_BOSSES];
	++m_bossSpawnCount;
	return bossType;
}
