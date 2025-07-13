//----------------------------------------------------------------------------------------------------
// Player.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------------------------------
#include "Engine/Core/Timer.hpp"
#include "Game/Gameplay/Entity.hpp"

class Player : public Entity
{
public:
    explicit Player(ActorID actorID,Vec2 const& position, float orientationDegrees);
    void     UpdateWindowFocus();
    void     Update(float deltaSeconds) override;
    void     Render() const override;
    void     UpdateFromInput() override;
    void     FireBullet();
    void     BounceOfWindow() ;

    Timer m_bulletFireTimer;
};
