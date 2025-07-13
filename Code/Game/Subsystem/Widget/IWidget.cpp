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

IWidget::IWidget(WidgetSubsystem* owner)
    : m_owner(owner)
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
    return m_entity;
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
    m_entity = owner;
}

void IWidget::SetZOrder(int zOrder)
{
    if (m_zOrder != zOrder)
    {
        m_zOrder = zOrder;
        // 通知 WidgetSubsystem 需要重新排序
        m_owner->m_bNeedsSorting = true;
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
    m_owner  = nullptr; // 全域 Widget 沒有擁有者
    m_owner->AddWidget(std::shared_ptr<IWidget>(this), zOrder);
}

void IWidget::AddToEntityViewport(Entity* entity, int zOrder)
{
    m_zOrder = zOrder;
    m_entity = entity;
    m_owner->AddWidgetToEntity(std::shared_ptr<IWidget>(this), entity, zOrder);
}

void IWidget::RemoveFromViewport()
{
    m_owner->RemoveWidget(std::shared_ptr<IWidget>(this));
}

void IWidget::MarkForDestroy()
{
    m_bIsGarbage = true;
}
