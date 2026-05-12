//----------------------------------------------------------------------------------------------------
// Bullet.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"

#include "Game.hpp"
#include "Player.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Bullet::Bullet(EntityID const& entityID,
               Vec2 const&     position,
               float const     orientationDegrees,
               Rgba8 const&    color,
               bool const      isVisible,
               bool const      hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID     = entityID;
    m_name         = "Bullet";
    m_physicRadius = 10.f;
    m_speed        = 500.f;
    m_health       = 1;

    if (m_hasChildWindow)
    {
        g_windowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 100, 100);
    }
}

Bullet::~Bullet()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
    }
}

void Bullet::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    UpdateFromInput( deltaSeconds);
    // Homing: steer toward nearest enemy
    if (m_homingStrength > 0.f && m_name == "Bullet")
    {
        Entity* nearestEnemy    = nullptr;
        float   nearestDistSq   = FLT_MAX;

        for (Entity* entity : g_game->m_entityList)
        {
            if (!entity || entity->IsDead()) continue;
            if (!Game::IsEnemy(entity)) continue;

            float distSq = GetDistanceSquared2D(m_position, entity->m_position);
            if (distSq < nearestDistSq)
            {
                nearestDistSq = distSq;
                nearestEnemy  = entity;
            }
        }

        if (nearestEnemy)
        {
            float currentAngle = m_velocity.GetOrientationDegrees();
            Vec2  toEnemy      = (nearestEnemy->m_position - m_position).GetNormalized();
            float targetAngle  = toEnemy.GetOrientationDegrees();
            float newAngle     = GetTurnedTowardDegrees(currentAngle, targetAngle, m_homingStrength * deltaSeconds);
            m_velocity         = Vec2::MakeFromPolarDegrees(newAngle);
        }
    }

    m_position.x += m_velocity.x * deltaSeconds * m_speed;
    m_position.y += m_velocity.y * deltaSeconds * m_speed;

    WindowID windowID = g_windowSubsystem->FindWindowIDByEntityID(g_game->GetPlayer()->m_entityID);
    Window*  window   = g_windowSubsystem->GetWindow(windowID);

    // Check collision with window edges
    // Player bullets expand the window; enemy bullets just die at the edge
    {
        Vec2 currentPos  = window->GetWindowPosition();
        Vec2 currentSize = window->GetWindowDimensions();

        bool hitEdge = false;

        if (m_position.x + m_physicRadius * 2.f > currentPos.x + currentSize.x)
        {
            hitEdge = true;
            if (m_name == "Bullet")
            {
                Vec2 newPos  = currentPos + Vec2(10, 0);
                Vec2 newSize = currentSize + Vec2(10, 0);
                g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            }
        }
        else if (m_position.x - m_physicRadius * 2.f < currentPos.x)
        {
            hitEdge = true;
            if (m_name == "Bullet")
            {
                Vec2 newPos  = currentPos + Vec2(-20, 0);
                Vec2 newSize = currentSize + Vec2(10, 0);
                g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            }
        }
        else if (m_position.y + m_physicRadius * 2.f > currentPos.y + currentSize.y)
        {
            hitEdge = true;
            if (m_name == "Bullet")
            {
                Vec2 newPos  = currentPos + Vec2(0, 10);
                Vec2 newSize = currentSize + Vec2(0, 10);
                g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            }
        }
        else if (m_position.y - m_physicRadius * 2.f < currentPos.y)
        {
            hitEdge = true;
            if (m_name == "Bullet")
            {
                Vec2 newPos  = currentPos + Vec2(0, -20);
                Vec2 newSize = currentSize + Vec2(0, 10);
                g_windowSubsystem->AnimateWindowPositionAndDimensions(windowID, newPos, newSize, 0.1f);
            }
        }

        if (hitEdge)
        {
            m_health -= 1;
        }
    }

    if (m_hasChildWindow)
    {
        WindowID    windowID2   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData2 = g_windowSubsystem->GetWindowData(windowID2);
        windowData2->m_window->SetClientPosition(m_position - windowData2->m_window->GetClientDimensions() * 0.5f);
    }
}

void Bullet::Render() const
{
    // Determine bullet color based on active upgrades
    bool hasPiercing = m_piercingCount > 0;
    bool hasHoming   = m_homingStrength > 0.f;

    Rgba8 bulletColor = m_color;
    if (m_name == "Bullet")
    {
        if (hasPiercing && hasHoming)
            bulletColor = Rgba8(255, 255, 0, 255);   // Yellow: both upgrades
        else if (hasPiercing)
            bulletColor = Rgba8(0, 200, 255, 255);   // Cyan: piercing
        else if (hasHoming)
            bulletColor = Rgba8(0, 255, 100, 255);   // Green: homing
    }

    VertexList_PCU verts;
    AddVertsForDisc2D(verts, m_position, m_physicRadius, bulletColor);

    // Homing ring indicator
    if (hasHoming && m_name == "Bullet")
    {
        AddVertsForDisc2D(verts, m_position, m_physicRadius + 4.f, 2.f, Rgba8(0, 255, 100, 120));
    }

    g_renderer->SetModelConstants();
    g_renderer->SetBlendMode(eBlendMode::ALPHA);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);
}

void Bullet::UpdateFromInput(float deltaSeconds)
{
    UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
bool Bullet::HasAlreadyHit(EntityID enemyID) const
{
    for (EntityID id : m_hitEnemyIDs)
    {
        if (id == enemyID) return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------
void Bullet::RegisterHit(EntityID enemyID)
{
    m_hitEnemyIDs.push_back(enemyID);
}

//----------------------------------------------------------------------------------------------------
bool Bullet::CanPierce() const
{
    return static_cast<int>(m_hitEnemyIDs.size()) <= m_piercingCount;
}

