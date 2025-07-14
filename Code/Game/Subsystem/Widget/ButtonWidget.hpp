//----------------------------------------------------------------------------------------------------
// ButtonWidget.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Subsystem/Widget/IWidget.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Entity;

class ButtonWidget : public IWidget
{
public:
    ButtonWidget(WidgetSubsystem* owner, String const& text, int x, int y, int width, int height, Rgba8 const& color);

    void Draw() const override;
    void Update() override;

    void   SetText(String const& text);
    String GetText() const;

private:
    String m_text;
    int    m_x, m_y, m_width, m_height;
    Rgba8  m_color;
};
