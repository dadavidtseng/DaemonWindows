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

	// Calculate enemies for this wave using difficulty scaling
	m_totalEnemiesInWave = static_cast<int>(m_baseEnemiesPerWave * powf(m_difficultyScaling, static_cast<float>(m_currentWaveNumber - 1)));
	m_remainingEnemies   = m_totalEnemiesInWave;

	// Scale spawn interval: faster spawns in later waves (min capped)
	m_spawnInterval = m_baseSpawnInterval / (1.0f + 0.1f * static_cast<float>(m_currentWaveNumber - 1));
	if (m_spawnInterval < m_minSpawnInterval)
	{
		m_spawnInterval = m_minSpawnInterval;
	}

	// Check if this is a boss wave (every 5 waves)
	constexpr int BOSS_WAVE_INTERVAL = 5;
	m_isBossActive = (m_currentWaveNumber % BOSS_WAVE_INTERVAL == 0);

	// Fire OnWaveStart event
	EventArgs args;
	args.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
	args.SetValue("totalEnemies", std::to_string(m_totalEnemiesInWave));
	args.SetValue("isBossWave", m_isBossActive ? "true" : "false");
	g_eventSystem->FireEvent("OnWaveStart", args);

	// Fire OnBossSpawn event if this is a boss wave
	if (m_isBossActive)
	{
		EventArgs bossArgs;
		bossArgs.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
		g_eventSystem->FireEvent("OnBossSpawn", bossArgs);
	}
}

//-----------------------------------------------------------------------------------------------
// CompleteWave - Ends the current wave
//-----------------------------------------------------------------------------------------------
void WaveManager::CompleteWave()
{
	m_isWaveActive = false;
	m_isBossActive = false;

	// Begin wave transition (pause before next wave)
	m_isInTransition      = true;
	m_waveTransitionTimer = 0.0f;

	// Fire OnWaveComplete event
	EventArgs args;
	args.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
	args.SetValue("totalEnemies", std::to_string(m_totalEnemiesInWave));
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

	// Triangle - basic chaser, always available, high weight early
	m_spawnTable.push_back({eEnemyType::TRIANGLE, 30});

	// Circle - orbiter, always available
	m_spawnTable.push_back({eEnemyType::CIRCLE, 25});

	// Octagon - ranged shooter, always available
	m_spawnTable.push_back({eEnemyType::OCTAGON, 20});

	// Square - tanky slow chaser, introduced from wave 2
	if (m_currentWaveNumber >= 2)
	{
		m_spawnTable.push_back({eEnemyType::SQUARE, 10});
	}

	// Pentagon - fast zigzag, introduced from wave 3
	if (m_currentWaveNumber >= 3)
	{
		m_spawnTable.push_back({eEnemyType::PENTAGON, 15});
	}

	// Hexagon - splits on death, introduced from wave 4
	if (m_currentWaveNumber >= 4)
	{
		m_spawnTable.push_back({eEnemyType::HEXAGON, 10});
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
