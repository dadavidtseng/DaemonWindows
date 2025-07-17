//----------------------------------------------------------------------------------------------------
// Player.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Player.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

Player::Player(EntityID const actorID, Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : Entity(position, orientationDegrees, color),
      m_bulletFireTimer(0.5f)
{
    m_actorID        = actorID;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_theWindowSubsystem->CreateChildWindow(m_actorID, "Player Window");
    // m_textWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("%d", m_coin), 200, 200, 300, 300, Rgba8::BLUE);
    // g_theWidgetSubsystem->AddWidget(m_textWidget, 100);
}

void Player::UpdateWindowFocus()
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

void Player::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    UpdateFromInput();
    BounceOfWindow();
    // UpdateWindowFocus();
    WindowID    windowID   = g_theWindowSubsystem->FindWindowByActor(m_actorID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    WindowRect  rect       = windowData->window->lastRect;
    DebugAddScreenText(Stringf("Player Window Position(top:%ld, bottom:%ld, left:%ld, right:%ld)", rect.top, rect.bottom, rect.left, rect.right), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 20.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Window Dimensions(width:%.1f, height:%.1f)", windowData->window->GetWindowDimensions().x, windowData->window->GetWindowDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 40.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Client Dimensions(width:%.1f, height:%.1f)", windowData->window->GetClientDimensions().x, windowData->window->GetClientDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 60.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Position(%.1f, %.1f)", m_position.x, m_position.y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 80.f), 20.f, Vec2::ZERO, 0.f);
    // m_textWidget->SetPosition(g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowByActor(m_actorID))->GetClientPosition());
}

void Player::Render() const
{
    VertexList_PCU verts2;
    AddVertsForDisc2D(verts2, m_position, m_physicRadius, m_thickness, m_color);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts2);
}

void Player::UpdateFromInput()
{
    if (g_theInput->IsKeyDown(KEYCODE_W)) m_position.y += 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_A)) m_position.x -= 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_S)) m_position.y -= 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_D)) m_position.x += 10.f;

    if (g_theInput->WasKeyJustPressed(KEYCODE_P))
    {
        m_coin += 1;
        m_textWidget->SetText(Stringf("%d", m_coin));
    }

    // if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
    // {
    //     FireBullet();
    // }

    // 連發射擊（持續按住）
    if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE))
    {
        // 檢查計時器是否已經停止（第一次射擊）或時間已到
        if (m_bulletFireTimer.IsStopped() || m_bulletFireTimer.HasPeriodElapsed())
        {
            FireBullet();

            // 重新啟動計時器
            m_bulletFireTimer.Start();

            // 如果計時器期間已過，減去一個週期
            if (m_bulletFireTimer.HasPeriodElapsed())
            {
                m_bulletFireTimer.DecrementPeriodIfElapsed();
            }
        }
    }
    else
    {
        // 當滑鼠鬆開時停止計時器
        m_bulletFireTimer.Stop();
    }
}

void Player::FireBullet()
{
    Bullet* bullet     = new Bullet(m_position, 0.f, Rgba8::GREEN);
    int     id         = g_theRNG->RollRandomIntInRange(100, 1000);
    bullet->m_actorID  = id;
    bullet->m_windowID = 10;
    bullet->m_name     = "BULLET";

    Vec2 velocity      = (Window::s_mainWindow->GetCursorPositionOnScreen() - m_position).GetNormalized();
    bullet->m_velocity = velocity;

    // g_theWindowSubsystem->CreateChildWindow(bullet->m_actorID, bullet->m_name);
    g_theGame->m_entities.push_back(bullet);
}

void Player::BounceOfWindow()
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
