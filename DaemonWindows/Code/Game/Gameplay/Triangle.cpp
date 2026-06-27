//----------------------------------------------------------------------------------------------------
// Triangle.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Triangle.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Gameplay/WaveManager.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"


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
    m_name           = "Triangle";
    m_physicRadius   = 28.f;  // medium chaser
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    // Health scaling: base 3-5, +1 per 3 waves
    int const baseHealth  = g_rng->RollRandomIntInRange(3, 5);
    int       waveBonus   = 0;
    WaveManager* waveMgr  = g_game->GetWaveManager();
    if (waveMgr)
    {
        waveBonus = waveMgr->GetCurrentWaveNumber() / 3;
    }
    m_health = baseHealth + waveBonus;

    // Speed: moderate chase speed (100-150)
    m_speed = g_rng->RollRandomFloatInRange(100.f, 150.f);

    // Coin drop: proportional to health
    m_coinToDrop = m_health;

    if (m_hasChildWindow)
    {
        g_windowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 200, 200);

        Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
        Vec2    windowClientPosition  = window->GetClientPosition();
        Vec2    windowClientDimension = window->GetClientDimensions();

        m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("Health=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        g_widgetSubsystem->AddWidget(m_healthWidget, 200);
    }
}

Triangle::~Triangle()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
        m_healthWidget->MarkForDestroy();
    }
}

void Triangle::UpdateWindowFocus()
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

void Triangle::Update(float const deltaSeconds)
{
    if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
    Entity::Update(deltaSeconds);

    SyncWindowWidgetToPosition();
    if (m_isDead) return;

    // Chase player with smooth movement
    Player* player = g_game->GetPlayer();
    if (player && !player->IsDead())
    {
        Vec2 const previousPosition = m_position;
        EnemyUtils::ChasePlayer(m_position, m_orientationDegrees, player->m_position, m_speed, deltaSeconds);

        // Track velocity for knockback calculations
        if (deltaSeconds > 0.f)
        {
            m_velocity = (m_position - previousPosition) / deltaSeconds;
        }
    }
}

void Triangle::Render() const
{
    VertexList_PCU verts;
    Vec2 const     ccw0 = Vec2(m_position.x, m_position.y + m_physicRadius);
    Vec2 const     ccw1 = Vec2(m_position.x - m_physicRadius, m_position.y - m_physicRadius);
    Vec2 const     ccw2 = Vec2(m_position.x + m_physicRadius, m_position.y - m_physicRadius);
    AddVertsForTriangle2D(verts, ccw0, ccw1, ccw2, m_color);
    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

void Triangle::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}

