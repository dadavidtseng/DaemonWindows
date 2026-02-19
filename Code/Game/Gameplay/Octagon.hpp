//----------------------------------------------------------------------------------------------------
// Octagon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Octagon : public Entity
{
public:
    explicit Octagon(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Octagon() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void BounceOfWindow();
    void UpdateFromInput(float deltaSeconds) override;
    void ShrinkWindow();

private:
    void FireBulletAtPlayer();

    static bool OnCollisionEnter(EventArgs& args);
    std::shared_ptr<ButtonWidget> m_healthWidget;

    // Ranged combat state
    float m_shootRange    = 300.f;
    float m_shootCooldown = 1.5f;    // seconds between shots
    float m_shootTimer    = 0.f;
    float m_preferredDist = 200.f;   // ideal distance from player
};
