//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

#include <dxgi1_2.h>

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Gameplay/Game.hpp"

//----------------------------------------------------------------------------------------------------
WindowSubsystem::WindowSubsystem(sWindowSubsystemConfig const& config)
    : m_config(config)
{
}

void WindowSubsystem::StartUp()
{
}

void WindowSubsystem::BeginFrame()
{
}

void WindowSubsystem::Update()
{
    if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
    float const deltaSeconds = static_cast<float>(g_game->GetGameClock()->GetDeltaSeconds());

    UpdateWindowAnimations(deltaSeconds);

    for (auto& [windowId, windowData] : m_windowList)
    {
        if (!windowData.m_isActive || !windowData.m_window) continue;

        windowData.m_window->UpdatePosition();

        // NOTE: Swap chain resizing is disabled due to DirectX limitations
        // The Engine's UpdateDimension() has a bug that causes infinite resize loops
        // (it compares client rect dimensions to window dimensions, which are never equal)
        // Additionally, ResizeBuffers fails with DXGI_ERROR_INVALID_CALL when called
        // during active rendering, which happens constantly in a game loop.
        // DirectX can render to windows of any size without explicit swap chain resizing.

        // Clear the flag to prevent resize attempts
        windowData.m_window->m_shouldUpdateDimension = false;
    }
}

void WindowSubsystem::Render()
{
    g_renderer->ReadStagingTextureToPixelData();

    for (auto& [windowId, windowData] : m_windowList)
    {
        if (!windowData.m_isActive || !windowData.m_window) continue;

        if (windowData.m_window->m_shouldUpdatePosition)
        {
            g_renderer->RenderViewportToWindow(*windowData.m_window);
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
    // Validate input parameters
    if (owner == 0)
    {
        DebuggerPrintf("CreateChildWindow: Invalid owner ID 0.\n");
        return INVALID_WINDOW_ID;
    }

    if (width <= 0 || height <= 0)
    {
        DebuggerPrintf("CreateChildWindow: Invalid dimensions (%d x %d) for owner %llu.\n",
                       width, height, static_cast<unsigned long long>(owner));
        return INVALID_WINDOW_ID;
    }

    // Check if entity already owns a window
    auto existingIt = m_actorToWindow.find(owner);
    if (existingIt != m_actorToWindow.end())
    {
        DebuggerPrintf("CreateChildWindow: Entity %llu already owns Window %d.\n",
                       static_cast<unsigned long long>(owner), existingIt->second);
        return existingIt->second;
    }

    HWND hwnd = CreateOSWindow(windowTitle, x, y, width, height);

    if (!hwnd)
    {
        DebuggerPrintf("CreateWindowInternal: Failed to create OS window.\n");
        return INVALID_WINDOW_ID;
    }

    WindowID newId = m_nextWindowID++;

    sWindowConfig config;
    config.m_windowType  = eWindowType::WINDOWED;
    config.m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    config.m_windowTitle = windowTitle;

    std::unique_ptr<Window> newWindow = std::make_unique<Window>(config);

    newWindow->SetWindowHandle(hwnd);
    newWindow->SetDisplayContext(GetDC(hwnd));

    newWindow->SetWindowDimensions(Vec2(static_cast<float>(width), static_cast<float>(height)));
    newWindow->SetWindowPosition(Vec2(static_cast<float>(x), static_cast<float>(y)));
    newWindow->m_shouldUpdatePosition = true;

    // Initialize client position to prevent crash when GetClientPosition() is called
    InitializeWindowClientPosition(newWindow.get(), hwnd);

    std::unordered_set const ownerSet = {owner};
    WindowData               windowData;
    windowData.m_window = std::move(newWindow);
    windowData.m_owners = ownerSet;
    windowData.m_name   = windowTitle;
    m_windowList.emplace(newId, std::move(windowData));

    m_actorToWindow[owner] = newId;

    if (g_renderer)
    {
        g_renderer->CreateWindowSwapChain(*m_windowList[newId].m_window);
    }

    ShowWindow(hwnd, SW_SHOW);

    DebuggerPrintf("CreateWindowInternal: Created window %d '%s' for actor %llu.\n", newId, windowTitle.c_str(), static_cast<unsigned long long>(owner));
    return newId;
}

bool WindowSubsystem::AddEntityToWindow(WindowID windowID, EntityID entityID)
{
    auto windowIt = m_windowList.find(windowID);
    if (windowIt == m_windowList.end())
    {
        DebuggerPrintf("AddActorToWindow: Window %d not found.\n", windowID);
        return false;
    }

    auto actorIt = m_actorToWindow.find(entityID);
    if (actorIt != m_actorToWindow.end())
    {
        if (actorIt->second == windowID)
        {
            DebuggerPrintf("AddActorToWindow: Actor %d already in window %d.\n", entityID, windowID);
            return true;
        }

        DebuggerPrintf("AddActorToWindow: Actor %d already in window %d, cannot add to window %d.\n",
                       entityID, actorIt->second, windowID);
        return false;
    }

    windowIt->second.m_owners.insert(entityID);
    m_actorToWindow[entityID] = windowID;

    DebuggerPrintf("AddActorToWindow: Added Actor %d to Window %d.\n", entityID, windowID);
    return true;
}

bool WindowSubsystem::RemoveEntityFromWindow(WindowID windowID, EntityID entityID)
{
    auto windowIt = m_windowList.find(windowID);
    if (windowIt == m_windowList.end())
    {
        DebuggerPrintf("RemoveActorFromWindow: Window %d not found.\n", windowID);
        return false;
    }

    auto& owners  = windowIt->second.m_owners;
    auto  actorIt = owners.find(entityID);
    if (actorIt == owners.end())
    {
        DebuggerPrintf("RemoveActorFromWindow: Actor %d not in window %d.\n", entityID, windowID);
        return false;
    }

    owners.erase(actorIt);
    m_actorToWindow.erase(entityID);

    // Auto-destroy window when it has no remaining owners
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

    for (EntityID actorId : windowIt->second.m_owners)
    {
        m_actorToWindow.erase(actorId);
    }

    if (windowIt->second.m_window)
    {
        windowIt->second.m_window->Shutdown();
    }

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

void WindowSubsystem::ShowWindowByWindowID(WindowID const windowID)
{
    Window* window = GetValidatedWindow(windowID, "ShowWindowByWindowID");
    if (window)
    {
        ShowWindow(static_cast<HWND>(window->GetWindowHandle()), SW_SHOW);
    }
}

void WindowSubsystem::HideWindowByWindowID(WindowID const windowID)
{
    Window* window = GetValidatedWindow(windowID, "HideWindowByWindowID");
    if (window)
    {
        ShowWindow(static_cast<HWND>(window->GetWindowHandle()), SW_HIDE);
    }
}

//----------------------------------------------------------------------------------------------------
// Query functions
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
    return (it != m_actorToWindow.end()) ? it->second : INVALID_WINDOW_ID;
}

std::vector<EntityID> WindowSubsystem::GetWindowOwners(WindowID const windowID)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end())
    {
        auto const& owners = it->second.m_owners;
        return {owners.begin(), owners.end()};
    }
    return {};
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
    result.reserve(m_windowList.size());
    for (auto const& [windowId, windowData] : m_windowList)
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
        return it->second.m_owners.find(entityID) != it->second.m_owners.end();
    }
    return false;
}

bool WindowSubsystem::WindowExists(WindowID const windowID)
{
    return m_windowList.find(windowID) != m_windowList.end();
}

//----------------------------------------------------------------------------------------------------
// Window operations
//----------------------------------------------------------------------------------------------------

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID)
{
    Window* window = GetValidatedWindow(windowID, "UpdateWindowPosition");
    if (window)
    {
        window->UpdatePosition();
    }
}

void WindowSubsystem::UpdateWindowPosition(WindowID const windowID,
                                           Vec2 const&    newPosition)
{
    // DEPRECATED: This method name is misleading - it adds to position rather than setting it
    // Use MoveWindowByOffset() for clarity
    MoveWindowByOffset(windowID, newPosition);
}

void WindowSubsystem::MoveWindowByOffset(WindowID const windowID, Vec2 const& offset)
{
    Window* window = GetValidatedWindow(windowID, "MoveWindowByOffset");
    if (window)
    {
        Vec2 const oldPosition = window->GetWindowPosition();
        Vec2 const newPosition = oldPosition + offset;
        window->SetWindowPosition(newPosition);
        DebuggerPrintf("MoveWindowByOffset: Moved Window %d by (%f, %f), from (%f, %f) to (%f, %f)\n",
                       windowID, offset.x, offset.y,
                       oldPosition.x, oldPosition.y, newPosition.x, newPosition.y);
    }
}

void WindowSubsystem::UpdateWindowDimension(WindowID const windowID)
{
    Window* window = GetValidatedWindow(windowID, "UpdateWindowDimension");
    if (window)
    {
        window->UpdateDimension();
    }
}

void WindowSubsystem::SetWindowActive(WindowID windowID, bool active)
{
    auto it = m_windowList.find(windowID);
    if (it != m_windowList.end())
    {
        it->second.m_isActive = active;

        DebuggerPrintf("SetWindowActive: Window %d set to %s.\n", windowID, active ? "active" : "inactive");
    }
    else
    {
        DebuggerPrintf("SetWindowActive: Window %d not found.\n", windowID);
    }
}

void WindowSubsystem::SetWindowName(WindowID windowId, String const& name)
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
    for (auto const& [windowId, windowData] : m_windowList)
    {
        if (windowData.m_isActive)
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
    // Convert title to wide string
    std::wstring wTitle;
    wTitle.resize(title.size());
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), static_cast<int>(title.size()), wTitle.data(), static_cast<int>(wTitle.size()));

    // Register window class (only once)
    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc      = {};
        wc.lpfnWndProc   = reinterpret_cast<WNDPROC>(GetWindowLongPtr(static_cast<HWND>(Window::s_mainWindow->GetWindowHandle()), GWLP_WNDPROC));
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = L"ChildWindow";
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon         = static_cast<HICON>(LoadImage(
            nullptr,
            m_config.m_iconFilePath,
            IMAGE_ICON,
            32, 32,
            LR_LOADFROMFILE
        ));
        RegisterClass(&wc);
        classRegistered = true;
    }

    // Adjust window size so the client area matches the specified width and height
    RECT rect = {0, 0, width, height};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    int adjustedWidth  = rect.right - rect.left;
    int adjustedHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowEx(
        0,
        L"ChildWindow",
        wTitle.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        x, y, adjustedWidth, adjustedHeight,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    return hwnd;
}

void WindowSubsystem::InitializeWindowClientPosition(Window* window, HWND hwnd)
{
    if (!window || !hwnd) return;

    // Get client rectangle in client coordinates
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    // Convert client area top-left corner to screen coordinates
    POINT clientTopLeft = {0, 0};
    ClientToScreen(hwnd, &clientTopLeft);

    // Calculate client dimensions
    int const clientWidth  = clientRect.right - clientRect.left;
    int const clientHeight = clientRect.bottom - clientRect.top;

    // Get screen dimensions for Y-axis flip
    Vec2 const screenDimensions = Vec2(static_cast<float>(GetSystemMetrics(SM_CXSCREEN)),
                                       static_cast<float>(GetSystemMetrics(SM_CYSCREEN)));

    // Set client position (with Y-axis flip for engine coordinate system)
    Vec2 const clientPosition = Vec2(static_cast<float>(clientTopLeft.x),
                                     screenDimensions.y - static_cast<float>(clientTopLeft.y + clientHeight));

    window->SetClientPosition(clientPosition);
    window->SetClientDimensions(Vec2(static_cast<float>(clientWidth), static_cast<float>(clientHeight)));
}

Window* WindowSubsystem::GetValidatedWindow(WindowID const windowID, char const* callerName)
{
    auto it = m_windowList.find(windowID);
    if (it == m_windowList.end() || !it->second.m_window)
    {
        if (callerName)
        {
            DebuggerPrintf("%s: Window %d not found.\n", callerName, windowID);
        }
        return nullptr;
    }
    return it->second.m_window.get();
}

void WindowSubsystem::SetupTransparentMainWindow()
{
    if (!Window::s_mainWindow) return;

    HWND mainHwnd = static_cast<HWND>(Window::s_mainWindow->GetWindowHandle());

    // Set up fullscreen transparent main window
    SetWindowLong(mainHwnd, GWL_EXSTYLE,
                  GetWindowLong(mainHwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

    // Fully transparent, mouse click-through
    SetLayeredWindowAttributes(mainHwnd, 0, 0, LWA_ALPHA);

    // Set to fullscreen overlay
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
    // Collect completed animations for removal
    std::vector<WindowID> completedAnimations;

    for (auto& [windowId, animData] : m_windowAnimations)
    {
        if (animData.IsAnimating())
        {
            UpdateSingleWindowAnimation(windowId, animData, deltaSeconds);

            // Mark for removal if completed
            if (!animData.IsAnimating())
            {
                completedAnimations.push_back(windowId);
            }
        }
    }

    // Remove completed animations to prevent memory growth
    for (WindowID windowId : completedAnimations)
    {
        m_windowAnimations.erase(windowId);
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
        // Animation complete
        t                              = 1.0f;
        animData.m_isAnimatingSize     = false;
        animData.m_isAnimatingPosition = false;
    }

    // Use SmoothStep5 for smooth animation easing
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
