//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Entity.hpp"

#include "Game.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Vec2 const&  position,
               float const  orientationDegrees,
               Rgba8 const& color,
               bool const   isVisible,
               bool const   hasChildWindow)
    : m_position(position),
      m_color(color),
      m_orientationDegrees(orientationDegrees),
      m_isChildWindowVisible(isVisible),
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
    m_isChildWindowVisible ? g_theWindowSubsystem->ShowWindowByWindowID(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID)) : g_theWindowSubsystem->HideWindowByWindowID(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
}

void Entity::MarkAsDead()
{
    m_isDead = true;

    if (m_name == "Bullet") return;

    if (g_theGame->GetCurrentGameState() == eGameState::GAME)
    {
        EventArgs args;
        args.SetValue("name", m_name);
        args.SetValue("entityID", std::to_string(m_entityID));
        g_theEventSystem->FireEvent("OnEntityDestroyed", args);
    }
}

void Entity::MarkAsGarbage()
{
    m_isGarbage = true;
}

void Entity::MarkAsChildWindowInvisible()
{
    m_isChildWindowVisible = false;
}

void Entity::MarkAsChildWindowVisible()
{
    m_isChildWindowVisible = true;
}

void Entity::MarkAsEntityInvisible()
{
    m_isEntityVisible = false;
}

void Entity::MarkAsEntityVisible()
{
    m_isEntityVisible = true;
}

bool Entity::IsDead() const
{
    return m_isDead;
}

bool Entity::IsGarbage() const
{
    return m_isGarbage;
}

bool Entity::IsChildWindowVisible() const
{
    return m_isChildWindowVisible;
}

bool Entity::IsEntityVisible() const
{
    return m_isEntityVisible;
}

void Entity::IncreaseHealth(int const amount)
{
    m_health += amount;
}

void Entity::DecreaseHealth(int const amount)
{
    m_health -= amount;
}
