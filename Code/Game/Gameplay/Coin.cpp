//----------------------------------------------------------------------------------------------------
// Coin.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Coin.hpp"

//----------------------------------------------------------------------------------------------------
Coin::Coin(EntityID const entityID,
           Vec2 const&    position,
           float const    orientationDegrees,
           Rgba8 const&   color,
           bool const     isVisible,
           bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_name           = "Coin";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);
}

//----------------------------------------------------------------------------------------------------
Coin::~Coin()
{
    g_theWindowSubsystem->RemoveEntityFromMappings(m_entityID);
}

//----------------------------------------------------------------------------------------------------
void Coin::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
}

//----------------------------------------------------------------------------------------------------
void Coin::Render() const
{
    VertexList_PCU verts2;
    Vec2 const     ccw0 = Vec2(m_position.x, m_position.y + m_physicRadius);
    Vec2 const     ccw1 = Vec2(m_position.x - m_physicRadius, m_position.y - m_physicRadius);
    Vec2 const     ccw2 = Vec2(m_position.x + m_physicRadius, m_position.y - m_physicRadius);
    AddVertsForTriangle2D(verts2, ccw0, ccw1, ccw2, m_color);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts2);
}

void Coin::UpdateFromInput()
{
}
