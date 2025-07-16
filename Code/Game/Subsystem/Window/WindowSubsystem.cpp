//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

#include <dxgi1_2.h>

#include "Engine/Core/ErrorWarningAssert.hpp"

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
    for (auto& [windowId, windowData] : m_windows)
    {
        if (!windowData.isActive || !windowData.window) continue;

        windowData.window->UpdatePosition();
        windowData.window->UpdateDimension();

        if (windowData.window->m_shouldUpdateDimension)
        {
            HRESULT const hr                           = g_theRenderer->ResizeWindowSwapChain(*windowData.window);
            windowData.window->m_shouldUpdateDimension = false;

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

    for (auto& [windowId, windowData] : m_windows)
    {
        if (!windowData.isActive || !windowData.window) continue;

        if (windowData.window->m_shouldUpdatePosition)
        {
            // g_theRenderer->RenderViewportToWindow(*windowData.window);
            g_theRenderer->RenderViewportToWindowDX11(*windowData.window);
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

//----------------------------------------------------------------------------------------------------
// 核心視窗管理功能
//----------------------------------------------------------------------------------------------------

WindowID WindowSubsystem::CreateChildWindow(std::vector<EntityID> const& owners, std::string const& name)
{
    if (owners.empty())
    {
        DebuggerPrintf("CreateWindow: No owners provided.\n");
        return 0;
    }

    // 檢查是否有actor已經在其他視窗中
    for (EntityID actorId : owners)
    {
        auto it = m_actorToWindow.find(actorId);
        if (it != m_actorToWindow.end())
        {
            DebuggerPrintf("CreateWindow: Actor %d already in window %d.\n", actorId, it->second);
            return 0; // 創建失敗
        }
    }

    std::string windowName = name.empty() ? GenerateDefaultWindowName(owners) : name;

    // 計算視窗位置（簡單的網格佈局）
    int windowIndex = static_cast<int>(m_windows.size());
    int x           = 100 + (windowIndex % 3) * 450;
    int y           = 100 + (windowIndex / 3) * 350;

    return CreateWindowInternal(owners, windowName, x, y, 400, 300);
}

WindowID WindowSubsystem::CreateChildWindow(EntityID const owner,
                                            String const&  name)
{
    return CreateChildWindow(std::vector<EntityID>{owner}, name);
}

bool WindowSubsystem::AddActorToWindow(WindowID windowID, EntityID entityID)
{
    // 檢查視窗是否存在
    auto windowIt = m_windows.find(windowID);
    if (windowIt == m_windows.end())
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

bool WindowSubsystem::RemoveActorFromWindow(WindowID windowID, EntityID entityID)
{
    // 檢查視窗是否存在
    auto windowIt = m_windows.find(windowID);
    if (windowIt == m_windows.end())
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
    auto windowIt = m_windows.find(windowID);
    if (windowIt == m_windows.end())
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
    if (windowIt->second.window)
    {
        windowIt->second.window->Shutdown();
    }

    // 移除視窗資料
    m_windows.erase(windowIt);

    DebuggerPrintf("DestroyWindow: Window %d destroyed.\n", windowID);
}

void WindowSubsystem::DestroyAllWindows()
{
    for (auto& [windowId, windowData] : m_windows)
    {
        if (windowData.window)
        {
            windowData.window->Shutdown();
        }
    }

    m_windows.clear();
    m_actorToWindow.clear();

    DebuggerPrintf("DestroyAllWindows: All windows destroyed.\n");
}

//----------------------------------------------------------------------------------------------------
// 查詢功能
//----------------------------------------------------------------------------------------------------

Window* WindowSubsystem::GetWindow(WindowID windowID)
{
    auto it = m_windows.find(windowID);
    return (it != m_windows.end() && it->second.window) ? it->second.window.get() : nullptr;
}

WindowData* WindowSubsystem::GetWindowData(WindowID const windowID)
{
    auto it = m_windows.find(windowID);
    return (it != m_windows.end()) ? &it->second : nullptr;
}

WindowID WindowSubsystem::FindWindowByActor(EntityID const entityID)
{
    auto it = m_actorToWindow.find(entityID);
    return (it != m_actorToWindow.end()) ? it->second : 0;
}

std::vector<EntityID> WindowSubsystem::GetWindowOwners(WindowID const windowID)
{
    std::vector<EntityID> result;
    auto                  it = m_windows.find(windowID);
    if (it != m_windows.end())
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
    for (const auto& [windowId, windowData] : m_windows)
    {
        result.push_back(windowId);
    }
    return result;
}

bool WindowSubsystem::IsActorInWindow(WindowID const windowID, EntityID const entityID)
{
    auto it = m_windows.find(windowID);
    if (it != m_windows.end())
    {
        return it->second.owners.find(entityID) != it->second.owners.end();
    }
    return false;
}

bool WindowSubsystem::WindowExists(WindowID const windowID)
{
    return m_windows.find(windowID) != m_windows.end();
}

//----------------------------------------------------------------------------------------------------
// 視窗操作
//----------------------------------------------------------------------------------------------------

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID)
{
    auto it = m_windows.find(windowID);
    if (it != m_windows.end() && it->second.window)
    {
        it->second.window->UpdatePosition();
    }
    else
    {
        DebuggerPrintf("UpdateWindowPosition: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID,
                                           Vec2 const&    newPosition)
{
    auto it = m_windows.find(windowID);
    if (it != m_windows.end() && it->second.window)
    {
        it->second.window->SetWindowPosition(it->second.window->GetWindowPosition() + newPosition);
        Vec2 totalPosition = it->second.window->GetWindowPosition() + newPosition;
        // it->second.window->UpdatePosition(newPosition);
        // it->second.window->m_shouldUpdatePosition = true;
        DebuggerPrintf("(NewPosition: %f, %f)\n", newPosition.x, newPosition.y);
        DebuggerPrintf("(it->second.window->GetWindowPosition(): %f, %f)\n", it->second.window->GetWindowPosition().x, it->second.window->GetWindowPosition().y);
    }
    else
    {
        DebuggerPrintf("UpdateWindowPosition: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::UpdateWindowDimension(WindowID windowID)
{
    auto it = m_windows.find(windowID);
    if (it != m_windows.end() && it->second.window)
    {
        it->second.window->UpdateDimension();
    }
    else
    {
        DebuggerPrintf("UpdateWindowDimension: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::SetWindowActive(WindowID windowID, bool active)
{
    auto it = m_windows.find(windowID);
    if (it != m_windows.end())
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
    auto it = m_windows.find(windowId);
    if (it != m_windows.end())
    {
        it->second.name = name;
        DebuggerPrintf("SetWindowName: Window %d renamed to '%s'.\n", windowId, name.c_str());
    }
    else
    {
        DebuggerPrintf("SetWindowName: Window %d not found.\n", windowId);
    }
}

std::string WindowSubsystem::GetWindowName(WindowID windowId)
{
    auto it = m_windows.find(windowId);
    return (it != m_windows.end()) ? it->second.name : "";
}

size_t WindowSubsystem::GetActiveWindowCount() const
{
    size_t count = 0;
    for (const auto& [windowId, windowData] : m_windows)
    {
        if (windowData.isActive)
        {
            count++;
        }
    }
    return count;
}

//----------------------------------------------------------------------------------------------------
// 批量操作
//----------------------------------------------------------------------------------------------------

void WindowSubsystem::CreateMultipleWindows(std::vector<std::vector<EntityID>> const& ownerGroups)
{
    for (size_t i = 0; i < ownerGroups.size(); ++i)
    {
        const auto& owners = ownerGroups[i];
        if (!owners.empty())
        {
            std::string name     = "Window " + std::to_string(i + 1);
            WindowID    windowId = CreateChildWindow(owners, name);

            if (windowId != 0)
            {
                DebuggerPrintf("CreateMultipleWindows: Created window %d for %zu actors.\n", windowId, owners.size());
            }
            else
            {
                DebuggerPrintf("CreateMultipleWindows: Failed to create window for group %zu.\n", i);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
size_t WindowSubsystem::GetWindowCount() const
{
    return m_windows.size();
}

//----------------------------------------------------------------------------------------------------
WindowID WindowSubsystem::CreateWindowInternal(std::vector<EntityID> const& owners, String const& name, int x, int y, int width, int height)
{
    // 轉換名稱為寬字符
    std::wstring wTitle;
    wTitle.resize(name.size());
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), static_cast<int>(name.size()), &wTitle[0], static_cast<int>(wTitle.size()));

    // 創建作業系統視窗
    HWND hwnd = CreateOSWindow(wTitle, x, y, width, height);
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
    config.m_windowTitle = name;

    // 創建 Window 物件
    std::unique_ptr<Window> newWindow = std::make_unique<Window>(config);

    // 設定 HWND 和 Display Context
    newWindow->SetWindowHandle(hwnd);
    newWindow->SetDisplayContext(GetDC(hwnd));

    // 設定視窗位置和大小追蹤
    newWindow->SetWindowPosition(Vec2(static_cast<float>(x), static_cast<float>(y)));
    newWindow->SetWindowDimensions(Vec2(static_cast<float>(width), static_cast<float>(height)));
    newWindow->m_shouldUpdatePosition = true;

    // 創建 WindowData 並添加到容器
    std::unordered_set<EntityID> ownerSet(owners.begin(), owners.end());
    m_windows.emplace(newId, WindowData(std::move(newWindow), ownerSet, name));

    // 建立actor到視窗的映射
    for (EntityID owner : owners)
    {
        m_actorToWindow[owner] = newId;
    }

    // 創建 SwapChain
    if (g_theRenderer)
    {
        g_theRenderer->CreateWindowSwapChain(*m_windows[newId].window);
    }

    ShowWindow(hwnd, SW_SHOW);

    DebuggerPrintf("CreateWindowInternal: Created window %d '%s' for %zu actors.\n", newId, name.c_str(), owners.size());
    return newId;
}

void WindowSubsystem::RemoveActorFromMappings(EntityID entityID)
{
    auto it = m_actorToWindow.find(entityID);
    if (it != m_actorToWindow.end())
    {
        WindowID windowId = it->second;
        RemoveActorFromWindow(windowId, entityID);
    }
}

HWND WindowSubsystem::CreateOSWindow(std::wstring const& title,
                                     int                 x,
                                     int                 y,
                                     int                 width,
                                     int                 height)
{
    // 註冊視窗類別（只需要註冊一次）
    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc      = {};
        wc.lpfnWndProc   = (WNDPROC)GetWindowLongPtr((HWND)Window::s_mainWindow->GetWindowHandle(), GWLP_WNDPROC);
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = L"GameWindow";
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
        L"GameWindow",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y, adjustedWidth, adjustedHeight,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    return hwnd;
}

std::string WindowSubsystem::GenerateDefaultWindowName(const std::vector<EntityID>& owners)
{
    if (owners.empty())
    {
        return "Empty Window";
    }

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
