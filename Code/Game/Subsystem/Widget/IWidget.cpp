//----------------------------------------------------------------------------------------------------
// WidgetBase.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Widget/IWidget.hpp"

//----------------------------------------------------------------------------------------------------
// WidgetSubsystem.cpp
//----------------------------------------------------------------------------------------------------

#include "WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
// Widget Implementation
//----------------------------------------------------------------------------------------------------

IWidget::IWidget()
{
}

IWidget::~IWidget()
{
}

void IWidget::BeginFrame()
{
    // 子類別可以覆蓋此函數
}

void IWidget::Render()
{
    if (m_bIsVisible && !m_bIsGarbage)
    {
        Draw();
    }
}

void IWidget::Draw() const
{
    // 子類別必須實作此函數來繪製 UI
}

void IWidget::Update()
{
    // 子類別可以覆蓋此函數
}

void IWidget::EndFrame()
{
    // 子類別可以覆蓋此函數
}

Entity* IWidget::GetOwner() const
{
    return m_owner;
}

int IWidget::GetZOrder() const
{
    return m_zOrder;
}

String IWidget::GetName() const
{
    return m_name;
}

bool IWidget::IsVisible() const
{
    return m_bIsVisible;
}

bool IWidget::IsTick() const
{
    return m_bIsTick;
}

bool IWidget::IsGarbage() const
{
    return m_bIsGarbage;
}

void IWidget::SetOwner(Entity* owner)
{
    m_owner = owner;
}

void IWidget::SetZOrder(int zOrder)
{
    if (m_zOrder != zOrder)
    {
        m_zOrder = zOrder;
        // 通知 WidgetSubsystem 需要重新排序
        WidgetSubsystem::GetInstance().m_bNeedsSorting = true;
    }
}

void IWidget::SetName(const String& name)
{
    m_name = name;
}

void IWidget::SetVisible(bool visible)
{
    m_bIsVisible = visible;
}

void IWidget::SetTick(bool tick)
{
    m_bIsTick = tick;
}

void IWidget::AddToViewport(int zOrder)
{
    m_zOrder = zOrder;
    m_owner = nullptr; // 全域 Widget 沒有擁有者
    WidgetSubsystem::GetInstance().AddWidget(std::shared_ptr<IWidget>(this), zOrder);
}

void IWidget::AddToEntityViewport(Entity* entity, int zOrder)
{
    m_zOrder = zOrder;
    m_owner = entity;
    WidgetSubsystem::GetInstance().AddWidgetToEntity(std::shared_ptr<IWidget>(this), entity, zOrder);
}

void IWidget::RemoveFromViewport()
{
    WidgetSubsystem::GetInstance().RemoveWidget(std::shared_ptr<IWidget>(this));
}

void IWidget::MarkForDestroy()
{
    m_bIsGarbage = true;
}