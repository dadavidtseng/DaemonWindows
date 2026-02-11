#include "Game/Gameplay/WaveManager.hpp"

#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/EngineCommon.hpp"

//-----------------------------------------------------------------------------------------------
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
	// TODO: Implement wave start logic
	// - Increment wave number
	// - Calculate number of enemies for this wave
	// - Set wave active flag
	// - Reset spawn timer
	// - Fire wave start event
}

//-----------------------------------------------------------------------------------------------
// CompleteWave - Ends the current wave
//-----------------------------------------------------------------------------------------------
void WaveManager::CompleteWave()
{
	// TODO: Implement wave completion logic
	// - Set wave active flag to false
	// - Fire wave complete event
	// - Prepare for next wave
	// - Award wave completion bonus (coins, etc.)
}
