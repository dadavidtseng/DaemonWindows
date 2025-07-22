//----------------------------------------------------------------------------------------------------
// Debris.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Debris : public Entity
{
public:
    explicit Debris(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Debris() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput() override;
};
