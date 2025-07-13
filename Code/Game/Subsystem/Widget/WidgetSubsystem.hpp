//----------------------------------------------------------------------------------------------------
// WidgetSubsystem.hpp (完整版)
//----------------------------------------------------------------------------------------------------

#pragma once
#include "IWidget.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include "Engine/Core/StringUtils.hpp"

class Entity;
using WidgetPtr = std::shared_ptr<IWidget>;

//----------------------------------------------------------------------------------------------------
class WidgetSubsystem
{
public:
    WidgetSubsystem();
    ~WidgetSubsystem();
    WidgetSubsystem(const WidgetSubsystem&)            = delete;
    WidgetSubsystem& operator=(const WidgetSubsystem&) = delete;

    void StartUp();
    void BeginFrame();
    void Update();
    void Render();
    void EndFrame();
    void ShutDown();

    /// Widget Management
    void AddWidget(WidgetPtr widget, int zOrder = 0);
    void AddWidgetToEntity(WidgetPtr widget, Entity* entity, int zOrder = 0);
    void RemoveWidget(WidgetPtr widget);
    void RemoveAllWidgetsFromEntity(Entity* entity);
    void RemoveAllWidgets();

    /// Widget Queries
    WidgetPtr              FindWidgetByName(String const& name) const;
    std::vector<WidgetPtr> GetWidgetsByEntity(Entity* entity) const;
    std::vector<WidgetPtr> GetAllWidgets() const;

    /// Viewport Management
    void      SetViewportWidget(WidgetPtr widget);
    WidgetPtr GetViewportWidget() const;

    /// Template function for creating widgets
    template <typename T, typename... Args>
    std::shared_ptr<T> CreateWidget(Args&&... args)
    {
        static_assert(std::is_base_of_v<IWidget, T>, "T must derive from Widget");
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    bool m_bNeedsSorting = false;

private:
    void SortWidgetsByZOrder();
    void CleanupGarbageWidgets();

    std::vector<WidgetPtr>                              m_widgets;
    std::unordered_map<Entity*, std::vector<WidgetPtr>> m_entityWidgets;
    WidgetPtr                                           m_viewportWidget = nullptr;
};
