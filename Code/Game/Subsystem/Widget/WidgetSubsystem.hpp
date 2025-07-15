//----------------------------------------------------------------------------------------------------
// WidgetSubsystem.hpp (完整版)
//----------------------------------------------------------------------------------------------------

#pragma once
#include "IWidget.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

#include "Engine/Core/StringUtils.hpp"

using WidgetPtr = std::shared_ptr<IWidget>;

//----------------------------------------------------------------------------------------------------
struct sWidgetSubsystemConfig
{
    size_t m_initialWidgetCapacity = 64;
    size_t m_initialOwnerCapacity  = 32;
};

//----------------------------------------------------------------------------------------------------
class WidgetSubsystem
{
public:
    explicit WidgetSubsystem(sWidgetSubsystemConfig const& config);
    ~WidgetSubsystem();

    void StartUp();
    void BeginFrame();
    void Update();
    void Render() const;
    void EndFrame();
    void ShutDown();

    /// Widget Management
    void AddWidget(WidgetPtr const& widget, int zOrder = 0);
    void AddWidgetToEntity(WidgetPtr const& widget, void* entity, int zOrder = 0);
    void RemoveWidget(WidgetPtr const& widget);
    void RemoveAllWidgetsFromEntity(void* entity);
    void RemoveAllWidgets();

    /// Widget Queries
    WidgetPtr              FindWidgetByName(String const& name) const;
    std::vector<WidgetPtr> GetWidgetsByOwner(void* owner) const;
    std::vector<WidgetPtr> GetAllWidgets() const;

    /// Viewport Management
    void      SetViewportWidget(WidgetPtr const& widget);
    WidgetPtr GetViewportWidget() const;

    /// Template function for creating widgets
    template <typename T, typename... Args>
    std::shared_ptr<T> CreateWidget(Args&&... args);

    bool m_bNeedsSorting = false;

private:
    void                                              SortWidgetsByZOrder();
    void                                              CleanupGarbageWidgets();
    sWidgetSubsystemConfig                            m_config;
    std::vector<WidgetPtr>                            m_widgets;
    std::unordered_map<void*, std::vector<WidgetPtr>> m_ownerWidgetsMapping;
    WidgetPtr                                         m_viewportWidget = nullptr;
};

//----------------------------------------------------------------------------------------------------
template <typename T, typename... Args>
std::shared_ptr<T> WidgetSubsystem::CreateWidget(Args&&... args)
{
    static_assert(std::is_base_of_v<IWidget, T>, "T must derive from Widget");

    return std::make_shared<T>(std::forward<Args>(args)...);
}
