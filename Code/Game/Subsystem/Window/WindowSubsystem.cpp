//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

#include <dxgi1_2.h>

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Gameplay/Game.hpp"

void WindowSubsystem::StartUp()
{

    // 創建一些測試視窗
    // std::vector<std::vector<ActorID>> ownerGroups = {
    //     {0},        // 視窗1: Actor 0
    //     {1, 2},     // 視窗2: Actor 1和2共享
    //     {3},        // 視窗3: Actor 3
    //     {4, 5},     // 視窗4: Actor 4和5共享
    // };
    //
    // CreateMultipleWindows(ownerGroups);
}

void WindowSubsystem::BeginFrame()
{
}

void WindowSubsystem::Update()
{
    float deltaSeconds = (float)Clock::GetSystemClock().GetDeltaSeconds();
    // 更新慣性移動
    UpdateWindowAnimations(deltaSeconds);
    // UpdateWindowInertia(deltaSeconds);    // 更新動畫

    for (auto& [windowId, windowData] : m_windowList)
    {
        if (!windowData.isActive || !windowData.m_window) continue;

        windowData.m_window->UpdatePosition();
        windowData.m_window->UpdateDimension();

        if (windowData.m_window->m_shouldUpdateDimension)
        {
            HRESULT const hr                             = g_theRenderer->ResizeWindowSwapChain(*windowData.m_window);
            windowData.m_window->m_shouldUpdateDimension = false;

            if (FAILED(hr))
            {
                DebuggerPrintf("Failed to resize window swap chain for WindowID %d: 0x%08X\n", windowId, hr);
            }
        }
    }
}

void WindowSubsystem::Render()
{
    g_theRenderer->ReadStagingTextureToPixelData();

    for (auto& [windowId, windowData] : m_windowList)
    {
        if (!windowData.isActive || !windowData.m_window) continue;

        if (windowData.m_window->m_shouldUpdatePosition)
        {
            g_theRenderer->RenderViewportToWindow(*windowData.m_window);
            // g_theRenderer->RenderViewportToWindowDX11(*windowData.m_window);
        }
    }
}

void WindowSubsystem::EndFrame()
{
}

void WindowSubsystem::ShutDown()
{
    DestroyAllWindows();
}

WindowID WindowSubsystem::CreateChildWindow(EntityID const owner,
                                            String const&  windowTitle,
                                            int const      x,
                                            int const      y,
                                            int const      width,
                                            int const      height)
{
    // 創建作業系統視窗
    HWND hwnd = CreateOSWindow(windowTitle, x, y, width, height);

    if (!hwnd)
    {
        DebuggerPrintf("CreateWindowInternal: Failed to create OS window.\n");
        return 0;
    }

    // 生成新的視窗ID
    WindowID newId = m_nextWindowID++;

    // 創建視窗配置
    sWindowConfig config;
    config.m_windowType  = eWindowType::WINDOWED;
    config.m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    config.m_windowTitle = windowTitle;

    // 創建 Window 物件
    std::unique_ptr<Window> newWindow = std::make_unique<Window>(config);

    // 設定 HWND 和 Display Context
    newWindow->SetWindowHandle(hwnd);
    newWindow->SetDisplayContext(GetDC(hwnd));

    // 設定視窗位置和大小追蹤
    newWindow->SetWindowDimensions(Vec2(width, height));
    newWindow->SetWindowPosition(Vec2(x, y));
    newWindow->m_shouldUpdatePosition = true;

    // 創建 WindowData 並添加到容器
    std::unordered_set const ownerSet = {owner};
    m_windowList.emplace(newId, WindowData(std::move(newWindow), ownerSet, windowTitle));

    // 建立actor到視窗的映射
    m_actorToWindow[owner] = newId;

    // 創建 SwapChain
    if (g_theRenderer)
    {
        g_theRenderer->CreateWindowSwapChain(*m_windowList[newId].m_window);
    }

    ShowWindow(hwnd, SW_SHOW);

    DebuggerPrintf("CreateWindowInternal: Created window %d '%s' for actor %llu.\n", newId, windowTitle.c_str(), static_cast<unsigned long long>(owner));
    return newId;
}

bool WindowSubsystem::AddEntityToWindow(WindowID windowID, EntityID entityID)
{
    // 檢查視窗是否存在
    auto windowIt = m_windowList.find(windowID);
    if (windowIt == m_windowList.end())
    {
        DebuggerPrintf("AddActorToWindow: Window %d not found.\n", windowID);
        return false;
    }

    // 檢查actor是否已經在其他視窗中
    auto actorIt = m_actorToWindow.find(entityID);
    if (actorIt != m_actorToWindow.end())
    {
        if (actorIt->second == windowID)
        {
            DebuggerPrintf("AddActorToWindow: Actor %d already in window %d.\n", entityID, windowID);
            return true; // 已經在該視窗中
        }
        else
        {
            DebuggerPrintf("AddActorToWindow: Actor %d already in window %d, cannot add to window %d.\n",
                           entityID, actorIt->second, windowID);
            return false;
        }
    }

    // 添加映射關係
    windowIt->second.owners.insert(entityID);
    m_actorToWindow[entityID] = windowID;

    DebuggerPrintf("AddActorToWindow: Added Actor %d to Window %d.\n", entityID, windowID);
    return true;
}

bool WindowSubsystem::RemoveEntityFromWindow(WindowID windowID, EntityID entityID)
{
    // 檢查視窗是否存在
    auto windowIt = m_windowList.find(windowID);
    if (windowIt == m_windowList.end())
    {
        DebuggerPrintf("RemoveActorFromWindow: Window %d not found.\n", windowID);
        return false;
    }

    // 檢查actor是否在該視窗中
    auto& owners  = windowIt->second.owners;
    auto  actorIt = owners.find(entityID);
    if (actorIt == owners.end())
    {
        DebuggerPrintf("RemoveActorFromWindow: Actor %d not in window %d.\n", entityID, windowID);
        return false;
    }

    // 移除映射關係
    owners.erase(actorIt);
    m_actorToWindow.erase(entityID);

    // 如果視窗沒有任何owner了，自動銷毀視窗
    if (owners.empty())
    {
        DebuggerPrintf("RemoveActorFromWindow: Window %d now empty, destroying.\n", windowID);
        DestroyWindow(windowID);
    }
    else
    {
        DebuggerPrintf("RemoveActorFromWindow: Removed Actor %d from Window %d.\n", entityID, windowID);
    }

    return true;
}

void WindowSubsystem::DestroyWindow(WindowID windowID)
{
    auto windowIt = m_windowList.find(windowID);
    if (windowIt == m_windowList.end())
    {
        DebuggerPrintf("DestroyWindow: Window %d not found.\n", windowID);
        return;
    }

    // 移除所有相關的actor映射
    for (EntityID actorId : windowIt->second.owners)
    {
        m_actorToWindow.erase(actorId);
    }

    // 關閉視窗
    if (windowIt->second.m_window)
    {
        windowIt->second.m_window->Shutdown();
    }

    // 移除視窗資料
    m_windowList.erase(windowIt);

    DebuggerPrintf("DestroyWindow: Window %d destroyed.\n", windowID);
}

void WindowSubsystem::DestroyAllWindows()
{
    for (auto& [windowId, windowData] : m_windowList)
    {
        if (windowData.m_window)
        {
            windowData.m_window->Shutdown();
        }
    }

    m_windowList.clear();
    m_actorToWindow.clear();

    DebuggerPrintf("DestroyAllWindows: All windows destroyed.\n");
}

//----------------------------------------------------------------------------------------------------
// 查詢功能
//----------------------------------------------------------------------------------------------------

Window* WindowSubsystem::GetWindow(WindowID windowID)
{
    auto it = m_windowList.find(windowID);
    return (it != m_windowList.end() && it->second.m_window) ? it->second.m_window.get() : nullptr;
}

WindowData* WindowSubsystem::GetWindowData(WindowID const windowID)
{
    auto it = m_windowList.find(windowID);
    return (it != m_windowList.end()) ? &it->second : nullptr;
}

WindowID WindowSubsystem::FindWindowIDByEntityID(EntityID const entityID)
{
    auto it = m_actorToWindow.find(entityID);
    return (it != m_actorToWindow.end()) ? it->second : 0;
}

std::vector<EntityID> WindowSubsystem::GetWindowOwners(WindowID const windowID)
{
    std::vector<EntityID> result;
    auto                  it = m_windowList.find(windowID);
    if (it != m_windowList.end())
    {
        for (EntityID actorId : it->second.owners)
        {
            result.push_back(actorId);
        }
    }
    return result;
}

std::vector<WindowID> WindowSubsystem::GetActorWindows(EntityID const entityID)
{
    std::vector<WindowID> result;
    auto                  it = m_actorToWindow.find(entityID);
    if (it != m_actorToWindow.end())
    {
        result.push_back(it->second);
    }
    return result;
}

std::vector<WindowID> WindowSubsystem::GetAllWindowIDs()
{
    std::vector<WindowID> result;
    for (const auto& [windowId, windowData] : m_windowList)
    {
        result.push_back(windowId);
    }
    return result;
}

bool WindowSubsystem::IsActorInWindow(WindowID const windowID, EntityID const entityID)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end())
    {
        return it->second.owners.find(entityID) != it->second.owners.end();
    }
    return false;
}

bool WindowSubsystem::WindowExists(WindowID const windowID)
{
    return m_windowList.find(windowID) != m_windowList.end();
}

//----------------------------------------------------------------------------------------------------
// 視窗操作
//----------------------------------------------------------------------------------------------------

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end() && it->second.m_window)
    {
        it->second.m_window->UpdatePosition();
    }
    else
    {
        DebuggerPrintf("UpdateWindowPosition: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID,
                                           Vec2 const&    newPosition)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end() && it->second.m_window)
    {
        it->second.m_window->SetWindowPosition(it->second.m_window->GetWindowPosition() + newPosition);
        Vec2 totalPosition = it->second.m_window->GetWindowPosition() + newPosition;
        // it->second.window->UpdatePosition(newPosition);
        // it->second.window->m_shouldUpdatePosition = true;
        DebuggerPrintf("(NewPosition: %f, %f)\n", newPosition.x, newPosition.y);
        DebuggerPrintf("(it->second.window->GetWindowPosition(): %f, %f)\n", it->second.m_window->GetWindowPosition().x, it->second.m_window->GetWindowPosition().y);
    }
    else
    {
        DebuggerPrintf("UpdateWindowPosition: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::UpdateWindowDimension(WindowID windowID)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end() && it->second.m_window)
    {
        it->second.m_window->UpdateDimension();
    }
    else
    {
        DebuggerPrintf("UpdateWindowDimension: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::SetWindowActive(WindowID windowID, bool active)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end())
    {
        it->second.isActive = active;
        DebuggerPrintf("SetWindowActive: Window %d set to %s.\n", windowID, active ? "active" : "inactive");
    }
    else
    {
        DebuggerPrintf("SetWindowActive: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::SetWindowName(WindowID windowId, const std::string& name)
{
    auto it = m_windowList.find(windowId);
    if (it != m_windowList.end())
    {
        it->second.m_name = name;
        DebuggerPrintf("SetWindowName: Window %d renamed to '%s'.\n", windowId, name.c_str());
    }
    else
    {
        DebuggerPrintf("SetWindowName: Window %d not found.\n", windowId);
    }
}

std::string WindowSubsystem::GetWindowName(WindowID windowId)
{
    auto it = m_windowList.find(windowId);
    return (it != m_windowList.end()) ? it->second.m_name : "";
}

size_t WindowSubsystem::GetActiveWindowCount() const
{
    size_t count = 0;
    for (const auto& [windowId, windowData] : m_windowList)
    {
        if (windowData.isActive)
        {
            count++;
        }
    }
    return count;
}

//----------------------------------------------------------------------------------------------------
size_t WindowSubsystem::GetWindowCount() const
{
    return m_windowList.size();
}


void WindowSubsystem::RemoveEntityFromMappings(EntityID entityID)
{
    auto it = m_actorToWindow.find(entityID);
    if (it != m_actorToWindow.end())
    {
        WindowID windowId = it->second;
        RemoveEntityFromWindow(windowId, entityID);
    }
}

HWND WindowSubsystem::CreateOSWindow(String const& title,
                                     int const     x,
                                     int const     y,
                                     int const     width,
                                     int const     height)
{
    // 轉換名稱為寬字符
    std::wstring wTitle;
    wTitle.resize(title.size());
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), wTitle.data(), static_cast<int>(wTitle.size()));

    // 註冊視窗類別（只需要註冊一次）
    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc      = {};
        wc.lpfnWndProc   = (WNDPROC)GetWindowLongPtr((HWND)Window::s_mainWindow->GetWindowHandle(), GWLP_WNDPROC);
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = L"ChildWindow";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);

        RegisterClass(&wc);
        classRegistered = true;
    }

    // 調整視窗大小，確保客戶區域是指定的 width 和 height
    RECT rect = {0, 0, width, height};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    int adjustedWidth  = rect.right - rect.left;
    int adjustedHeight = rect.bottom - rect.top;

    // 創建 Windows 視窗
    HWND hwnd = CreateWindowEx(
        0,
        L"ChildWindow",
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y, adjustedWidth, adjustedHeight,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    return hwnd;
}

void WindowSubsystem::SetupTransparentMainWindow()
{
    if (!Window::s_mainWindow) return;

    HWND mainHwnd = static_cast<HWND>(Window::s_mainWindow->GetWindowHandle());

    // 設置全螢幕透明主視窗
    SetWindowLong(mainHwnd, GWL_EXSTYLE,
                  GetWindowLong(mainHwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

    // 完全透明，滑鼠穿透
    SetLayeredWindowAttributes(mainHwnd, 0, 0, LWA_ALPHA);

    // 設置為全螢幕覆蓋
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(mainHwnd, HWND_TOPMOST, 0, 0, screenWidth, screenHeight,
                 SWP_SHOWWINDOW);
}

String WindowSubsystem::GenerateDefaultWindowName(std::vector<EntityID> const& owners) const
{
    if (owners.empty()) return Stringf("Empty Window");

    if (owners.size() == 1)
    {
        return "Actor " + std::to_string(owners[0]) + " Window";
    }

    std::string name = "Shared Window (";
    for (size_t i = 0; i < owners.size(); ++i)
    {
        name += std::to_string(owners[i]);
        if (i < owners.size() - 1) name += ", ";
    }
    name += ")";

    return name;
}

void WindowSubsystem::AnimateWindowDimensions(WindowID id, Vec2 const& targetDimensions, float duration)
{
    auto windowIt = m_windowList.find(id);
    if (windowIt == m_windowList.end()) return;

    Window* window = windowIt->second.m_window.get();
    if (targetDimensions == window->GetWindowDimensions()) return;

    WindowAnimationData& animData     = m_windowAnimations[id];
    animData.m_startWindowDimensions  = window->GetWindowDimensions();
    animData.m_targetWindowDimensions = targetDimensions;
    animData.m_animationDuration      = duration;
    animData.m_animationTimer         = 0.0f;
    animData.m_isAnimatingSize        = true;
}

void WindowSubsystem::AnimateWindowPosition(WindowID id, Vec2 const& targetPosition, float duration)
{
    auto windowIt = m_windowList.find(id);
    if (windowIt == m_windowList.end()) return;

    Window* window = windowIt->second.m_window.get();
    if (targetPosition == window->GetWindowPosition()) return;

    WindowAnimationData& animData   = m_windowAnimations[id];
    animData.m_startWindowPosition  = window->GetWindowPosition();
    animData.m_targetWindowPosition = targetPosition;
    animData.m_animationDuration    = duration;
    animData.m_animationTimer       = 0.0f;
    animData.m_isAnimatingPosition  = true;
}

void WindowSubsystem::UpdateWindowAnimations(float deltaSeconds)
{
    for (auto& [windowId, animData] : m_windowAnimations)
    {
        if (animData.IsAnimating())
        {
            UpdateSingleWindowAnimation(windowId, animData, deltaSeconds);
        }
    }
}

void WindowSubsystem::UpdateSingleWindowAnimation(WindowID id, WindowAnimationData& animData, float deltaSeconds)
{
    auto windowIt = m_windowList.find(id);
    if (windowIt == m_windowList.end()) return;

    Window* window = windowIt->second.m_window.get();

    animData.m_animationTimer += deltaSeconds;
    float t = animData.m_animationTimer / animData.m_animationDuration;

    if (t >= 1.0f)
    {
        // 動畫完成
        t                              = 1.0f;
        animData.m_isAnimatingSize     = false;
        animData.m_isAnimatingPosition = false;
    }

    // 使用 SmoothStep5 來創造平滑的動畫效果
    float easedT = SmoothStep5(t);

    if (animData.m_isAnimatingSize)
    {
        Vec2 currentDimensions = Interpolate(animData.m_startWindowDimensions, animData.m_targetWindowDimensions, easedT);
        window->SetWindowDimensions(currentDimensions);
    }

    if (animData.m_isAnimatingPosition)
    {
        Vec2 currentPosition = Interpolate(animData.m_startWindowPosition, animData.m_targetWindowPosition, easedT);
        window->SetWindowPosition(currentPosition);
    }
}

void WindowSubsystem::AnimateWindowPositionAndDimensions(WindowID id, Vec2 const& targetPosition, Vec2 const& targetDimensions, float duration)
{
    auto windowIt = m_windowList.find(id);
    if (windowIt == m_windowList.end()) return;

    Window* window = windowIt->second.m_window.get();

    WindowAnimationData& animData     = m_windowAnimations[id];
    animData.m_startWindowPosition    = window->GetWindowPosition();
    animData.m_targetWindowPosition   = targetPosition;
    animData.m_startWindowDimensions  = window->GetWindowDimensions();
    animData.m_targetWindowDimensions = targetDimensions;
    animData.m_animationDuration      = duration;
    animData.m_animationTimer         = 0.f;
    animData.m_isAnimatingSize        = true;
    animData.m_isAnimatingPosition    = true;
}

bool WindowSubsystem::IsWindowAnimating(WindowID id) const
{
    auto it = m_windowAnimations.find(id);
    return (it != m_windowAnimations.end()) && it->second.IsAnimating();
}
