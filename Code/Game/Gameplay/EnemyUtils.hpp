//----------------------------------------------------------------------------------------------------
// EnemyUtils.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Engine/Math/Vec2.hpp"

//----------------------------------------------------------------------------------------------------
// EnemyUtils Namespace
// Provides pure, stateless utility functions for enemy AI behaviors.
// All functions are designed to work with any enemy type and avoid
// coupling to specific entity classes.
//-----------------------------------------------------------------------------------------------
namespace EnemyUtils
{
	// Returns the normalized direction vector from enemyPosition toward playerPosition.
	// Returns Vec2::ZERO if positions are identical.
	Vec2 GetDirectionToPlayer(Vec2 const& enemyPosition, Vec2 const& playerPosition);

	// Moves an enemy toward the player position at the given speed.
	// Updates outPosition and outOrientationDegrees based on the chase direction.
	void ChasePlayer(Vec2&       outPosition,
	                 float&      outOrientationDegrees,
	                 Vec2 const& playerPosition,
	                 float       speed,
	                 float       deltaSeconds);

	// Moves an enemy in a circular orbit around the player position.
	// Updates outPosition and outOrientationDegrees. orbitAngle is tracked across frames.
	void OrbitPlayer(Vec2&       outPosition,
	                 float&      outOrientationDegrees,
	                 Vec2 const& playerPosition,
	                 float       orbitRadius,
	                 float       angularSpeed,
	                 float&      orbitAngle,
	                 float       deltaSeconds);

	// Moves an enemy toward the player with a sinusoidal zigzag offset.
	// phase is tracked across frames to maintain smooth oscillation.
	void ZigZagToward(Vec2&       outPosition,
	                  float&      outOrientationDegrees,
	                  Vec2 const& playerPosition,
	                  float       speed,
	                  float       zigzagAmplitude,
	                  float&      phase,
	                  float       deltaSeconds);

	// Returns true if the enemy should fire at the player.
	// Checks range and cooldown timer. Resets timer on successful fire.
	bool ShouldShootAtPlayer(Vec2 const& enemyPosition,
	                         Vec2 const& playerPosition,
	                         float       range,
	                         float       cooldown,
	                         float&      timer,
	                         float       deltaSeconds);

	// Returns a random spawn position within the screen bounds.
	// The margin parameter keeps spawns away from screen edges.
	// Spawns are biased toward the screen periphery to avoid spawning on top of the player.
	Vec2 GetRandomSpawnPosition(Vec2 const& screenDimensions, float margin = 20.0f);
}
