//----------------------------------------------------------------------------------------------------
// Slime.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
// Slime (Smallest tier of Slimest boss)
// Chases player, lobs 1 residue blob, dies normally and drops coins.
//----------------------------------------------------------------------------------------------------
class Slime : public Entity
{
public:
	Slime(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Slime() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

private:
	void LobResidueBlobs();

	// Attack
	float m_lobCooldown = 2.5f;
	float m_lobTimer    = 0.f;
	int   m_blobCount   = 1;
};
