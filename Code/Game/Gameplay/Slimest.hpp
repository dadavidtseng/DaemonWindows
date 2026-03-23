//----------------------------------------------------------------------------------------------------
// Slimest.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Slimer;

//----------------------------------------------------------------------------------------------------
// Slimest (Main boss - largest tier)
// Chases player slowly, lobs 3 residue blobs, splits into 3 Slimers on death.
//----------------------------------------------------------------------------------------------------
class Slimest : public Entity
{
public:
	Slimest(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Slimest() override;

	void MarkAsDead() override;
	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

private:
	void LobResidueBlobs();
	void SpawnSlimers();

	// Attack
	float m_lobCooldown = 3.5f;
	float m_lobTimer    = 0.f;
	int   m_blobCount   = 3;

	// Split
	int m_splitCount = 3;
};
