//----------------------------------------------------------------------------------------------------
// Triangle.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Triangle.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Triangle::Triangle(EntityID const actorID, Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : Entity(position, orientationDegrees, color)
{
    m_actorID        = actorID;
    m_name           = "Triangle";
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_actorID, m_name);

    Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowByActor(m_actorID));
    Vec2    windowClientPosition  = window->GetClientPosition();
    Vec2    windowClientDimension = window->GetClientDimensions();

    m_healthWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("Health=%d", m_health), windowClientPosition.x, windowClientPosition.y, windowClientDimension.x, windowClientDimension.y, m_color);
    g_theWidgetSubsystem->AddWidget(m_healthWidget, 200);
}

Triangle::~Triangle()
{
    g_theWindowSubsystem->RemoveActorFromMappings(m_actorID);
    m_healthWidget->MarkForDestroy();
}

void Triangle::UpdateWindowFocus()
{
    WindowID    windowID   = g_theWindowSubsystem->FindWindowByActor(m_actorID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);

    if (windowData && windowData->window && windowData->window->GetWindowHandle())
    {
        HWND hwnd = (HWND)windowData->window->GetWindowHandle();

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
    Entity::Update(deltaSeconds);
    BounceOfWindow();
    WindowID    windowID   = g_theWindowSubsystem->FindWindowByActor(m_actorID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    m_healthWidget->SetPosition(windowData->window->GetClientPosition());
    m_healthWidget->SetDimensions(windowData->window->GetClientDimensions());
    m_healthWidget->SetText(Stringf("Health=%d", m_health));
    // UpdateWindowFocus();
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
    WindowID    windowID   = g_theWindowSubsystem->FindWindowByActor(m_actorID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);

    if (windowData == nullptr) return;

    WindowRect rect = windowData->window->lastRect;
    // 取得視窗的邊界
    float windowLeft   = static_cast<float>(rect.left);
    float windowTop    = Window::s_mainWindow->GetScreenDimensions().y - (float)rect.top;
    float windowRight  = static_cast<float>(rect.right);
    float windowBottom = Window::s_mainWindow->GetScreenDimensions().y - static_cast<float>(rect.bottom);
    // DebugAddScreenText(Stringf("Player Window Position(top:%.1f, bottom:%.1f, left:%.1f, right:%.1f)", windowTop, windowBottom, windowLeft, windowRight), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 20.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Position(%.1f, %.1f)", m_position.x, m_position.y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 40.f), 20.f, Vec2::ZERO, 0.f);


    float clampedX = GetClamped(m_position.x,
                                windowLeft + m_cosmeticRadius,   // 左邊界
                                windowRight - m_cosmeticRadius); // 右邊界

    float clampedY = GetClamped(m_position.y,
                                windowBottom + m_cosmeticRadius, // 下邊界（在遊戲座標系中較小）
                                windowTop - m_cosmeticRadius);   // 上邊界（在遊戲座標系中較大）

    // 更新 Player 的位置
    m_position.x = clampedX;
    m_position.y = clampedY;
}

void Triangle::UpdateFromInput()
{
}
