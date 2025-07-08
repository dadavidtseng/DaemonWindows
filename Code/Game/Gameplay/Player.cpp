//----------------------------------------------------------------------------------------------------
// Player.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Player.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/Game.hpp"

Player::Player()
{
}

void Player::Update(float deltaSeconds)
{
    UpdateFromInput();
}

void Player::Render() const
{
    VertexList_PCU verts2;
    AddVertsForDisc2D(verts2, Window::s_mainWindow->GetScreenDimensions() * 0.5f + m_position, 30.f, 10.f, Rgba8::YELLOW);
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
        Bullet* bullet     = new Bullet();
        bullet->m_velocity = Vec2::ONE;
        bullet->m_name     = "BULLET";
        bullet->m_position = m_position;
        g_theGame->m_entities.push_back(bullet);
    }
}
