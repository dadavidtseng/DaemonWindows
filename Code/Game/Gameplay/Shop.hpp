//----------------------------------------------------------------------------------------------------
// Shop.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Shop : public Entity
{
public:
    explicit Shop(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    ~Shop()override;
    void     Update(float deltaSeconds) override;
    void     Render() const override;
    void     UpdateFromInput() override;

    std::shared_ptr<ButtonWidget> m_testWidget;
};
