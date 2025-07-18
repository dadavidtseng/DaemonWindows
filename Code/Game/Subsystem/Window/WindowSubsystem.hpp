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
// 視窗慣性資料結構
struct WindowInertiaData
{
    Vec2  targetPosition = Vec2::ZERO;    // 目標位置（玩家位置）
    Vec2  currentPosition = Vec2::ZERO;   // 當前位置
    Vec2  velocity = Vec2::ZERO;          // 速度
    float currentScale = 1.0f;             // 當前縮放
    float targetScale = 0.1f;              // 目標縮放（最小尺寸）
    Vec2  originalSize = Vec2(400, 300);   // 原始尺寸
};

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

    // 新增：設定玩家位置供視窗追蹤
    void     SetPlayerPosition(const Vec2& playerPos) { m_playerPosition = playerPos; }

    // 新增：更新視窗慣性移動
    void     UpdateWindowInertia(float deltaTime);
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

    std::unordered_map<WindowID, WindowInertiaData>       m_windowInertiaData;  // 新增：慣性資料
    Vec2                                                   m_playerPosition = Vec2::ZERO;  // 玩家位置

    // 慣性參數
    const float INERTIA_DAMPING =1.f;       // 阻尼係數（越小越快停止）
    const float ATTRACTION_FORCE = 10.0f;      // 吸引力強度
    const float MIN_SCALE = 0.1f;              // 最小縮放比例
    const float SCALE_SPEED = 0.01f;            // 縮放速度
    const float MIN_DISTANCE = 30.0f;          // 最小距離（停止吸引）
};
