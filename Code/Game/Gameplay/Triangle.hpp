//----------------------------------------------------------------------------------------------------
// Triangle.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Triangle : public Entity
{
public:
    explicit Triangle(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Triangle() override;
    void UpdateWindowFocus();
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput() override;
    void ShrinkWindow();

private:
    static bool OnCollisionEnter(EventArgs& args);
    std::shared_ptr<ButtonWidget> m_healthWidget;
};
