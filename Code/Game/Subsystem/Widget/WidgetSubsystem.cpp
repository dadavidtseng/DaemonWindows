//----------------------------------------------------------------------------------------------------
// WidgetSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

#include <algorithm>

#include "Game/Subsystem/Widget/IWidget.hpp"

//----------------------------------------------------------------------------------------------------
// WidgetSubsystem Implementation
//----------------------------------------------------------------------------------------------------

WidgetSubsystem::WidgetSubsystem(sWidgetSubsystemConfig const& config )
    :m_config(config)
{
    m_widgets.reserve(m_config.m_initialWidgetCapacity);
    m_entityWidgets.reserve(m_config.m_initialOwnerCapacity);
}

WidgetSubsystem::~WidgetSubsystem()
{
}

void WidgetSubsystem::StartUp()
{
    // 清空所有容器
    m_widgets.clear();
    m_entityWidgets.clear();
    m_viewportWidget = nullptr;
    m_bNeedsSorting  = false;
}

void WidgetSubsystem::BeginFrame()
{
    // 對所有 Widget 呼叫 BeginFrame
    for (auto& widget : m_widgets)
    {
        if (widget && !widget->IsGarbage())
        {
            widget->BeginFrame();
        }
    }
}

void WidgetSubsystem::Update()
{
    // 清理垃圾 Widget
    CleanupGarbageWidgets();

    // 如果需要，重新排序
    if (m_bNeedsSorting)
    {
        SortWidgetsByZOrder();
        m_bNeedsSorting = false;
    }

    // 更新所有需要 Tick 的 Widget
    for (auto& widget : m_widgets)
    {
        if (widget && widget->IsTick() && !widget->IsGarbage())
        {
            widget->Update();
        }
    }
}

void WidgetSubsystem::Render() const
{
    // 按照 Z-Order 渲染所有 Widget
    for (auto& widget : m_widgets)
    {
        if (widget && widget->IsVisible() && !widget->IsGarbage())
        {
            widget->Render();
        }
    }
}

void WidgetSubsystem::EndFrame()
{
    // 對所有 Widget 呼叫 EndFrame
    for (auto& widget : m_widgets)
    {
        if (widget && !widget->IsGarbage())
        {
            widget->EndFrame();
        }
    }
}

void WidgetSubsystem::ShutDown()
{
    RemoveAllWidgets();
    m_viewportWidget = nullptr;
}

void WidgetSubsystem::AddWidget(WidgetPtr widget, int zOrder)
{
    if (!widget) return;

    widget->SetZOrder(zOrder);
    m_widgets.push_back(widget);
    m_bNeedsSorting = true;
}

void WidgetSubsystem::AddWidgetToEntity(WidgetPtr widget,void* entity, int zOrder)
{
    if (!widget || !entity) return;

    widget->SetOwner(entity);
    widget->SetZOrder(zOrder);

    m_widgets.push_back(widget);
    m_entityWidgets[entity].push_back(widget);
    m_bNeedsSorting = true;
}

void WidgetSubsystem::RemoveWidget(WidgetPtr widget)
{
    if (!widget) return;

    // 從主要列表中移除
    auto it = std::find(m_widgets.begin(), m_widgets.end(), widget);
    if (it != m_widgets.end())
    {
        m_widgets.erase(it);
    }

    // 從 Entity 映射中移除
    void* owner = widget->GetOwner();
    if (owner && m_entityWidgets.find(owner) != m_entityWidgets.end())
    {
        auto& entityWidgets = m_entityWidgets[owner];
        auto  entityIt      = std::find(entityWidgets.begin(), entityWidgets.end(), widget);
        if (entityIt != entityWidgets.end())
        {
            entityWidgets.erase(entityIt);
        }

        // 如果這個 Entity 沒有 Widget 了，移除整個條目
        if (entityWidgets.empty())
        {
            m_entityWidgets.erase(owner);
        }
    }
}

void WidgetSubsystem::RemoveAllWidgetsFromEntity(void* entity)
{
    if (!entity) return;

    auto it = m_entityWidgets.find(entity);
    if (it != m_entityWidgets.end())
    {
        // 從主要列表中移除所有屬於這個 Entity 的 Widget
        for (auto& widget : it->second)
        {
            auto mainIt = std::find(m_widgets.begin(), m_widgets.end(), widget);
            if (mainIt != m_widgets.end())
            {
                m_widgets.erase(mainIt);
            }
        }

        // 從 Entity 映射中移除
        m_entityWidgets.erase(it);
    }
}

void WidgetSubsystem::RemoveAllWidgets()
{
    m_widgets.clear();
    m_entityWidgets.clear();
}

WidgetPtr WidgetSubsystem::FindWidgetByName(const String& name) const
{
    for (auto& widget : m_widgets)
    {
        if (widget && widget->GetName() == name)
        {
            return widget;
        }
    }
    return nullptr;
}

std::vector<WidgetPtr> WidgetSubsystem::GetWidgetsByEntity(void* entity) const
{
    auto it = m_entityWidgets.find(entity);
    if (it != m_entityWidgets.end())
    {
        return it->second;
    }
    return {};
}

std::vector<WidgetPtr> WidgetSubsystem::GetAllWidgets() const
{
    return m_widgets;
}

void WidgetSubsystem::SetViewportWidget(WidgetPtr widget)
{
    m_viewportWidget = widget;
}

WidgetPtr WidgetSubsystem::GetViewportWidget() const
{
    return m_viewportWidget;
}


void WidgetSubsystem::SortWidgetsByZOrder()
{
    if (m_widgets.empty()) return;
    std::sort(m_widgets.begin(), m_widgets.end(),
              [](const WidgetPtr& a, const WidgetPtr& b) {
                  return a->GetZOrder() < b->GetZOrder();
              });
}

void WidgetSubsystem::CleanupGarbageWidgets()
{
    // 移除標記為垃圾的 Widget
    m_widgets.erase(
        std::remove_if(m_widgets.begin(), m_widgets.end(),
                       [](const WidgetPtr& widget) {
                           return !widget || widget->IsGarbage();
                       }),
        m_widgets.end());

    // 清理 Entity 映射中的垃圾 Widget
    for (auto& pair : m_entityWidgets)
    {
        auto& widgets = pair.second;
        widgets.erase(
            std::remove_if(widgets.begin(), widgets.end(),
                           [](const WidgetPtr& widget) {
                               return !widget || widget->IsGarbage();
                           }),
            widgets.end());
    }

    // 移除空的 Entity 條目
    for (auto it = m_entityWidgets.begin(); it != m_entityWidgets.end();)
    {
        if (it->second.empty())
        {
            it = m_entityWidgets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
