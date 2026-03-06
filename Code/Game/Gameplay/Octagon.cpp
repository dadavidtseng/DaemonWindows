//----------------------------------------------------------------------------------------------------
// Octagon.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Octagon.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Gameplay/WaveManager.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
Octagon::Octagon(EntityID const entityID,
                 Vec2 const&    position,
                 float const    orientationDegrees,
                 Rgba8 const&   color,
                 bool const     isVisible,
                 bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_name           = "Octagon";
    m_physicRadius   = 30.f;
    m_thickness      = 8.f;
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

    // Speed: slower than chasers (design spec: 80)
    m_speed = 80.f;

    // Coin drop: proportional to health
    m_coinToDrop = m_health;

    // Randomize shoot cooldown slightly for variety
    m_shootCooldown = g_rng->RollRandomFloatInRange(1.2f, 1.8f);

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

Octagon::~Octagon()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
        m_healthWidget->MarkForDestroy();
    }
}

void Octagon::Update(float const deltaSeconds)
{
    if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
    Entity::Update(deltaSeconds);

    if (m_hasChildWindow)
    {
        WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
        m_healthWidget->SetPosition(windowData->m_window->GetClientPosition());
        m_healthWidget->SetDimensions(windowData->m_window->GetClientDimensions());
        m_healthWidget->SetText(Stringf("Health=%d", m_health));
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }
    if (m_isDead) return;

    Player* player = g_game->GetPlayer();
    if (!player || player->IsDead()) return;

    // Distance maintenance: keep preferred distance from player
    float const distToPlayer = GetDistance2D(m_position, player->m_position);
    Vec2 const  dirToPlayer  = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);

    if (distToPlayer < m_preferredDist * 0.8f)
    {
        // Too close — retreat away from player
        m_position -= dirToPlayer * m_speed * deltaSeconds;
    }
    else if (distToPlayer > m_shootRange)
    {
        // Too far — approach player
        m_position += dirToPlayer * m_speed * deltaSeconds;
    }

    // Always face the player
    if (dirToPlayer != Vec2::ZERO)
    {
        float const goalDegrees = dirToPlayer.GetOrientationDegrees();
        m_orientationDegrees    = GetTurnedTowardDegrees(m_orientationDegrees, goalDegrees, 180.f * deltaSeconds);
    }

    // Track velocity for knockback
    m_velocity = dirToPlayer * m_speed;

    // Shoot at player when in range and cooldown is ready
    if (EnemyUtils::ShouldShootAtPlayer(m_position, player->m_position, m_shootRange, m_shootCooldown, m_shootTimer, deltaSeconds))
    {
        FireBulletAtPlayer();
    }
}

void Octagon::FireBulletAtPlayer()
{
    Player* player = g_game->GetPlayer();
    if (!player) return;

    Vec2 const direction = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
    if (direction == Vec2::ZERO) return;

    // Spawn bullet at octagon's position, aimed at player
    Bullet* bullet = new Bullet(
        g_rng->RollRandomIntInRange(100, 1000),
        m_position,
        direction.GetOrientationDegrees(),
        m_color,
        true,
        false
    );
    bullet->m_velocity = direction;
    bullet->m_name     = "EnemyBullet";

    g_game->m_entityList.push_back(bullet);
}

void Octagon::Render() const
{
    // Render as an 8-sided polygon
    VertexList_PCU verts;
    constexpr int  NUM_SIDES = 8;

    for (int i = 0; i < NUM_SIDES; ++i)
    {
        float const angle0 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i);
        float const angle1 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i + 1);

        Vec2 const vert0 = m_position + Vec2::MakeFromPolarDegrees(angle0, m_physicRadius);
        Vec2 const vert1 = m_position + Vec2::MakeFromPolarDegrees(angle1, m_physicRadius);

        AddVertsForTriangle2D(verts, m_position, vert0, vert1, m_color);
    }

    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::OPAQUE);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

void Octagon::BounceOfWindow()
{
    Vec2 screenDimensions = Window::s_mainWindow->GetScreenDimensions();

    float clampedX = GetClamped(m_position.x,
                                m_cosmeticRadius,
                                screenDimensions.x - m_cosmeticRadius);

    float clampedY = GetClamped(m_position.y,
                                m_cosmeticRadius,
                                screenDimensions.y - m_cosmeticRadius);

    m_position.x = clampedX;
    m_position.y = clampedY;
}

void Octagon::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}

void Octagon::ShrinkWindow()
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

