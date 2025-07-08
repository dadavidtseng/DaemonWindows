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

Player::Player(ActorID actorID, Vec2 const& position, float const orientationDegrees)
    : Entity(position, orientationDegrees),
      m_bulletFireTimer(0.5f)
{
    m_actorID = actorID;
}

void Player::Update(float deltaSeconds)
{
    UpdateFromInput();
    BounceOfWindow();
}

void Player::Render() const
{
    VertexList_PCU verts2;
    AddVertsForDisc2D(verts2, m_position, 30.f, 10.f, Rgba8::YELLOW);
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

    if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
    {
        FireBullet();
    }

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
    Bullet* bullet     = new Bullet(m_position, 0.f);
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
    WindowID    windowID   = g_theWindowSubsystem->FindWindowByActor(0);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);

    WindowRect rect = windowData->window->lastRect;
    DebugAddScreenText(Stringf("(%ld, %ld, %ld, %ld)", rect.top, rect.bottom, rect.left, rect.right).c_str(), Vec2(500, 500), 20.f, Vec2::ZERO, 0.f);

    // 取得視窗的邊界
    float windowLeft   = static_cast<float>(rect.left);
    float windowTop    = static_cast<float>(rect.top);
    float windowRight  = static_cast<float>(rect.right);
    float windowBottom = static_cast<float>(rect.bottom);

    // Player 的半徑
    float playerRadius = 30.f;

    // 如果 m_position 是絕對螢幕座標，使用這種方式限制
    float clampedX = GetClamped(m_position.x,
                               windowLeft + playerRadius,   // 左邊界
                               windowRight - playerRadius); // 右邊界

    float clampedY = GetClamped(m_position.y,
                               windowTop + playerRadius,    // 上邊界
                               windowBottom - playerRadius);// 下邊界

    // 更新 Player 的位置
    m_position.x = clampedX;
    m_position.y = clampedY;
}
