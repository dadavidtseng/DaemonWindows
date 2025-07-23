//----------------------------------------------------------------------------------------------------
// Shop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Shop.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Shop::Shop(EntityID const entityID,
           Vec2 const&    position,
           float const    orientationDegrees,
           Rgba8 const&   color,
           bool const     isVisible,
           bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_name           = "Shop";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    if (m_hasChildWindow)
    {
        g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);

        Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
        Vec2    windowClientPosition  = window->GetClientPosition();
        Vec2    windowClientDimension = window->GetClientDimensions();

        m_testWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("TEST=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        g_theWidgetSubsystem->AddWidget(m_testWidget, 200);
    }
}

//----------------------------------------------------------------------------------------------------
Shop::~Shop()
{
    if (m_hasChildWindow)
    {
        g_theWindowSubsystem->RemoveEntityFromMappings(m_entityID);
        m_testWidget->MarkForDestroy();
    }
}

//----------------------------------------------------------------------------------------------------
void Shop::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);

    if (m_hasChildWindow)
    {
        WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }
}

//----------------------------------------------------------------------------------------------------
void Shop::Render() const
{
    // if (!m_isVisible) return;

    VertexList_PCU verts;
    Vec2 const     ccw0 = Vec2(m_position.x, m_position.y + m_physicRadius);
    Vec2 const     ccw1 = Vec2(m_position.x - m_physicRadius, m_position.y - m_physicRadius);
    Vec2 const     ccw2 = Vec2(m_position.x + m_physicRadius, m_position.y - m_physicRadius);
    AddVertsForTriangle2D(verts, ccw0, ccw1, ccw2, m_color);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);
}

//----------------------------------------------------------------------------------------------------
void Shop::UpdateFromInput()
{
    // if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
    // {
    //     m_hasChildWindow = !m_hasChildWindow;
    // }
}
