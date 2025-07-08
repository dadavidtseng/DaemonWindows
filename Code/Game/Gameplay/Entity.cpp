//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Vec2 const& position, float const orientationDegrees)
    : m_position(position),
      m_orientationDegrees(orientationDegrees)
{
}
