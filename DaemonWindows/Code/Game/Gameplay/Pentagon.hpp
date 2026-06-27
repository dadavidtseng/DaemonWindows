//----------------------------------------------------------------------------------------------------
// Pentagon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Pentagon : public Entity
{
public:
    explicit Pentagon(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Pentagon() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput(float deltaSeconds) override;

private:
    // Zigzag-specific state
    float m_zigzagPhase     = 0.f;
    float m_zigzagAmplitude = 50.f;
};
