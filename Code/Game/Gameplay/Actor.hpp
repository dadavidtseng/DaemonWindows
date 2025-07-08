//----------------------------------------------------------------------------------------------------
// Actor.hpp
//----------------------------------------------------------------------------------------------------

#pragma once


#include "Game/Framework/GameCommon.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"


class Actor
{
protected:
    ActorID m_id;

public:
    virtual void Render() const = 0;

    Vec2 m_position = Vec2::ZERO;
};
