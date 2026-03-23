//----------------------------------------------------------------------------------------------------
// Slimer.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Slime;

//----------------------------------------------------------------------------------------------------
// Slimer (Mid-tier of Slimest boss)
// Chases player, lobs 2 residue blobs, splits into 3 Slimes on death.
//----------------------------------------------------------------------------------------------------
class Slimer : public Entity
{
public:
	Slimer(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Slimer() override;

	void MarkAsDead() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

private:
	void LobResidueBlobs();
	void SpawnSlimes();

	// Attack
	float m_lobCooldown = 3.0f;
	float m_lobTimer    = 0.f;
	int   m_blobCount   = 2;

	// Residue trail
	void  SpawnResidue();
	float m_trailCooldown = 0.5f;
	float m_trailTimer    = 0.f;

	// Split
	int m_splitCount = 3;
};
