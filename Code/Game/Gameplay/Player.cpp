//----------------------------------------------------------------------------------------------------
// Player.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Player.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"

//----------------------------------------------------------------------------------------------------
Player::Player(EntityID const entityID,
               Vec2 const&    position,
               float const    orientationDegrees,
               Rgba8 const&   color,
               bool const     isVisible,
               bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow),
      m_bulletFireTimer(0.3f)
{
    m_entityID       = entityID;
    m_health         = 10;
    m_maxHealth      = 10;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;
    m_name           = "You";
    // m_speed          = 5.f;

    g_eventSystem->SubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_eventSystem->SubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);

    g_windowSubsystem->CreateChildWindow(m_entityID, m_name, 100, 100, (int)(1445 * 0.6f), (int)(248));

    Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
    Vec2    windowClientPosition  = window->GetClientPosition();
    Vec2    windowClientDimension = window->GetClientDimensions();

    m_coinWidget   = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("Coin=%d", m_coin), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
    m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("Health=%d/%d", m_health, m_maxHealth), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);


    g_widgetSubsystem->AddWidget(m_coinWidget, 100);
    g_widgetSubsystem->AddWidget(m_healthWidget, 200);

    m_coinWidget->SetVisible(false);
    m_healthWidget->SetVisible(false);
}

//----------------------------------------------------------------------------------------------------
Player::~Player()
{
    Entity::~Entity();
    g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnCollisionEnter", OnCollisionEnter);
    m_coinWidget->MarkForDestroy();
    m_healthWidget->MarkForDestroy();

    // TODO: this should be replaced to end game scene
    g_game->ChangeGameState(eGameState::ATTRACT);
}

//----------------------------------------------------------------------------------------------------
void Player::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);

    if (g_game->GetCurrentGameState() != eGameState::ATTRACT)
    {
        UpdateFromInput(deltaSeconds);
        BounceOfWindow();
        ShrinkWindow();
    }

    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
    if (windowData == nullptr) return;
    // WindowRect  rect       = windowData->m_window->lastRect;
    // DebugAddScreenText(Stringf("Player Window Position(top:%ld, bottom:%ld, left:%ld, right:%ld)", rect.top, rect.bottom, rect.left, rect.right), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 20.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Window Dimensions(width:%.1f, height:%.1f)", windowData->m_window->GetWindowDimensions().x, windowData->m_window->GetWindowDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 40.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Client Dimensions(width:%.1f, height:%.1f)", windowData->m_window->GetClientDimensions().x, windowData->m_window->GetClientDimensions().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 60.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Window Position(width:%.1f, height:%.1f)", windowData->m_window->GetWindowPosition().x, windowData->m_window->GetWindowPosition().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 80.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Client Position(width:%.1f, height:%.1f)", windowData->m_window->GetClientPosition().x, windowData->m_window->GetClientPosition().y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 100.f), 20.f, Vec2::ZERO, 0.f);
    // DebugAddScreenText(Stringf("Player Position(%.1f, %.1f)", m_position.x, m_position.y), Vec2(0.f, Window::s_mainWindow->GetScreenDimensions().y - 120.f), 20.f, Vec2::ZERO, 0.f);

    m_coinWidget->SetPosition(windowData->m_window->GetClientPosition());
    m_coinWidget->SetDimensions(windowData->m_window->GetClientDimensions());
    m_healthWidget->SetPosition(windowData->m_window->GetClientPosition() + Vec2(0, 20));
    m_healthWidget->SetDimensions(windowData->m_window->GetClientDimensions());


    if (g_game->GetCurrentGameState() == eGameState::ATTRACT)
    {
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }
}

//----------------------------------------------------------------------------------------------------
void Player::Render() const
{
    VertexList_PCU verts2;
    AddVertsForDisc2D(verts2, m_position, m_physicRadius, m_thickness, m_color);
    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts2);
}

//----------------------------------------------------------------------------------------------------
void Player::UpdateFromInput(float const deltaSeconds)
{
    if (g_input->IsKeyDown(KEYCODE_W)) m_position.y += deltaSeconds * m_speed;
    if (g_input->IsKeyDown(KEYCODE_A)) m_position.x -= deltaSeconds * m_speed;
    if (g_input->IsKeyDown(KEYCODE_S)) m_position.y -= deltaSeconds * m_speed;
    if (g_input->IsKeyDown(KEYCODE_D)) m_position.x += deltaSeconds * m_speed;

    // Continuous fire (hold to shoot)
    if (g_input->IsKeyDown(KEYCODE_LEFT_MOUSE))
    {
        // Check if timer is stopped (first shot) or period has elapsed
        if (m_bulletFireTimer.IsStopped() || m_bulletFireTimer.HasPeriodElapsed())
        {
            FireBullet();

            // Restart the fire timer
            m_bulletFireTimer.Start();

            // If the timer period has elapsed, decrement one period
            if (m_bulletFireTimer.HasPeriodElapsed())
            {
                m_bulletFireTimer.DecrementPeriodIfElapsed();
            }
        }
    }
    else
    {
        // Stop the fire timer when mouse is released
        m_bulletFireTimer.Stop();
    }

    if (g_input->WasKeyJustReleased(KEYCODE_LEFT_MOUSE))
    {
    }
}

//----------------------------------------------------------------------------------------------------
void Player::UpdateWindowFocus()
{
    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);

    if (windowData && windowData->m_window && windowData->m_window->GetWindowHandle())
    {
        HWND hwnd = static_cast<HWND>(windowData->m_window->GetWindowHandle());

        // Only reset focus when the window has lost it
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
    Bullet* bullet = new Bullet(g_rng->RollRandomIntInRange(100, 1000), m_position, 0.f, Rgba8::WHITE, true, false);

    Vec2 velocity      = (Window::s_mainWindow->GetCursorPositionOnScreen() - m_position).GetNormalized();
    bullet->m_velocity = velocity;

    g_game->m_entityList.push_back(bullet);
    SoundID const attractBGM = g_audio->CreateOrGetSound("Data/Audio/shoot.mp3", eAudioSystemSoundDimension::Sound2D);
    g_audio->StartSound(attractBGM, false, 1.f, 0.f, 1.f);
}

void Player::BounceOfWindow()
{
    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);

    if (windowData == nullptr) return;

    // Get window bounds
    float windowLeft   = windowData->m_window->GetClientPosition().x;
    float windowBottom = windowData->m_window->GetClientPosition().y;
    float windowTop    = windowData->m_window->GetClientPosition().y + windowData->m_window->GetClientDimensions().y;
    float windowRight  = windowData->m_window->GetClientPosition().x + windowData->m_window->GetClientDimensions().x;


    float clampedX = GetClamped(m_position.x,
                                windowLeft + m_physicRadius,
                                windowRight - m_physicRadius);

    float clampedY = GetClamped(m_position.y,
                                windowBottom + m_physicRadius,
                                windowTop - m_physicRadius);

    // Update player position
    m_position.x = clampedX;
    m_position.y = clampedY;
}

void Player::ShrinkWindow()
{
    WindowID windowID = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    Window*  window   = g_windowSubsystem->GetWindow(windowID);

    if (!g_windowSubsystem->IsWindowAnimating(windowID))
    {
        Vec2 currentPos              = window->GetWindowPosition();
        Vec2 currentSize             = window->GetWindowDimensions();
        Vec2 currentClientDimensions = window->GetClientDimensions();
        if (currentClientDimensions.x <= m_physicRadius * 2.5f || currentClientDimensions.y <= m_physicRadius * 2.5f) return;

        Vec2 newPos  = currentPos + Vec2(1, 1);
        Vec2 newSize = currentSize + Vec2(-1, -1);
        g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
    }
}

bool Player::OnGameStateChanged(EventArgs& args)
{
    String const preGameState = args.GetValue("preGameState", "DEFAULT");
    String const curGameState = args.GetValue("curGameState", "DEFAULT");
    if (preGameState == "ATTRACT" && curGameState == "GAME")
    {
        g_game->GetPlayer()->m_coinWidget->SetVisible(true);
        g_game->GetPlayer()->m_healthWidget->SetVisible(true);
    }
    else if (preGameState == "GAME" && curGameState == "ATTRACT")
    {
        g_game->GetPlayer()->m_coinWidget->SetVisible(false);
        g_game->GetPlayer()->m_healthWidget->SetVisible(false);
        WindowID windowID = g_windowSubsystem->FindWindowIDByEntityID(g_game->GetPlayer()->m_entityID);
        Window*  window   = g_windowSubsystem->GetWindow(windowID);
        window->SetClientDimensions(Vec2((int)(1445 * 0.6f), (int)(248)));
    }

    return false;
}

STATIC bool Player::OnCollisionEnter(EventArgs& args)
{
    String   entityA   = args.GetValue("entityA", "DEFAULT");
    String   entityB   = args.GetValue("entityB", "DEFAULT");
    EntityID entityBID = args.GetValue("entityBID", -1);
    Player*  player    = g_game->GetPlayer();
    Entity*  entity    = g_game->GetEntityByEntityID(entityBID);
    if (entityA == "You" && entityB == "Coin")
    {
        player->IncreaseCoin(1);
        player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));
    }
    else if (entityA == "You" && entityB == "Triangle")
    {
        player->DecreaseHealth(1);
        player->m_healthWidget->SetText(Stringf("Health=%d/%d", player->m_health, player->m_maxHealth));
        player->m_position += (player->m_position - entity->m_position);
    }

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
