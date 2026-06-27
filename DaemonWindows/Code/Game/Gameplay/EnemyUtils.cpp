//----------------------------------------------------------------------------------------------------
// EnemyUtils.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/EnemyUtils.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Game/Framework/GameCommon.hpp"


//----------------------------------------------------------------------------------------------------
Vec2 EnemyUtils::GetDirectionToPlayer(Vec2 const& enemyPosition, Vec2 const& playerPosition)
{
	Vec2 const displacement = playerPosition - enemyPosition;

	if (displacement.GetLengthSquared() < 0.0001f)
	{
		return Vec2::ZERO;
	}

	return displacement.GetNormalized();
}

//-----------------------------------------------------------------------------------------------
void EnemyUtils::ChasePlayer(Vec2&       outPosition,
                             float&      outOrientationDegrees,
                             Vec2 const& playerPosition,
                             float       speed,
                             float       deltaSeconds)
{
	Vec2 const direction = GetDirectionToPlayer(outPosition, playerPosition);

	if (direction == Vec2::ZERO)
	{
		return;
	}

	float const goalDegrees = direction.GetOrientationDegrees();
	outOrientationDegrees   = GetTurnedTowardDegrees(outOrientationDegrees, goalDegrees, 360.f * deltaSeconds);

	Vec2 const velocity = Vec2::MakeFromPolarDegrees(outOrientationDegrees);
	outPosition += velocity * deltaSeconds * speed;
}

//-----------------------------------------------------------------------------------------------
void EnemyUtils::OrbitPlayer(Vec2&       outPosition,
                             float&      outOrientationDegrees,
                             Vec2 const& playerPosition,
                             float       orbitRadius,
                             float       angularSpeed,
                             float&      orbitAngle,
                             float       deltaSeconds)
{
	orbitAngle += angularSpeed * deltaSeconds;

	// Keep angle in [0, 360) range
	if (orbitAngle >= 360.f)
	{
		orbitAngle -= 360.f;
	}

	Vec2 const orbitOffset = Vec2::MakeFromPolarDegrees(orbitAngle, orbitRadius);
	Vec2 const targetPos   = playerPosition + orbitOffset;

	// Smooth convergence toward orbit target using Engine Interpolate
	if (GetDistanceSquared2D(outPosition, targetPos) > 0.0001f)
	{
		float const fraction = GetClamped(3.f * deltaSeconds, 0.f, 1.f);
		Vec2 const  prevPos  = outPosition;
		outPosition          = Interpolate(outPosition, targetPos, fraction);

		Vec2 const moveDir = (outPosition - prevPos);
		if (moveDir.GetLengthSquared() > 0.0001f)
		{
			outOrientationDegrees = moveDir.GetOrientationDegrees();
		}
	}
}

//-----------------------------------------------------------------------------------------------
void EnemyUtils::ZigZagToward(Vec2&       outPosition,
                              float&      outOrientationDegrees,
                              Vec2 const& playerPosition,
                              float       speed,
                              float       zigzagAmplitude,
                              float&      phase,
                              float       deltaSeconds)
{
	Vec2 const direction = GetDirectionToPlayer(outPosition, playerPosition);

	if (direction == Vec2::ZERO)
	{
		return;
	}

	// Advance phase in degrees (360 deg/sec oscillation rate)
	phase += 360.f * deltaSeconds;
	if (phase >= 360.f)
	{
		phase -= 360.f;
	}

	// Perpendicular direction for zigzag offset
	Vec2 const  perpendicular(-direction.y, direction.x);
	float const sineValue = SinDegrees(phase);

	// Combined movement: forward + perpendicular zigzag
	Vec2 const moveDir = (direction + perpendicular * sineValue * (zigzagAmplitude / speed)).GetNormalized();
	outPosition += moveDir * speed * deltaSeconds;

	outOrientationDegrees = moveDir.GetOrientationDegrees();
}

//-----------------------------------------------------------------------------------------------
bool EnemyUtils::ShouldShootAtPlayer(Vec2 const& enemyPosition,
                                     Vec2 const& playerPosition,
                                     float       range,
                                     float       cooldown,
                                     float&      timer,
                                     float       deltaSeconds)
{
	timer += deltaSeconds;

	if (timer < cooldown)
	{
		return false;
	}

	if (GetDistanceSquared2D(enemyPosition, playerPosition) > range * range)
	{
		return false;
	}

	timer = 0.f;
	return true;
}

//-----------------------------------------------------------------------------------------------
Vec2 EnemyUtils::GetRandomSpawnPosition(Vec2 const& screenDimensions, float margin)
{
	// Pick a random edge (0=top, 1=bottom, 2=left, 3=right) to spawn from periphery
	int const edge = g_rng->RollRandomIntInRange(0, 3);

	float x = 0.0f;
	float y = 0.0f;

	switch (edge)
	{
	case 0: // Top edge
		x = g_rng->RollRandomFloatInRange(margin, screenDimensions.x - margin);
		y = screenDimensions.y - margin;
		break;
	case 1: // Bottom edge
		x = g_rng->RollRandomFloatInRange(margin, screenDimensions.x - margin);
		y = margin;
		break;
	case 2: // Left edge
		x = margin;
		y = g_rng->RollRandomFloatInRange(margin, screenDimensions.y - margin);
		break;
	case 3: // Right edge
		x = screenDimensions.x - margin;
		y = g_rng->RollRandomFloatInRange(margin, screenDimensions.y - margin);
		break;
	default:
		break;
	}

	return Vec2(x, y);
}
