//----------------------------------------------------------------------------------------------------
// Triangle.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Triangle.hpp"

#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Triangle::Triangle(EntityID const entityID,
                   Vec2 const&    position,
                   float const    orientationDegrees,
                   Rgba8 const&   color,
                   bool const     isVisible,
                   bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_health         = g_theRNG->RollRandomIntInRange(1, 5);
    m_name           = "Triangle";
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);

    Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
    Vec2    windowClientPosition  = window->GetClientPosition();
    Vec2    windowClientDimension = window->GetClientDimensions();

    m_healthWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("Health=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
    g_theWidgetSubsystem->AddWidget(m_healthWidget, 200);
}

Triangle::~Triangle()
{
    g_theWindowSubsystem->RemoveEntityFromMappings(m_entityID);
    m_healthWidget->MarkForDestroy();
}

void Triangle::UpdateWindowFocus()
{
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);

    if (windowData && windowData->m_window && windowData->m_window->GetWindowHandle())
    {
        HWND hwnd = (HWND)windowData->m_window->GetWindowHandle();

        // 只有在視窗失去焦點時才重新設定
        if (GetForegroundWindow() != hwnd)
        {
            SetForegroundWindow(hwnd);
            SetFocus(hwnd);
        }
    }
}

void Triangle::Update(float const deltaSeconds)
{
    if (g_theGame->GetCurrentGameState() == eGameState::SHOP || g_theGame->GetCurrentGameState() == eGameState::ATTRACT) return;
    Entity::Update(deltaSeconds);

    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    m_healthWidget->SetPosition(windowData->m_window->GetClientPosition());
    m_healthWidget->SetDimensions(windowData->m_window->GetClientDimensions());
    m_healthWidget->SetText(Stringf("Health=%d", m_health));

    if (m_isDead) return;

    // 追蹤玩家的邏輯
    if (g_theGame->m_entities[0] && !g_theGame->m_entities[0]->IsDead())
    {
        Vec2 playerShipPos     = g_theGame->m_entities[0]->m_position;
        Vec2 directionToPlayer = (playerShipPos - m_position).GetNormalized();
        m_orientationDegrees   = directionToPlayer.GetOrientationDegrees();
    }

    m_velocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
    m_position += m_velocity * deltaSeconds * m_speed;

    // 先限制Triangle位置在螢幕邊界內
    // BounceOfWindow();

    // 然後用限制後的位置來設定視窗位置
    windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
}

void Triangle::Render() const
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

void Triangle::BounceOfWindow()
{
    // 使用螢幕邊界，而不是視窗邊界
    Vec2 screenDimensions = Window::s_mainWindow->GetScreenDimensions();

    float screenLeft   = 0.0f;
    float screenBottom = 0.0f;
    float screenTop    = screenDimensions.y;
    float screenRight  = screenDimensions.x;

    // 限制Triangle在螢幕邊界內
    float clampedX = GetClamped(m_position.x,
                                screenLeft + m_cosmeticRadius,   // 左邊界
                                screenRight - m_cosmeticRadius); // 右邊界

    float clampedY = GetClamped(m_position.y,
                                screenBottom + m_cosmeticRadius, // 下邊界
                                screenTop - m_cosmeticRadius);   // 上邊界

    // 更新Triangle的位置
    m_position.x = clampedX;
    m_position.y = clampedY;
}

void Triangle::UpdateFromInput()
{
}

void Triangle::ShrinkWindow()
{
    WindowID windowID = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    Window*  window   = g_theWindowSubsystem->GetWindow(windowID);

    if (!g_theWindowSubsystem->IsWindowAnimating(windowID))
    {
        Vec2 currentPos              = window->GetWindowPosition();
        Vec2 currentSize             = window->GetWindowDimensions();
        Vec2 currentClientDimensions = window->GetClientDimensions();
        if (currentClientDimensions.x <= m_physicRadius * 2.5f || currentClientDimensions.y <= m_physicRadius * 2.5f) return;

        // 右邊界：增加寬度
        Vec2 newPos  = currentPos + Vec2(1, 1);
        Vec2 newSize = currentSize + Vec2(-1, -1);
        g_theWindowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
        // g_theWindowSubsystem->AnimateWindowDimensions(windowID,  newSize, 0.1f);
    }
}
