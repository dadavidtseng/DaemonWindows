//----------------------------------------------------------------------------------------------------
// Hexagon.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Hexagon.hpp"
//----------------------------------------------------------------------------------------------------
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
Hexagon::Hexagon(EntityID const entityID,
                 Vec2 const&    position,
                 float const    orientationDegrees,
                 Rgba8 const&   color,
                 bool const     isVisible,
                 bool const     hasChildWindow,
                 bool           canSplit)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
    , m_canSplit(canSplit)
{
    m_entityID       = entityID;
    m_name           = "Hexagon";
    m_thickness      = 8.f;

    if (m_canSplit)
    {
        // Large hexagon: full stats
        m_physicRadius = 30.f;

        int const baseHealth  = g_rng->RollRandomIntInRange(4, 6);
        int       waveBonus   = 0;
        WaveManager* waveMgr  = g_game->GetWaveManager();
        if (waveMgr)
        {
            waveBonus = waveMgr->GetCurrentWaveNumber() / 3;
        }
        m_health     = baseHealth + waveBonus;
        m_speed      = 100.f;
        m_splitCount = g_rng->RollRandomIntInRange(2, 3);
        m_coinToDrop = m_health;
    }
    else
    {
        // Small hexagon: reduced stats, no splitting
        m_physicRadius = 18.f;
        m_health       = g_rng->RollRandomIntInRange(1, 2);
        m_speed        = 130.f;
        m_coinToDrop   = 1;
    }

    m_cosmeticRadius = m_physicRadius + m_thickness;

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

Hexagon::~Hexagon()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
        m_healthWidget->MarkForDestroy();
    }
}

void Hexagon::MarkAsDead()
{
    if (!m_isDead && m_canSplit)
    {
        SpawnSplitHexagons();
    }

    Entity::MarkAsDead();
}

void Hexagon::SpawnSplitHexagons()
{
    for (int i = 0; i < m_splitCount; ++i)
    {
        // Scatter children around the death position
        float const scatterAngle = 360.f / static_cast<float>(m_splitCount) * static_cast<float>(i);
        Vec2 const  offset       = Vec2::MakeFromPolarDegrees(scatterAngle, m_physicRadius * 1.5f);
        Vec2 const  spawnPos     = m_position + offset;

        int const randomType = g_rng->RollRandomIntInRange(0, 1);

        Hexagon* child = new Hexagon(
            g_rng->RollRandomIntInRange(100, 1000),
            spawnPos,
            0.f,
            Rgba8::YELLOW,
            true,
            randomType,
            false    // small hexagons cannot split
        );

        g_game->m_entityList.push_back(child);
    }
}

void Hexagon::Update(float const deltaSeconds)
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

    // Chase player
    Player* player = g_game->GetPlayer();
    if (player && !player->IsDead())
    {
        Vec2 const previousPosition = m_position;
        EnemyUtils::ChasePlayer(m_position, m_orientationDegrees, player->m_position, m_speed, deltaSeconds);

        if (deltaSeconds > 0.f)
        {
            m_velocity = (m_position - previousPosition) / deltaSeconds;
        }
    }
}

void Hexagon::Render() const
{
    // Render as a 6-sided polygon
    VertexList_PCU verts;
    constexpr int  NUM_SIDES = 6;

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

void Hexagon::BounceOfWindow()
{
    Vec2 screenDimensions = Window::s_mainWindow->GetScreenDimensions();

    m_position.x = GetClamped(m_position.x,
                              m_cosmeticRadius,
                              screenDimensions.x - m_cosmeticRadius);

    m_position.y = GetClamped(m_position.y,
                              m_cosmeticRadius,
                              screenDimensions.y - m_cosmeticRadius);
}

void Hexagon::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}

void Hexagon::ShrinkWindow()
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

STATIC bool Hexagon::OnCollisionEnter(EventArgs& args)
{
    String   entityA   = args.GetValue("entityA", "DEFAULT");
    String   entityB   = args.GetValue("entityB", "DEFAULT");
    EntityID entityBID = args.GetValue("entityBID", -1);
    Entity*  entity    = g_game->GetEntityByEntityID(entityBID);

    if (entityA == "Bullet" && entityB == "Hexagon")
    {
        if (entity->m_entityID == entityBID)
        {
            entity->DecreaseHealth(1);

            // Knockback in opposite direction of movement, clamped for safety
            Vec2 const knockback = entity->m_velocity.GetClamped(1.f) * 15.f;
            entity->m_position -= knockback;
        }

        DebuggerPrintf("HEXAGON HIT\n");
    }

    return false;
}
