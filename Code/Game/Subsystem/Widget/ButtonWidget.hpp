//----------------------------------------------------------------------------------------------------
// ButtonWidget.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Widget/IWidget.hpp"

//----------------------------------------------------------------------------------------------------
class ButtonWidget : public IWidget
{
public:
    ButtonWidget(WidgetSubsystem* owner, String const& text, int x, int y, int width, int height, Rgba8 const& color);

    void Draw() const override;
    void Update() override;

    void   SetText(String const& text);
    String GetText() const;
    void   SetPosition(Vec2 const& newPosition);
    void   SetDimensions(Vec2 const& newDimensions);

private:
    String m_text;
    int    m_x, m_y, m_width, m_height;
    Rgba8  m_color;
};
