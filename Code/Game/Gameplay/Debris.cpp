//----------------------------------------------------------------------------------------------------
// Debris.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Debris.hpp"

//----------------------------------------------------------------------------------------------------
Debris::Debris(EntityID const entityID, Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : Entity(position, orientationDegrees, color)
{
    m_entityID       = entityID;
    m_name           = "Debris";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);
}

//----------------------------------------------------------------------------------------------------
void Debris::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    // m_velocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
    // m_position += m_velocity * deltaSeconds * m_speed;
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
}

//----------------------------------------------------------------------------------------------------
void Debris::Render() const
{
    VertexList_PCU verts;
    AddVertsForAABB2D(verts, AABB2(m_position - Vec2(m_physicRadius, m_physicRadius), m_position + Vec2(m_physicRadius, m_physicRadius)), m_color);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);
}

void Debris::UpdateFromInput()
{
}
