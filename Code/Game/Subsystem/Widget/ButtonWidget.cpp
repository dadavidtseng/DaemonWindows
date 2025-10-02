//----------------------------------------------------------------------------------------------------
// ButtonWidget.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Widget/ButtonWidget.hpp"

#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"

ButtonWidget::ButtonWidget(WidgetSubsystem* owner, String const& text, int x, int y, int width, int height, Rgba8 const& color)
    : m_text(text),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_color(color)
{
    // Note: Owner parameter is kept for backward compatibility but not used
    // Widgets are now managed by WidgetSubsystem::AddWidget/AddWidgetToOwner
    SetName("ButtonWidget_" + text);
}

void ButtonWidget::Draw() const
{
    // VertexList_PCU verts1;
    // AddVertsForAABB2D(verts1, AABB2(Vec2(m_x, m_y), Vec2(m_x + m_width, m_y + m_height)), m_color);
    // g_theRenderer->BindTexture(nullptr);
    // g_theRenderer->DrawVertexArray(verts1);

    VertexList_PCU verts2;
    // g_theBitmapFont->AddVertsForText2D(verts, m_text, Vec2(m_x, m_y), 20.f);
    g_bitmapFont->AddVertsForTextInBox2D(verts2, m_text, AABB2(Vec2(m_x, m_y), Vec2(m_x + m_width, m_y + m_height)), 20.f, m_color, 1.f, Vec2(1, 0), OVERRUN);
    g_renderer->BindTexture(&g_bitmapFont->GetTexture());
    g_renderer->DrawVertexArray(verts2);
}

void ButtonWidget::Update()
{
    // 處理按鈕邏輯，例如滑鼠懸停、點擊等
    // if (Input::IsMouseInRect(m_x, m_y, m_width, m_height))
    // {
    //     // 處理懸停邏輯
    // }
}

void ButtonWidget::SetText(String const& text)
{
    m_text = text;
}

String ButtonWidget::GetText() const
{
    return m_text;
}

void ButtonWidget::SetPosition(Vec2 const& newPosition)
{
    m_x = (int)newPosition.x;
    m_y = (int)newPosition.y;
}

void ButtonWidget::SetDimensions(Vec2 const& newDimensions)
{
    m_width  = (int)newDimensions.x;
    m_height = (int)newDimensions.y;
}
