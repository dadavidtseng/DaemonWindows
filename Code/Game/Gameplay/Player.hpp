//----------------------------------------------------------------------------------------------------
// Player.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"
#include "Game/Gameplay/Entity.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
class Player : public Entity
{
public:
    explicit Player(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Player() override;

    void Update(float deltaSeconds) override;
    void Render() const override;

    void                          UpdateFromInput() override;
    void                          UpdateWindowFocus();
    void                          FireBullet();
    std::shared_ptr<ButtonWidget> m_healthWidget;
    std::shared_ptr<ButtonWidget> m_coinWidget;
    int                           m_maxHealth = 0;
    int                           m_coin      = 50;

private:
    static bool OnGameStateChanged(EventArgs& args);
    static bool OnCollisionEnter(EventArgs& args);
    void        IncreaseCoin(int amount);
    void        DecreaseCoin(int amount);
    void        BounceOfWindow();
    void        ShrinkWindow();

    Timer m_bulletFireTimer;
};