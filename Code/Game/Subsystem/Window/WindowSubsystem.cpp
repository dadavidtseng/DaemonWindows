//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

#include <dxgi1_2.h>

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

void WindowSubsystem::StartUp()
{
    sWindowConfig config;
    // WindowID id = 0;
    // m_windowList.emplace(id, std::make_unique<Window>(config));
    // Window* window = m_windowList.at(id).get();
    std::vector<ActorID> actors = {0,1,2,3,4,5};
    CreateAndRegisterMultipleWindows(5, actors);
    // CreateGameWindow(L"ChildWindow ", 100,100,100,100);
}

void WindowSubsystem::BeginFrame()
{
}

void WindowSubsystem::Update()
{
    for (auto& [id, windowPtr] : m_windowList)
    {
        windowPtr->UpdatePosition();
        windowPtr->UpdateDimension();

        if (windowPtr->m_shouldUpdateDimension)
        {
            HRESULT const hr                   = g_theRenderer->ResizeWindowSwapChain(*windowPtr);
            windowPtr->m_shouldUpdateDimension = false;

            if (FAILED(hr))
            {
                DebuggerPrintf("Failed to resize window swap chain: 0x%08X\n", hr);
            }
        }
    }
}

void WindowSubsystem::UpdatePositionByWindowID(WindowID id)
{
    auto it = m_windowList.find(id);
    if (it != m_windowList.end())
    {
        std::unique_ptr<Window>& windowPtr = it->second;
        if (windowPtr)
        {
            windowPtr->UpdatePosition();
        }
    }
    else
    {
        DebuggerPrintf("UpdatePositionByWindowID: WindowID %d not found.\n", id);
    }
}

void WindowSubsystem::UpdateDimensionByWindowID(WindowID const id)
{
    auto const it = m_windowList.find(id);
    if (it != m_windowList.end())
    {
        if (std::unique_ptr<Window> const& windowPtr = it->second)
        {
            windowPtr->UpdateDimension();
        }
    }
    else
    {
        DebuggerPrintf("UpdatePositionByWindowID: WindowID %d not found.\n", id);
    }
}

void WindowSubsystem::Render()
{
    RenderWindows();
}

void WindowSubsystem::EndFrame()
{
}

void WindowSubsystem::ShutDown()
{
    for (auto& [id, windowPtr] : m_windowList)
    {
        // DX_SAFE_RELEASE(window.m_swapChain)
        // DX_SAFE_RELEASE(window.m_renderTargetView)
        windowPtr->Shutdown();
    }
}

WindowID WindowSubsystem::createWindow(ActorID owner)
{
    // 檢查實體是否已經擁有視窗
    auto it = actorWindowMapping.find(owner);
    if (it != actorWindowMapping.end())
    {
        // 實體已經有視窗了，返回現有的
        return it->second;
    }

    // 生成新的視窗ID
    static WindowID nextId = 1;
    WindowID        newId  = nextId++;

    // 創建新視窗
    // auto newWindow = std::make_unique<Window>();
    // newWindow->setId(newId);
    // newWindow->setOwnerId(owner);
    //
    // // 設定預設值
    // newWindow->setPosition(Vec2(100.0f, 100.0f));
    // newWindow->setSize(Vec2(400.0f, 300.0f));
    //
    // // 加入管理
    // windows[newId] = std::move(newWindow);
    // entityWindowMapping[owner] = newId;

    return newId;
}

// void WindowSubsystem::destroyWindow(WindowID id) {
//     auto windowIt = windows.find(id);
//     if (windowIt == windows.end()) {
//         return; // 視窗不存在
//     }
//
//     // 找到擁有者並移除映射
//     // EntityID owner = windowIt->second->getOwnerId();
//     // entityWindowMapping.erase(owner);
//
//     // 移除視窗
//     windows.erase(windowIt);
// }
//
// void WindowSubsystem::updateWindowPosition(WindowID id, Vec2 position) {
//     auto it = windows.find(id);
//     if (it != windows.end()) {
//         // it->second->setPosition(position);
//     }
// }
//
// void WindowSubsystem::updateWindowSize(WindowID id, Vec2 size) {
//     auto it = windows.find(id);
//     if (it != windows.end()) {
//         // 可以加入最小/最大尺寸限制
//         Vec2 clampedSize;
//         // clampedSize.x = std::max(100.0f, std::min(1920.0f, size.x));
//         // clampedSize.y = std::max(100.0f, std::min(1080.0f, size.y));
//
//         // it->second->setSize(clampedSize);
//     }
// }
//
// Window* WindowSubsystem::getWindow(WindowID id) {
//     auto it = windows.find(id);
//     if (it != windows.end()) {
//         return it->second.get();
//     }
//     return nullptr;
// }
//
// WindowID WindowSubsystem::getWindowForEntity(ActorID entityId) {
//     auto it = entityWindowMapping.find(entityId);
//     if (it != entityWindowMapping.end()) {
//         return it->second;
//     }
//     return 0; // 0 表示無效的WindowID
// }

// TODO: fix: not properly render when player moves.
void WindowSubsystem::RenderWindows()
{
    g_theRenderer->ReadStagingTextureToPixelData();

    for (auto& [id, windowPtr] : m_windowList)
    {
        if (windowPtr->m_shouldUpdatePosition)
        {
            // g_theRenderer->RenderViewportToWindow(window);
            g_theRenderer->RenderViewportToWindowDX11(*windowPtr);
        }
    }
}

void WindowSubsystem::UpdateWindows(std::vector<Window>& windows)
{
    for (Window& window : windows)
    {
        if (window.m_shouldUpdateDimension)
        {
            HRESULT hr                     = g_theRenderer->ResizeWindowSwapChain(window);
            window.m_shouldUpdateDimension = false;

            if (FAILED(hr))
            {
                DebuggerPrintf("Failed to resize window swap chain: 0x%08X\n", hr);
            }

            // window.m_shouldUpdatePosition = true;
        }
    }
}

void WindowSubsystem::CreateAndRegisterMultipleWindows(int windowCount, const std::vector<ActorID>& owners)
{
    const int width = 400;
    const int height = 300;
    const int startX = 100;
    const int startY = 100;
    const int offsetX = 450;
    const int offsetY = 350;

    for (int i = 0; i < windowCount; ++i)
    {
        // 決定 ActorID
        ActorID owner;
        if (!owners.empty() && i < static_cast<int>(owners.size()))
        {
            owner = owners[i];
        }
        else
        {
            // 如果沒有提供足夠的 ActorID，自動生成
            owner = static_cast<ActorID>(1000 + i); // 使用 1000+ 作為自動生成的 ActorID
        }

        // 檢查該 Actor 是否已經有視窗
        auto it = actorWindowMapping.find(owner);
        if (it != actorWindowMapping.end())
        {
            continue; // 跳過已經有視窗的 Actor
        }

        std::wstring title = L"ChildWindow " + std::to_wstring(i + 1) + L" (Actor " + std::to_wstring(owner) + L")";
        int x = startX + (i % 3) * offsetX;  // 每列最多3個視窗
        int y = startY + (i / 3) * offsetY;  // 每滿3個換行

        HWND hwnd = CreateGameWindow(owner, title.c_str(), x, y, width, height);

        if (!hwnd)
        {
            // 如果創建失敗，從映射中移除
            auto mapIt = actorWindowMapping.find(owner);
            if (mapIt != actorWindowMapping.end())
            {
                WindowID failedId = mapIt->second;
                actorWindowMapping.erase(mapIt);
                m_windowList.erase(failedId);
            }
        }
    }
}

HWND WindowSubsystem::CreateGameWindow(ActorID owner, const wchar_t* title, int x, int y, int width, int height)
{
    // 檢查該 Actor 是否已經有視窗
    auto it = actorWindowMapping.find(owner);
    if (it != actorWindowMapping.end())
    {
        // 如果已經有視窗，獲取現有視窗的 HWND
        Window* existingWindow = FindWindowByWindowID(it->second);
        if (existingWindow && existingWindow->GetWindowHandle())
        {
            return static_cast<HWND>(existingWindow->GetWindowHandle());
        }
    }

    // 註冊視窗類別（只需要註冊一次）
    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc = {};
        wc.lpfnWndProc = (WNDPROC)GetWindowLongPtr((HWND)Window::s_mainWindow->GetWindowHandle(), GWLP_WNDPROC);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"GameWindow";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

        RegisterClass(&wc);
        classRegistered = true;
    }

    // 調整視窗大小，確保客戶區域是指定的 width 和 height
    RECT rect = {0, 0, width, height};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    int adjustedWidth = rect.right - rect.left;
    int adjustedHeight = rect.bottom - rect.top;

    // 創建 Windows 視窗
    HWND hwnd = CreateWindowEx(
        0,
        L"GameWindow",
        title,
        WS_OVERLAPPEDWINDOW,
        x, y, adjustedWidth, adjustedHeight,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (hwnd)
    {
        // 生成新的視窗ID
        WindowID newId = m_nextWindowID++;

        // 創建視窗配置
        sWindowConfig config;
        config.m_windowType = eWindowType::WINDOWED;
        config.m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        // 轉換 title 為 string
        int titleLength = WideCharToMultiByte(CP_UTF8, 0, title, -1, nullptr, 0, nullptr, nullptr);
        std::string titleStr(titleLength - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, title, -1, &titleStr[0], titleLength, nullptr, nullptr);
        config.m_windowTitle = titleStr;

        // 創建 Window 物件
        auto newWindow = std::make_unique<Window>(config);

        // 設定 HWND 和 Display Context
        newWindow->m_windowHandle = hwnd;
        newWindow->m_displayContext = GetDC(hwnd);

        // 設定視窗位置和大小追蹤
        newWindow->SetWindowPosition(Vec2(x,y));
        newWindow->SetWindowDimensions(Vec2(width, height));
        newWindow->m_shouldUpdatePosition = true;

        // 建立映射關係
        actorWindowMapping[owner] = newId;
        m_windowList[newId] = std::move(newWindow);

        // 創建 SwapChain
        if (g_theRenderer)
        {
            g_theRenderer->CreateWindowSwapChain(*m_windowList[newId]);
        }

        ShowWindow(hwnd, SW_SHOW);
    }

    return hwnd;
}

Window* WindowSubsystem::FindWindowByWindowID(WindowID id)
{
    auto it = m_windowList.find(id);
    return (it != m_windowList.end()) ? it->second.get() : nullptr;
}

// void WindowSubsystem::CreateAndRegisterMultipleWindows(int windowCount)
// {
//     const int width   = 400;
//     const int height  = 300;
//     const int startX  = 100;
//     const int startY  = 100;
//     const int offsetX = 450;
//     const int offsetY = 350;
//
//     for (int i = 0; i < windowCount; ++i)
//     {
//         std::wstring title = L"ChildWindow " + std::to_wstring(i + 1);
//         int          x     = startX + (i % 3) * offsetX;
//         int          y     = startY + (i / 3) * offsetY;
//
//         HWND hwnd = CreateGameWindow(title.c_str(), x, y, width, height);
//         if (hwnd)
//         {
//             sWindowConfig           config;
//             std::unique_ptr<Window> window = std::make_unique<Window>(config);
//             window->m_windowHandle         = hwnd;
//             window->m_displayContext       = GetDC(hwnd);
//             window->m_shouldUpdatePosition = true; // 依實際邏輯設定
//
//             WindowID id = i; // 可自定義 WindowID 類型
//             g_theRenderer->CreateWindowSwapChain(*window); // 注意解參考
//
//             m_windowList.emplace(id, std::move(window));
//         }
//     }
// }


// HWND WindowSubsystem::CreateGameWindow(const wchar_t* title, int x, int y, int width, int height)
// {
//     static bool classRegistered = false;
//     if (!classRegistered)
//     {
//         WNDCLASS wc      = {};
//         wc.lpfnWndProc   = (WNDPROC)GetWindowLongPtr((HWND)Window::s_mainWindow->GetWindowHandle(), GWLP_WNDPROC);
//         wc.hInstance     = GetModuleHandle(nullptr);
//         wc.lpszClassName = L"GameWindow";
//         wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//         wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
//
//         RegisterClass(&wc);
//         classRegistered = true;
//     }
//
//     // 調整視窗大小，確保客戶區域是指定的 width 和 height
//     RECT rect = {0, 0, width, height};
//     AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
//
//     int adjustedWidth  = rect.right - rect.left;
//     int adjustedHeight = rect.bottom - rect.top;
//
//     HWND hwnd = CreateWindowEx(
//         0,
//         L"GameWindow",
//         title,
//         WS_OVERLAPPEDWINDOW,
//         x, y, adjustedWidth, adjustedHeight,
//         nullptr,
//         nullptr,
//         GetModuleHandle(nullptr),
//         nullptr
//     );
//
//     if (hwnd)
//     {
//         ShowWindow(hwnd, SW_SHOW);
//         // UpdateWindow(hwnd);
//     }
//
//     return hwnd;
// }
