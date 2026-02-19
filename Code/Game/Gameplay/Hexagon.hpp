//----------------------------------------------------------------------------------------------------
// Hexagon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Hexagon : public Entity
{
public:
    explicit Hexagon(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow, bool canSplit = true);
    ~Hexagon() override;
    void MarkAsDead() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput(float deltaSeconds) override;
    void ShrinkWindow();

private:
    void SpawnSplitHexagons();

    static bool OnCollisionEnter(EventArgs& args);
    std::shared_ptr<ButtonWidget> m_healthWidget;

    // Split behavior
    bool m_canSplit  = true;
    int  m_splitCount = 2;    // number of children to spawn on death
};
