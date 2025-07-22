//----------------------------------------------------------------------------------------------------
// Player.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"
#include "Game/Gameplay/Entity.hpp"

class ButtonWidget;

class Player : public Entity
{
public:
    explicit Player(EntityID actorID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Player() override;
    void UpdateWindowFocus();
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput() override;
    void FireBullet();
    void BounceOfWindow();
    void ShrinkWindow();

private:
    static bool OnGameStateChanged(EventArgs& args);
    void        IncreaseCoin(int amount);
    void        DecreaseCoin(int amount);

    Timer                         m_bulletFireTimer;
    std::shared_ptr<ButtonWidget> m_coinWidget;
    std::shared_ptr<ButtonWidget> m_healthWidget;
    int                           m_coin = 0;
};
