//----------------------------------------------------------------------------------------------------
// Player.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
#include "Engine/Core/Timer.hpp"
#include "Game/Gameplay/Entity.hpp"

class ButtonWidget;

class Player : public Entity
{
public:
    explicit Player(EntityID actorID, Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    void     UpdateWindowFocus();
    void     Update(float deltaSeconds) override;
    void     Render() const override;
    void     UpdateFromInput() override;
    void     FireBullet();
    void     BounceOfWindow();

    Timer m_bulletFireTimer;
    std::shared_ptr<ButtonWidget> m_textWidget;
    int   m_coin = 0;
};
