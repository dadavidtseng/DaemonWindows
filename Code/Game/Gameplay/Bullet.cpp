//----------------------------------------------------------------------------------------------------
// Bullet.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"

Bullet::Bullet()
{
    // 明確初始化所有重要成員
    m_name = "BULLET";
    m_velocity = Vec2::ZERO;  // 先設為零，之後再設定
    m_position = Vec2::ZERO;
    m_speed = 100.0f;
    m_health = 1;  // 子彈通常一擊就消失
}

void Bullet::Update(float deltaSeconds)
{
    UpdateFromInput();
    m_position.x += m_velocity.x * deltaSeconds*m_speed;
    m_position.y += m_velocity.y * deltaSeconds*m_speed;
}

void Bullet::Render() const
{
    VertexList_PCU verts2;
    AddVertsForDisc2D(verts2, Window::s_mainWindow->GetScreenDimensions() * 0.5f + m_position, 30.f, 10.f, Rgba8::GREEN);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts2);
}

void Bullet::UpdateFromInput()
{
}
