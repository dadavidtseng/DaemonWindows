//----------------------------------------------------------------------------------------------------
// Spiker.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Entity.hpp"
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Spiker Boss
// Immobile boss that spawns in its own window. Fires 8-bullet radial waves and a laser
// with dotted-line telegraph. Teleports its window to a new desktop position at HP thresholds.
//----------------------------------------------------------------------------------------------------
class Spiker : public Entity
{
public:
	Spiker(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Spiker() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

private:
	// Attack: 8-bullet radial wave
	void FireRadialBulletWave();
	float m_radialAttackTimer    = 0.0f;
	float m_radialAttackCooldown = 2.0f;

	// Attack: laser with dotted-line telegraph
	void  FireLaser();
	void  RenderLaserTelegraph() const;
	float m_laserTimer           = 0.0f;
	float m_laserCooldown        = 8.0f;
	float m_laserTelegraphTime   = 1.5f;   // dotted line shows this long before firing
	bool  m_isLaserCharging      = false;
	float m_laserChargeTimer     = 0.0f;
	float m_laserAimDegrees      = 0.0f;   // direction laser is aimed

	// Teleport on damage threshold
	void CheckTeleportThreshold();
	int  m_maxHealth             = 48;
	int  m_lastTeleportThreshold = 0;       // tracks which 25% threshold was last triggered
};
