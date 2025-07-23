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

//----------------------------------------------------------------------------------------------------
Player::Player(EntityID const entityID,
               Vec2 const&    position,
               float const    orientationDegrees,
               Rgba8 const&   color,
               bool const     isVisible,
               bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow),
      m_bulletFireTimer(0.5f)
{
    m_entityID       = entityID;
    m_health         = 10;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;
    m_name           = "You";

    g_theEventSystem->SubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_theEventSystem->SubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);

    g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, 100, 100, (int)(1445 * 0.6f), (int)(248));

    Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
    Vec2    windowClientPosition  = window->GetClientPosition();
    Vec2    windowClientDimension = window->GetClientDimensions();

    m_coinWidget   = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("Coin=%d", m_coin), windowClientPosition.x, windowClientPosition.y, windowClientDimension.x, windowClientDimension.y, m_color);
    m_healthWidget = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("Health=%d", m_health), windowClientPosition.x, windowClientPosition.y, windowClientDimension.x, windowClientDimension.y, m_color);

    g_theWidgetSubsystem->AddWidget(m_coinWidget, 100);
    g_theWidgetSubsystem->AddWidget(m_healthWidget, 200);
    m_coinWidget->SetVisible(false);
    m_healthWidget->SetVisible(false);
}

//----------------------------------------------------------------------------------------------------
Player::~Player()
{
    Entity::~Entity();
    g_theEventSystem->UnsubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_theEventSystem->UnsubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);
}

//----------------------------------------------------------------------------------------------------
void Player::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);

    if (g_theGame->GetCurrentGameState() == eGameState::GAME)
    {
        UpdateFromInput();
        BounceOfWindow();
        ShrinkWindow();
    }

    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    WindowRect  rect       = windowData->m_window->lastRect;
    DebugAddScreenText(Stringf("Player Window Position(top:%ld, bottom:%ld, left:%ld, right:%ld)", rect.top, rect.bottom, rect.left, rect.right), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 20.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Window Dimensions(width:%.1f, height:%.1f)", windowData->m_window->GetWindowDimensions().x, windowData->m_window->GetWindowDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 40.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Client Dimensions(width:%.1f, height:%.1f)", windowData->m_window->GetClientDimensions().x, windowData->m_window->GetClientDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 60.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Window Position(width:%.1f, height:%.1f)", windowData->m_window->GetWindowPosition().x, windowData->m_window->GetWindowPosition().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 80.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Client Position(width:%.1f, height:%.1f)", windowData->m_window->GetClientPosition().x, windowData->m_window->GetClientPosition().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 100.f), 20.f, Vec2::ZERO, 0.f);
    DebugAddScreenText(Stringf("Player Position(%.1f, %.1f)", m_position.x, m_position.y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 120.f), 20.f, Vec2::ZERO, 0.f);

    m_coinWidget->SetPosition(windowData->m_window->GetClientPosition());
    m_coinWidget->SetDimensions(windowData->m_window->GetClientDimensions());
    m_healthWidget->SetPosition(windowData->m_window->GetClientPosition() + Vec2(0, 20));
    m_healthWidget->SetDimensions(windowData->m_window->GetClientDimensions());

    if (g_theGame->GetCurrentGameState() == eGameState::ATTRACT)
    {
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }
}

//----------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------
void Player::UpdateFromInput()
{
    if (g_theInput->IsKeyDown(KEYCODE_W)) m_position.y += 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_A)) m_position.x -= 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_S)) m_position.y -= 10.f;
    if (g_theInput->IsKeyDown(KEYCODE_D)) m_position.x += 10.f;

    // 連發射擊（持續按住）
    if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE))
    {
        // 檢查計時器是否已經停止（第一次射擊）或時間已到
        if (m_bulletFireTimer.IsStopped() || m_bulletFireTimer.HasPeriodElapsed())
        {
            FireBullet();
            // m_isFiringBullet = true;
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

    if (g_theInput->WasKeyJustReleased(KEYCODE_LEFT_MOUSE))
    {
        // m_isFiringBullet = false;
    }
}

//----------------------------------------------------------------------------------------------------
void Player::UpdateWindowFocus()
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

//----------------------------------------------------------------------------------------------------
void Player::FireBullet()
{
    Bullet* bullet = new Bullet(g_theRNG->RollRandomIntInRange(100, 1000), m_position, 0.f, Rgba8::WHITE, true, false);

    Vec2 velocity      = (Window::s_mainWindow->GetCursorPositionOnScreen() - m_position).GetNormalized();
    bullet->m_velocity = velocity;

    // g_theWindowSubsystem->CreateChildWindow(bullet->m_actorID, bullet->m_name);
    g_theGame->m_entities.push_back(bullet);
}

void Player::BounceOfWindow()
{
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);

    if (windowData == nullptr) return;

    // 取得視窗的邊界
    float windowLeft   = windowData->m_window->GetClientPosition().x;
    float windowBottom = windowData->m_window->GetClientPosition().y;
    float windowTop    = windowData->m_window->GetClientPosition().y + windowData->m_window->GetClientDimensions().y;
    float windowRight  = windowData->m_window->GetClientPosition().x + windowData->m_window->GetClientDimensions().x;


    float clampedX = GetClamped(m_position.x,
                                windowLeft + m_physicRadius,   // 左邊界
                                windowRight - m_physicRadius); // 右邊界

    float clampedY = GetClamped(m_position.y,
                                windowBottom + m_physicRadius, // 下邊界（在遊戲座標系中較小）
                                windowTop - m_physicRadius);   // 上邊界（在遊戲座標系中較大）

    // 更新 Player 的位置
    m_position.x = clampedX;
    m_position.y = clampedY;
}

void Player::ShrinkWindow()
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
    }
}

bool Player::OnGameStateChanged(EventArgs& args)
{
    String const preGameState = args.GetValue("preGameState", "DEFAULT");
    String const curGameState = args.GetValue("curGameState", "DEFAULT");
    if (preGameState == "ATTRACT" && curGameState == "GAME")
    {
        g_theGame->GetPlayer()->m_coinWidget->SetVisible(true);
        g_theGame->GetPlayer()->m_healthWidget->SetVisible(true);
    }
    else if (preGameState == "GAME" && curGameState == "ATTRACT")
    {
        g_theGame->GetPlayer()->m_coinWidget->SetVisible(false);
        g_theGame->GetPlayer()->m_healthWidget->SetVisible(false);
    }

    return false;
}

STATIC bool Player::OnCollisionEnter(EventArgs& args)
{
    String entityA = args.GetValue("entityA", "DEFAULT");
    String entityB = args.GetValue("entityB", "DEFAULT");

    if (entityA != "You" && entityB != "You") return false;

    Player* player = g_theGame->GetPlayer();
    player->IncreaseCoin(1);
    player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));

    return false;
}

void Player::IncreaseCoin(int const amount)
{
    m_coin += amount;
}

void Player::DecreaseCoin(int const amount)
{
    m_coin -= amount;
}
