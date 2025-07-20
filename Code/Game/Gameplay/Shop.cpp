//----------------------------------------------------------------------------------------------------
// Shop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Shop.hpp"

#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

Shop::Shop(EntityID const entityID, Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : Entity(position, orientationDegrees, color)
{
    m_entityID       = entityID;
    m_name           = "Shop";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);

    Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
    Vec2    windowClientPosition  = window->GetClientPosition();
    Vec2    windowClientDimension = window->GetClientDimensions();

    m_testWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("TEST=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
    g_theWidgetSubsystem->AddWidget(m_testWidget, 200);
}

//----------------------------------------------------------------------------------------------------
Shop::~Shop()
{
    g_theWindowSubsystem->RemoveEntityFromMappings(m_entityID);
    m_testWidget->MarkForDestroy();
}

void Shop::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
}

void Shop::Render() const
{
}

void Shop::UpdateFromInput()
{
}
