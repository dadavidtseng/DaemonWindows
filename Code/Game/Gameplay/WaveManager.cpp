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
	UNUSED(deltaSeconds)

	// TODO: Implement wave update logic
	// - Track spawn timer
	// - Spawn regular enemies during wave
	// - Detect when all regular enemies are killed
	// - Spawn boss enemy
	// - Detect when boss is killed
	// - Complete wave and start next wave
}

//-----------------------------------------------------------------------------------------------
// StartWave - Begins a new wave
//-----------------------------------------------------------------------------------------------
void WaveManager::StartWave()
{
	++m_currentWaveNumber;
	m_isWaveActive = true;
	m_spawnTimer   = 0.0f;

	// Calculate enemies for this wave using difficulty scaling
	m_totalEnemiesInWave = static_cast<int>(m_baseEnemiesPerWave * powf(m_difficultyScaling, static_cast<float>(m_currentWaveNumber - 1)));
	m_remainingEnemies   = m_totalEnemiesInWave;

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

	// Fire OnWaveComplete event
	EventArgs args;
	args.SetValue("waveNumber", std::to_string(m_currentWaveNumber));
	args.SetValue("totalEnemies", std::to_string(m_totalEnemiesInWave));
	g_eventSystem->FireEvent("OnWaveComplete", args);
}
