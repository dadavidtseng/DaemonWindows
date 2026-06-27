//----------------------------------------------------------------------------------------------------
// Hexagon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Hexagon : public Entity
{
public:
    explicit Hexagon(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow, bool canSplit = true);
    ~Hexagon() override;
    void MarkAsDead() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput(float deltaSeconds) override;

private:
    void SpawnSplitHexagons();

    // Split behavior
    bool m_canSplit  = true;
    int  m_splitCount = 2;    // number of children to spawn on death
};
