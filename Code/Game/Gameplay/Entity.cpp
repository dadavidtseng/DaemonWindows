//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Entity.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : m_position(position),
      m_color(color),
      m_orientationDegrees(orientationDegrees)
{
}

Entity::~Entity()
{
}

void Entity::Update(float const deltaSeconds)
{
    UNUSED(deltaSeconds)
    if (m_health <= 0) MarkAsDead();
}

void Entity::MarkAsDead()
{
    m_isDead = true;
}

void Entity::MarkAsGarbage()
{
    m_isGarbage = true;
}

bool Entity::IsDead() const
{
    return m_isDead;
}

bool Entity::IsGarbage() const
{
    return m_isGarbage;
}

void Entity::DecreaseHealth(int const damage)
{
    m_health -= damage;
}
