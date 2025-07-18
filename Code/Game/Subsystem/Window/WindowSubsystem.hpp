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
struct WindowAnimationData
{
    bool m_isAnimatingSize     = false;
    bool m_isAnimatingPosition = false;

    Vec2 m_targetWindowDimensions = Vec2::ZERO;
    Vec2 m_startWindowDimensions  = Vec2::ZERO;
    Vec2 m_targetWindowPosition   = Vec2::ZERO;
    Vec2 m_startWindowPosition    = Vec2::ZERO;

    float m_animationDuration = 0.5f;
    float m_animationTimer    = 0.0f;

    bool IsAnimating() const { return m_isAnimatingSize || m_isAnimatingPosition; }
};

//----------------------------------------------------------------------------------------------------
struct WindowData
{
    std::unique_ptr<Window>      window;
    std::unordered_set<EntityID> owners;
    String                       m_name;
    bool                         isActive = true;

    WindowData() = default;

    WindowData(std::unique_ptr<Window> win, std::unordered_set<EntityID> const& ownerSet, String windowName)
        : window(std::move(win)), owners(ownerSet), m_name(std::move(windowName))
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
    bool     AddActorToWindow(WindowID windowID, EntityID entityID);
    bool     RemoveActorFromWindow(WindowID windowID, EntityID entityID);
    void     RemoveActorFromMappings(EntityID entityID);
    void     DestroyWindow(WindowID windowID);
    void     DestroyAllWindows();

    // 查詢功能
    Window*               GetWindow(WindowID windowID);
    WindowData*           GetWindowData(WindowID windowID);
    WindowID              FindWindowByActor(EntityID entityID);
    std::vector<EntityID> GetWindowOwners(WindowID windowID);
    std::vector<WindowID> GetActorWindows(EntityID entityID);
    std::vector<WindowID> GetAllWindowIDs();
    bool                  IsActorInWindow(WindowID windowID, EntityID entityID);
    bool                  WindowExists(WindowID windowID);

    // 視窗操作
    void   UpdateWindowPosition(WindowID windowID);
    void   UpdateWindowPosition(WindowID windowID, Vec2 const& newPosition);
    void   UpdateWindowDimension(WindowID windowID);
    void   SetWindowActive(WindowID windowID, bool active);
    void   SetWindowName(WindowID windowId, String const& name);
    String GetWindowName(WindowID windowId);

    // 批量操作
    void   CreateMultipleWindows(std::vector<std::vector<EntityID>> const& ownerGroups);
    size_t GetWindowCount() const;
    size_t GetActiveWindowCount() const;

    // Animations
    void AnimateWindowDimensions(WindowID id, Vec2 const& targetDimensions, float duration = 0.5f);
    void AnimateWindowPosition(WindowID id, Vec2 const& targetPosition, float duration = 0.5f);
    void AnimateWindowPositionAndDimensions(WindowID id, Vec2 const& targetPosition, Vec2 const& targetDimensions, float duration = 0.5f);
    bool IsWindowAnimating(WindowID id) const;
    // void InstallGlobalMouseHook();
    // void UninstallGlobalMouseHook();

private:
    std::unordered_map<WindowID, WindowData>          m_windowList;// 主要資料結構：WindowID -> WindowData
    std::unordered_map<EntityID, WindowID>            m_actorToWindow;// 快速查找：ActorID -> WindowID (一個actor只能在一個視窗)
    std::unordered_map<WindowID, WindowAnimationData> m_windowAnimations;
    WindowID                                          m_nextWindowID = 1; // 從1開始，0保留為無效ID

    // 內部輔助函數
    WindowID CreateWindowInternal(std::vector<EntityID> const& owners, String const& name, int x, int y, int width, int height);

    HWND        CreateOSWindow(std::wstring const& title, int x, int y, int width, int height);
    void        SetupTransparentMainWindow();
    std::string GenerateDefaultWindowName(std::vector<EntityID> const& owners);
    // 私有方法：動畫更新
    void                    UpdateWindowAnimations(float deltaSeconds);
    void                    UpdateSingleWindowAnimation(WindowID id, WindowAnimationData& animData, float deltaSeconds);
    // HHOOK                   m_globalMouseHook    = nullptr;
    // HHOOK                   m_globalKeyboardHook = nullptr;
    // void                    OnGlobalMouseEvent(int eventType, POINT mousePos);
    // void                    OnGlobalKeyboardEvent(int eventType, DWORD vkCode);
    // void                    HandleClickOutsideWindows(POINT mousePos);
    // void                    InstallGlobalKeyboardHook();
    // void                    UninstallGlobalKeyboardHook();
    // static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    // static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);  // 新增
};
