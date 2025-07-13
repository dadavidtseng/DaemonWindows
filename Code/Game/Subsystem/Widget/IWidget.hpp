//----------------------------------------------------------------------------------------------------
// IWidget.hpp
//----------------------------------------------------------------------------------------------------

#pragma once
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Framework/GameCommon.hpp"


//-Forward-Declaration--------------------------------------------------------------------------------
class Entity;

//----------------------------------------------------------------------------------------------------
class IWidget
{
    friend class WidgetSubsystem;

public:
    IWidget(WidgetSubsystem* owner);
    virtual ~IWidget();

    virtual void BeginFrame();
    virtual void Render();
    virtual void Draw() const;
    virtual void Update();
    virtual void EndFrame();

    /// Getter
    virtual Entity* GetOwner() const;
    virtual int     GetZOrder() const;
    virtual String  GetName() const;
    virtual bool    IsVisible() const;
    virtual bool    IsTick() const;
    virtual bool    IsGarbage() const;

    /// Setter
    virtual void SetOwner(Entity* owner);
    virtual void SetZOrder(int zOrder);
    virtual void SetName(const String& name);
    virtual void SetVisible(bool visible);
    virtual void SetTick(bool tick);

    /// Viewport Management
    virtual void AddToViewport(int zOrder = 0);
    virtual void AddToEntityViewport(Entity* entity, int zOrder = 0);
    virtual void RemoveFromViewport();

    /// Lifecycle
    virtual void MarkForDestroy();

protected:
    WidgetSubsystem* m_owner      = nullptr;
    Entity*          m_entity     = nullptr; // 擁有此 Widget 的 Entity
    int              m_zOrder     = 0;       // 渲染順序，數字越大越在前面
    bool             m_bIsTick    = true;    // 是否需要 Update
    String           m_name       = "DEFAULT";
    bool             m_bIsVisible = true;    // 是否可見
    bool             m_bIsGarbage = false;   // 是否標記為垃圾回收
};

//----------------------------------------------------------------------------------------------------
// 使用範例 1: 自定義 Widget 類別
//----------------------------------------------------------------------------------------------------

class ButtonWidget : public IWidget
{
public:
    ButtonWidget(WidgetSubsystem* owner, const String& text, int x, int y, int width, int height)
        : IWidget(owner), m_text(text), m_x(x), m_y(y), m_width(width), m_height(height)
    {
        SetName("ButtonWidget_" + text);
    }

    void Draw() const override
    {
        // 實際的繪製邏輯 (假設有一個 Renderer 類別)
        // Renderer::DrawButton(m_x, m_y, m_width, m_height, m_text);
        VertexList_PCU verts;
        AddVertsForAABB2D(verts, AABB2(Vec2::ZERO, Vec2(500,500)), Rgba8::GREEN);
        g_theRenderer->BindTexture(nullptr);
        g_theRenderer->DrawVertexArray(verts);
    }

    void Update() override
    {
        // 處理按鈕邏輯，例如滑鼠懸停、點擊等
        // if (Input::IsMouseInRect(m_x, m_y, m_width, m_height))
        // {
        //     // 處理懸停邏輯
        // }
    }

    void   SetText(const String& text) { m_text = text; }
    String GetText() const { return m_text; }

private:
    String m_text;
    int    m_x, m_y, m_width, m_height;
};
