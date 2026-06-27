//----------------------------------------------------------------------------------------------------
// Residue.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
// Residue - Stationary damaging puddle left behind by Slimest/Slimer/Slime as they move.
// Damages the player on contact with a cooldown. Fades and self-destructs after a lifetime.
//----------------------------------------------------------------------------------------------------
class Residue : public Entity
{
public:
	Residue(EntityID entityID, Vec2 const& position, Rgba8 const& color);
	~Residue() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;

	bool CanDamage() const;
	void ResetDamageCooldown();

private:
	float m_lifetime        = 4.f;    // seconds before self-destruct
	float m_age             = 0.f;
	float m_damageCooldown  = 1.0f;   // seconds between damage ticks
	float m_damageTimer     = 0.f;
};
