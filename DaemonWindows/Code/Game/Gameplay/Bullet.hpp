//----------------------------------------------------------------------------------------------------
// Bullet.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
    Bullet(EntityID const& entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Bullet() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput(float deltaSeconds) override;

    bool HasAlreadyHit(EntityID enemyID) const;
    void RegisterHit(EntityID enemyID);
    bool CanPierce() const;

    int   m_piercingCount   = 0;    // Number of additional enemies this bullet can pierce through (0 = dies on first hit)
    float m_homingStrength  = 0.f;  // Homing turn rate in degrees per second (0 = no homing)

private:
    std::vector<EntityID> m_hitEnemyIDs; // Track enemies already hit to prevent double-hits
};
