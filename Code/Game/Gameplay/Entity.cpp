//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : m_position(position),
      m_orientationDegrees(orientationDegrees),
      m_color(color)
{
}

void Entity::Update(float deltaSeconds)
{
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

void Entity::TakeDamage(int damage)
{
    m_health -= damage;
}
