//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Entity.hpp"

#include "Engine/Core/EngineCommon.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Vec2 const&  position,
               float const  orientationDegrees,
               Rgba8 const& color,
               bool const   isVisible,
               bool const   hasChildWindow)
    : m_position(position),
      m_color(color),
      m_orientationDegrees(orientationDegrees),
      m_isVisible(isVisible),
      m_hasChildWindow(hasChildWindow)
{
}

Entity::~Entity()
{
}

void Entity::Update(float const deltaSeconds)
{
    UNUSED(deltaSeconds)
    if (m_health <= 0) MarkAsDead();
    m_isVisible ? g_theWindowSubsystem->ShowWindowByWindowID(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID)) : g_theWindowSubsystem->HideWindowByWindowID(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
}

void Entity::MarkAsDead()
{
    m_isDead = true;
}

void Entity::MarkAsGarbage()
{
    m_isGarbage = true;
}

void Entity::MarkAsInvisible()
{
    m_isVisible = false;
}

void Entity::MarkAsVisible()
{
    m_isVisible = true;
}

bool Entity::IsDead() const
{
    return m_isDead;
}

bool Entity::IsGarbage() const
{
    return m_isGarbage;
}

bool Entity::IsVisible() const
{
    return m_isVisible;
}

void Entity::IncreaseHealth(int const amount)
{
    m_health += amount;
}

void Entity::DecreaseHealth(int const amount)
{
    m_health -= amount;
}
