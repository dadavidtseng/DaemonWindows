//----------------------------------------------------------------------------------------------------
// Bullet.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"

#include "Game.hpp"
#include "Player.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//----------------------------------------------------------------------------------------------------
Bullet::Bullet(EntityID const& entityID,
               Vec2 const&     position,
               float const     orientationDegrees,
               Rgba8 const&    color,
               bool const      isVisible,
               bool const      hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID     = entityID;
    m_name         = "Bullet";
    m_physicRadius = 10.f;
    m_speed        = 500.f;
    m_health       = 1;

    g_eventSystem->SubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);

    if (m_hasChildWindow)
    {
        g_windowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 100, 100);
    }
}

Bullet::~Bullet()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
    }
    g_eventSystem->UnsubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);
}

void Bullet::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    UpdateFromInput( deltaSeconds);
    m_position.x += m_velocity.x * deltaSeconds * m_speed;
    m_position.y += m_velocity.y * deltaSeconds * m_speed;

    WindowID windowID = g_windowSubsystem->FindWindowIDByEntityID(g_game->GetPlayer()->m_entityID);
    Window*  window   = g_windowSubsystem->GetWindow(windowID);

    // Check collision with window edges and expand the window accordingly
    {
        Vec2 currentPos  = window->GetWindowPosition();
        Vec2 currentSize = window->GetWindowDimensions();

        if (m_position.x + m_physicRadius * 2.f > currentPos.x + currentSize.x)
        {
            // Right edge: expand width
            Vec2 newPos  = currentPos + Vec2(10, 0);
            Vec2 newSize = currentSize + Vec2(10, 0);
            g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.x - m_physicRadius * 2.f < currentPos.x)
        {
            // Left edge: shift left and expand width
            Vec2 newPos  = currentPos + Vec2(-20, 0);
            Vec2 newSize = currentSize + Vec2(10, 0);
            g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.y + m_physicRadius * 2.f > currentPos.y + currentSize.y)
        {
            // Top edge: shift up and expand height
            Vec2 newPos  = currentPos + Vec2(0, 10);
            Vec2 newSize = currentSize + Vec2(0, 10);
            g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.y - m_physicRadius * 2.f < currentPos.y)
        {
            // Bottom edge: shift down and expand height
            Vec2 newPos  = currentPos + Vec2(0, -20);
            Vec2 newSize = currentSize + Vec2(0, 10);
            g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            m_health -= 1;
        }
    }

    if (m_hasChildWindow)
    {
        WindowID    windowID2   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData2 = g_windowSubsystem->GetWindowData(windowID2);
        windowData2->m_window->SetClientPosition(m_position - windowData2->m_window->GetClientDimensions() * 0.5f);
    }
}

void Bullet::Render() const
{
    VertexList_PCU verts;
    AddVertsForDisc2D(verts, m_position, m_physicRadius, m_color);
    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

void Bullet::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}

STATIC bool Bullet::OnCollisionEnter(EventArgs& args)
{
    EntityID entityAID = args.GetValue("entityAID", -1);
    Entity*  entity    = g_game->GetEntityByEntityID(entityAID);
    entity->DecreaseHealth(1);

    return false;
}
