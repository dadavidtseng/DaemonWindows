//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

void WindowSubsystem::StartUp()
{
    sWindowConfig config;
    // WindowID id = 0;
    // m_windowList.emplace(id, std::make_unique<Window>(config));
    // Window* window = m_windowList.at(id).get();
    CreateAndRegisterMultipleWindows(m_windowList, 3);
    // CreateGameWindow(L"ChildWindow ", 100,100,100,100);
}

void WindowSubsystem::BeginFrame()
{
}

void WindowSubsystem::Update()
{
    for (Window& window : m_windowList)
    {
        // window.UpdateWindowDrift((float)Clock::GetSystemClock().GetDeltaSeconds() * 1.5f);
        window.UpdateWindowPosition();
    }

    UpdateWindowsResizeIfNeeded(m_windowList);
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

void WindowSubsystem::RenderWindows() const
{
    for (const Window& window : m_windowList)
    {
        if (window.needsUpdate)
        {
            // g_theRenderer->RenderViewportToWindow(window);
            g_theRenderer->RenderViewportToWindowDX11(window);
        }
    }
}

void WindowSubsystem::UpdateWindowsResizeIfNeeded(std::vector<Window>& windows)
{
    for (Window& window : windows)
    {
        if (window.needsResize)
        {
            HRESULT hr         = g_theRenderer->ResizeWindowSwapChain(window);
            window.needsResize = false;

            if (FAILED(hr))
            {
                DebuggerPrintf("Failed to resize window swap chain: 0x%08X\n", hr);
                continue;
            }

            window.needsUpdate = true;
        }
    }
}

void WindowSubsystem::UpdateWindows(std::vector<Window>& windows) const
{
    for (int i = 0; i < windows.size(); ++i)
    {
        if (windows[i].needsResize)
        {
            HRESULT hr             = g_theRenderer->ResizeWindowSwapChain(windows[i]);
            windows[i].needsResize = false;
            if (FAILED(hr))
            {
                DebuggerPrintf("Failed to resize window swap chain: 0x%08X\n", hr);
                continue;
            }
        }

        if (windows[i].needsUpdate)
        {
            // 使用 DirectX 11 版本渲染
            g_theRenderer->RenderViewportToWindowDX11(windows[i]);
            // g_theRenderer->RenderViewportToWindow(windows[i]);
            // window.needsUpdate = false;
        }
    }
}

void WindowSubsystem::CreateAndRegisterMultipleWindows(std::vector<Window>& windows, int windowCount)
{
    const int width   = 400;
    const int height  = 300;
    const int startX  = 100;
    const int startY  = 100;
    const int offsetX = 450;
    const int offsetY = 350;

    for (int i = 0; i < windowCount; ++i)
    {
        std::wstring title = L"ChildWindow " + std::to_wstring(i + 1);
        int          x     = startX + (i % 5) * offsetX;       // 每列最多5個視窗
        int          y     = startY + (i / 5) * offsetY;       // 每滿5個換行

        HWND hwnd = CreateGameWindow(title.c_str(), x, y, width, height);
        if (hwnd)
        {
            sWindowConfig config;
            Window        window    = Window(config);
            window.m_windowHandle   = hwnd;
            window.m_displayContext = GetDC(hwnd);
            window.needsUpdate      = true;

            m_windowList.push_back(window);

            g_theRenderer->CreateWindowSwapChain(windows[i]);
        }
    }
}


HWND WindowSubsystem::CreateGameWindow(const wchar_t* title, int x, int y, int width, int height)
{
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
        ShowWindow(hwnd, SW_SHOW);
        // UpdateWindow(hwnd);
    }

    return hwnd;
}
