//----------------------------------------------------------------------------------------------------
// WindowSubsystem.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Engine/Platform/Window.hpp"
#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
struct WindowData
{
    std::unique_ptr<Window>      window;
    std::unordered_set<EntityID> owners;
    std::string                  name;
    bool                         isActive = true;

    WindowData() = default;

    WindowData(std::unique_ptr<Window> win, const std::unordered_set<EntityID>& ownerSet, const std::string& windowName)
        : window(std::move(win)), owners(ownerSet), name(windowName)
    {
    }
};

//----------------------------------------------------------------------------------------------------
class WindowSubsystem
{
public:
    void StartUp();
    void BeginFrame();
    void Update();
    void Render();
    void EndFrame();
    void ShutDown();

    // 核心視窗管理功能
    WindowID CreateChildWindow(std::vector<EntityID> const& owners, String const& name = "");
    WindowID CreateChildWindow(EntityID owner, String const& name = "");
    bool     AddActorToWindow(WindowID windowId, EntityID actorId);
    bool     RemoveActorFromWindow(WindowID windowId, EntityID actorId);
    void     RemoveActorFromMappings(EntityID actorId);
    void     DestroyWindow(WindowID windowId);
    void     DestroyAllWindows();

    // 查詢功能
    Window*               GetWindow(WindowID windowId);
    WindowData*           GetWindowData(WindowID windowId);
    WindowID              FindWindowByActor(EntityID actorId);
    std::vector<EntityID> GetWindowOwners(WindowID windowId);
    std::vector<WindowID> GetActorWindows(EntityID actorId);
    std::vector<WindowID> GetAllWindowIDs();
    bool                  IsActorInWindow(WindowID windowId, EntityID actorId);
    bool                  WindowExists(WindowID windowId);

    // 視窗操作
    void        UpdateWindowPosition(WindowID windowId);
    void        UpdateWindowPosition(WindowID windowId, Vec2 const& newPosition);
    void        UpdateWindowDimension(WindowID windowId);
    void        SetWindowActive(WindowID windowId, bool active);
    void        SetWindowName(WindowID windowId, const std::string& name);
    std::string GetWindowName(WindowID windowId);

    // 批量操作
    void   CreateMultipleWindows(std::vector<std::vector<EntityID>> const& ownerGroups);
    size_t GetWindowCount() const;
    size_t GetActiveWindowCount() const;

private:
    // 主要資料結構：WindowID -> WindowData
    std::unordered_map<WindowID, WindowData> m_windows;

    // 快速查找：ActorID -> WindowID (一個actor只能在一個視窗)
    std::unordered_map<EntityID, WindowID> m_actorToWindow;

    WindowID m_nextWindowID = 1; // 從1開始，0保留為無效ID

    // 內部輔助函數
    WindowID CreateWindowInternal(std::vector<EntityID> const& owners, String const& name, int x, int y, int width, int height);

    HWND        CreateOSWindow(std::wstring const& title, int x, int y, int width, int height);
    std::string GenerateDefaultWindowName(const std::vector<EntityID>& owners);
};
