//----------------------------------------------------------------------------------------------------
// Bullet.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"

#include "Game.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Bullet::Bullet(Vec2 const& position, float const orientationDegrees, Rgba8 const& color)
    : Entity(position, orientationDegrees, color)
{
    // 明確初始化所有重要成員
    m_name         = "BULLET";
    m_physicRadius = 10.f;
    m_speed        = 500.0f;
    m_health       = 1;  // 子彈通常一擊就消失
}

void Bullet::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);
    UpdateFromInput();
    m_position.x += m_velocity.x * deltaSeconds * m_speed;
    m_position.y += m_velocity.y * deltaSeconds * m_speed;

    Window* window = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowByActor(g_theGame->m_entities[0]->m_actorID));
    if (m_position.x>window->GetWindowPosition().x+window->GetWindowDimensions().x)
    {
        Vec2 dimensions = window->GetWindowDimensions();
        window->SetWindowDimensions(dimensions+Vec2(100,0));
        m_health-=1;
    }
}

void Bullet::Render() const
{
    VertexList_PCU verts;
    AddVertsForDisc2D(verts, m_position, m_physicRadius, m_color);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);
}

void Bullet::UpdateFromInput()
{
}
