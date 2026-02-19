//----------------------------------------------------------------------------------------------------
// Pentagon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Pentagon : public Entity
{
public:
    explicit Pentagon(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Pentagon() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput(float deltaSeconds) override;
    void ShrinkWindow();

private:
    static bool OnCollisionEnter(EventArgs& args);
    std::shared_ptr<ButtonWidget> m_healthWidget;

    // Zigzag-specific state
    float m_zigzagPhase     = 0.f;
    float m_zigzagAmplitude = 50.f;
};
