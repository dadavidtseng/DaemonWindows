//----------------------------------------------------------------------------------------------------
// Wyrm.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

#include <vector>

//-Forward-Declaration--------------------------------------------------------------------------------
class WyrmSegment;

//----------------------------------------------------------------------------------------------------
class Wyrm : public Entity
{
public:
	Wyrm(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Wyrm() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

	// Access the segment chain for split logic
	std::vector<WyrmSegment*> const& GetSegments() const { return m_segments; }

private:
	void SpawnSegments();
	void InitiateSequentialFire();

	// Segments
	std::vector<WyrmSegment*> m_segments;
	static constexpr int      NUM_SEGMENTS = 7;

	// Movement
	float m_moveAngle        = 0.f;     // current movement direction (degrees)
	float m_turnSpeed        = 120.f;   // degrees per second toward player
	float m_headSpeed        = 180.f;   // pixels per second

	// Sequential fire
	float m_fireCooldown     = 3.0f;
	float m_fireTimer        = 0.0f;
};
