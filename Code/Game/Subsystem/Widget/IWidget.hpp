//----------------------------------------------------------------------------------------------------
// IWidget.hpp
//----------------------------------------------------------------------------------------------------

#pragma once
#include "Engine/Core/StringUtils.hpp"


//-Forward-Declaration--------------------------------------------------------------------------------
class Entity;

//----------------------------------------------------------------------------------------------------
class IWidget
{
    friend class WidgetSubsystem;

public:
    IWidget();
    virtual ~IWidget();

    virtual void BeginFrame();
    virtual void Render();
    virtual void Draw() const;
    virtual void Update();
    virtual void EndFrame();

    /// Getter
    virtual void* GetOwner() const;
    virtual int     GetZOrder() const;
    virtual String  GetName() const;
    virtual bool    IsVisible() const;
    virtual bool    IsTick() const;
    virtual bool    IsGarbage() const;

    /// Setter
    virtual void SetOwner(void* owner);
    virtual void SetZOrder(int zOrder);
    virtual void SetName(const String& name);
    virtual void SetVisible(bool visible);
    virtual void SetTick(bool tick);

    /// Viewport Management
    // virtual void AddToViewport(int zOrder = 0);
    // virtual void AddToEntityViewport(Entity* entity, int zOrder = 0);
    // virtual void RemoveFromViewport();

    /// Lifecycle
    virtual void MarkForDestroy();

protected:
    void* m_owner      = nullptr;
    // Entity*          m_entity     = nullptr; // 擁有此 Widget 的 Entity
    int              m_zOrder     = 0;       // 渲染順序，數字越大越在前面
    bool             m_bIsTick    = true;    // 是否需要 Update
    String           m_name       = "DEFAULT";
    bool             m_bIsVisible = true;    // 是否可見
    bool             m_bIsGarbage = false;   // 是否標記為垃圾回收
};

