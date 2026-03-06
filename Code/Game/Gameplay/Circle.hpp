//----------------------------------------------------------------------------------------------------
// Circle.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Circle : public Entity
{
public:
    explicit Circle(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Circle() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput(float deltaSeconds) override;
    void ShrinkWindow();

private:
    std::shared_ptr<ButtonWidget> m_healthWidget;

    // Orbit-specific state
    float m_orbitAngle       = 0.f;
    float m_orbitRadius      = 200.f;
    float m_orbitAngularSpeed = 90.f;   // degrees per second
};
