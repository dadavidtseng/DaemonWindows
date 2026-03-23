//----------------------------------------------------------------------------------------------------
// WyrmSegment.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class WyrmSegment : public Entity
{
public:
	WyrmSegment(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow, int segmentIndex);
	~WyrmSegment() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void UpdateFromInput(float deltaSeconds) override;
	void DecreaseHealth(int amount) override;

	// Chain management
	void         SetLeader(Entity* leader) { m_leader = leader; }
	Entity*      GetLeader() const { return m_leader; }
	void         SetFollower(WyrmSegment* follower) { m_follower = follower; }
	WyrmSegment* GetFollower() const { return m_follower; }
	int          GetSegmentIndex() const { return m_segmentIndex; }
	void         SetSegmentIndex(int index) { m_segmentIndex = index; }

	// Sequential fire
	void         TriggerFire();

private:
	void FireBulletAtPlayer();

	Entity*      m_leader        = nullptr;   // predecessor in chain (Wyrm head or another WyrmSegment)
	WyrmSegment* m_follower      = nullptr;   // next segment in chain
	int          m_segmentIndex  = 0;         // 0-based index in the chain

	// Sequential fire timing
	float m_fireDelay        = 0.f;   // delay before this segment fires after being triggered
	float m_fireDelayTimer   = 0.f;
	bool  m_fireTriggered    = false;

	// Follow distance
	float m_followDistance    = 65.f;  // distance to maintain from leader
};
