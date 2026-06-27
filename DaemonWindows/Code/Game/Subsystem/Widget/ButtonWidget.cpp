//----------------------------------------------------------------------------------------------------
// ButtonWidget.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
ButtonWidget::ButtonWidget([[maybe_unused]] WidgetSubsystem* owner,
                           String const&                    text,
                           int                              x,
                           int                              y,
                           int                              width,
                           int                              height,
                           Rgba8 const&                     color)
    : m_text(text),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_color(color)
{
    SetName("ButtonWidget_" + text);
}

void ButtonWidget::Draw() const
{
    VertexList_PCU verts;
    BitmapFont*    bitmapFont = g_resourceSubsystem->CreateOrGetBitmapFontFromFile("Data/Fonts/DaemonFont");
    bitmapFont->AddVertsForTextInBox2D(verts, m_text, AABB2(Vec2(m_x, m_y), Vec2(m_x + m_width, m_y + m_height)), 20.f, m_color, 1.f, Vec2(1, 0), eTextBoxMode::OVERRUN);
    g_renderer->BindTexture(&bitmapFont->GetTexture());
    g_renderer->DrawVertexArray(verts);
}

void ButtonWidget::Update()
{
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
    m_x = static_cast<int>(newPosition.x);
    m_y = static_cast<int>(newPosition.y);
}

void ButtonWidget::SetDimensions(Vec2 const& newDimensions)
{
    m_width  = static_cast<int>(newDimensions.x);
    m_height = static_cast<int>(newDimensions.y);
}
