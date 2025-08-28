//----------------------------------------------------------------------------------------------------
// Debris.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Debris.hpp"
#include <Engine/Core/EngineCommon.hpp>

//----------------------------------------------------------------------------------------------------
Debris::Debris(EntityID const entityID,
               Vec2 const&    position,
               float const    orientationDegrees,
               Rgba8 const&   color,
               bool const     isVisible,
               bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_name           = "Debris";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_windowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);
}

//----------------------------------------------------------------------------------------------------
Debris::~Debris()
{
    g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
}

//----------------------------------------------------------------------------------------------------
void Debris::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    // m_velocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
    // m_position += m_velocity * deltaSeconds * m_speed;
    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
    windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
}

//----------------------------------------------------------------------------------------------------
void Debris::Render() const
{
    VertexList_PCU verts;
    AddVertsForAABB2D(verts, AABB2(m_position - Vec2(m_physicRadius, m_physicRadius), m_position + Vec2(m_physicRadius, m_physicRadius)), m_color);
    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

void Debris::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}
