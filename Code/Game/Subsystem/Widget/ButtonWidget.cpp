//----------------------------------------------------------------------------------------------------
// ButtonWidget.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Widget/ButtonWidget.hpp"

ButtonWidget::ButtonWidget(WidgetSubsystem* owner, const String& text, int x, int y, int width, int height, Rgba8 const& color)
    : m_text(text),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_color(color)
{
    SetOwner(owner);
    SetName("ButtonWidget_" + text);
}

void ButtonWidget::Draw() const
{
    // 實際的繪製邏輯 (假設有一個 Renderer 類別)
    // Renderer::DrawButton(m_x, m_y, m_width, m_height, m_text);
    VertexList_PCU verts;
    AddVertsForAABB2D(verts, AABB2(Vec2(m_x, m_y), Vec2(m_x + m_width, m_y + m_height)), m_color);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(verts);
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
