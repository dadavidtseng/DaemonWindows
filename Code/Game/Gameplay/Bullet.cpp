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
    // Entity::Update(deltaSeconds);
    // UpdateFromInput();
    // m_position.x += m_velocity.x * deltaSeconds * m_speed;
    // m_position.y += m_velocity.y * deltaSeconds * m_speed;
    //
    // Window* window = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowByActor(g_theGame->m_entities[0]->m_actorID));
    // if (m_position.x > window->GetWindowPosition().x + window->GetWindowDimensions().x)
    // {
    //     window->SetWindowDimensions(window->GetWindowDimensions() + Vec2(10, 0));
    //     m_health -= 1;
    // }
    // if (m_position.x < window->GetWindowPosition().x)
    // {
    //     window->SetWindowPosition(window->GetWindowPosition() + Vec2(-10, 0));
    //     window->SetWindowDimensions(window->GetWindowDimensions() + Vec2(10, 0));
    //     m_health -= 1;
    // }
    // if (m_position.y > window->GetWindowPosition().y + window->GetWindowDimensions().y)
    // {
    //     window->SetWindowPosition(window->GetWindowPosition() + Vec2(0, 10));
    //     window->SetWindowDimensions(window->GetWindowDimensions() + Vec2(0, 10));
    //
    //     m_health -= 1;
    // }
    // if (m_position.y < window->GetWindowPosition().y)
    // {
    //     window->SetWindowDimensions(window->GetWindowDimensions() + Vec2(0, 10));
    //
    //     m_health -= 1;
    // }
    Entity::Update(deltaSeconds);
    UpdateFromInput();
    m_position.x += m_velocity.x * deltaSeconds * m_speed;
    m_position.y += m_velocity.y * deltaSeconds * m_speed;

    Window* window = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowByActor(g_theGame->m_entities[0]->m_actorID));

    // 檢查碰撞，但只在不在動畫中時才觸發新的動畫
    if (!window->IsAnimating())
    {
        Vec2 currentPos  = window->GetWindowPosition();
        Vec2 currentSize = window->GetWindowDimensions();

        if (m_position.x + m_physicRadius * 2.f > currentPos.x + currentSize.x)
        {
            // 右邊界：增加寬度
            Vec2 newPos  = currentPos + Vec2(10, 0);
            Vec2 newSize = currentSize + Vec2(10, 0);
            window->AnimateToWindowPositionAndDimensions(newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.x - m_physicRadius * 2.f < currentPos.x)
        {
            // 左邊界：向左移動並增加寬度
            Vec2 newPos  = currentPos + Vec2(-20, 0);
            Vec2 newSize = currentSize + Vec2(10, 0);
            window->AnimateToWindowPositionAndDimensions(newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.y + m_physicRadius * 2.f > currentPos.y + currentSize.y)
        {
            // 上邊界：向上移動並增加高度
            Vec2 newPos  = currentPos + Vec2(0, 10);
            Vec2 newSize = currentSize + Vec2(0, 10);
            window->AnimateToWindowPositionAndDimensions(newPos, newSize, 0.1f);
            m_health -= 1;
        }
        else if (m_position.y - m_physicRadius * 2.f < currentPos.y)
        {
            // 下邊界：增加高度
            Vec2 newPos  = currentPos + Vec2(0, -20);
            Vec2 newSize = currentSize + Vec2(0, 10);
            window->AnimateToWindowPositionAndDimensions(newPos, newSize, 0.1f);
            m_health -= 1;
        }
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
