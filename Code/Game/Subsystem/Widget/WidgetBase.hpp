//----------------------------------------------------------------------------------------------------
// WidgetBase.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/StringUtils.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class PlayerController;

//----------------------------------------------------------------------------------------------------
class Widget
{
    friend class WidgetSubsystem;

public:
    Widget();
    virtual ~Widget();

    virtual void BeginFrame();
    virtual void Render();
    virtual void Draw() const;
    virtual void Update();
    virtual void EndFrame();

    /// Getter
    virtual PlayerController* GetOwner();
    virtual int               GetZOrder() const;
    virtual String            GetName() const;

    virtual void AddToViewport(int zOrder = 0);
    virtual void AddToPlayerViewport(PlayerController* player, int zOrder = 0);
    virtual void RemoveFromViewport();

protected:
    PlayerController* m_owner      = nullptr; // If player controller is null it basic means that it is the viewport widget.
    int               m_zOrder     = 0;
    bool              m_bIsTick    = true;
    String            m_name       = "DEFAULT";
    bool              m_bIsVisible = true;
    bool              m_bIsGarbage = false;
};
