//----------------------------------------------------------------------------------------------------
// Triangle.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Triangle : public Entity
{
public:
    explicit Triangle(EntityID actorID, Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    void     UpdateWindowFocus();
    void     Update(float deltaSeconds) override;
    void     Render() const override;
    void     BounceOfWindow();
    void     UpdateFromInput() override;
};
