//----------------------------------------------------------------------------------------------------
// Bullet.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
    Bullet(EntityID const& entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
~Bullet() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput() override;
};
