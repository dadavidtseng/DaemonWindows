//----------------------------------------------------------------------------------------------------
// Triangle.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once


#include "Game/Subsystem/Window/WindowSubsystem.hpp"

class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Triangle : public Entity
{
public:
    explicit                  Triangle(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    ~Triangle() override;
    void UpdateWindowFocus();
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput() override;
    void ShrinkWindow();

    std::shared_ptr<ButtonWidget> m_healthWidget;
};
