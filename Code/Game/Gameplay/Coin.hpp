//----------------------------------------------------------------------------------------------------
// Coin.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Coin : public Entity
{
public:
    explicit Coin(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Coin() override;

    void Update(float deltaSeconds) override;
    void Render() const override;

private:
    static bool OnCollisionEnter(EventArgs& args);
    void UpdateFromInput() override;
};
