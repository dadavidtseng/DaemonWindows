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
// Constants
//----------------------------------------------------------------------------------------------------
constexpr WindowID INVALID_WINDOW_ID         = 0;
constexpr float    DEFAULT_ANIMATION_DURATION = 0.5f;

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
    std::unique_ptr<Window>      m_window;
    std::unordered_set<EntityID> m_owners;
    String                       m_name;
    bool                         m_isActive = true;
};

struct sWindowSubsystemConfig
{
    wchar_t const* m_iconFilePath = nullptr;
};

//----------------------------------------------------------------------------------------------------
class WindowSubsystem
{
public:
    explicit WindowSubsystem(sWindowSubsystemConfig const& config);
    void     StartUp();
    void     BeginFrame();
    void     Update();
    void     Render();
    void     EndFrame();
    void     ShutDown();

    // 核心視窗管理功能
    WindowID CreateChildWindow(EntityID owner, String const& windowTitle, int x, int y, int width, int height);
    bool     AddEntityToWindow(WindowID windowID, EntityID entityID);
    bool     RemoveEntityFromWindow(WindowID windowID, EntityID entityID);
    void     RemoveEntityFromMappings(EntityID entityID);
    void     DestroyWindow(WindowID windowID);
    void     DestroyAllWindows();

    // Show and Hide window
    void ShowWindowByWindowID(WindowID windowID);
    void HideWindowByWindowID(WindowID windowID);

    // 查詢功能
    Window*               GetWindow(WindowID windowID);
    WindowData*           GetWindowData(WindowID windowID);
    WindowID              FindWindowIDByEntityID(EntityID entityID);
    std::vector<EntityID> GetWindowOwners(WindowID windowID);
    std::vector<WindowID> GetActorWindows(EntityID entityID);
    std::vector<WindowID> GetAllWindowIDs();
    bool                  IsActorInWindow(WindowID windowID, EntityID entityID);
    bool                  WindowExists(WindowID windowID);

    // 視窗操作
    void   UpdateWindowPosition(WindowID windowID);
    void   UpdateWindowPosition(WindowID windowID, Vec2 const& newPosition); // DEPRECATED: Use MoveWindowByOffset instead
    void   MoveWindowByOffset(WindowID windowID, Vec2 const& offset);
    void   UpdateWindowDimension(WindowID windowID);
    void   SetWindowActive(WindowID windowID, bool active);
    void   SetWindowName(WindowID windowId, String const& name);
    String GetWindowName(WindowID windowId);

    size_t GetWindowCount() const;
    size_t GetActiveWindowCount() const;

    // Animations
    void AnimateWindowDimensions(WindowID id, Vec2 const& targetDimensions, float duration = DEFAULT_ANIMATION_DURATION);
    void AnimateWindowPosition(WindowID id, Vec2 const& targetPosition, float duration = DEFAULT_ANIMATION_DURATION);
    void AnimateWindowPositionAndDimensions(WindowID id, Vec2 const& targetPosition, Vec2 const& targetDimensions, float duration = DEFAULT_ANIMATION_DURATION);
    bool IsWindowAnimating(WindowID id) const;

private:
    sWindowSubsystemConfig                            m_config;
    std::unordered_map<WindowID, WindowData>          m_windowList;// 主要資料結構：WindowID -> WindowData
    std::unordered_map<EntityID, WindowID>            m_actorToWindow;// 快速查找：ActorID -> WindowID (一個actor只能在一個視窗)
    std::unordered_map<WindowID, WindowAnimationData> m_windowAnimations;
    WindowID                                          m_nextWindowID = 1; // 從1開始，0保留為無效ID

    HWND   CreateOSWindow(String const& title, int x, int y, int width, int height);
    void   InitializeWindowClientPosition(Window* window, HWND hwnd);
    Window* GetValidatedWindow(WindowID windowID, char const* callerName = nullptr);
    void   SetupTransparentMainWindow();
    String GenerateDefaultWindowName(std::vector<EntityID> const& owners) const;

    void UpdateWindowAnimations(float deltaSeconds);
    void UpdateSingleWindowAnimation(WindowID id, WindowAnimationData& animData, float deltaSeconds);
};
