//----------------------------------------------------------------------------------------------------
// WindowSubsystem.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <unordered_map>

#include "Engine/Math/Vec2.hpp"
#include "Engine/Platform/Window.hpp"
#include "Game/Gameplay/Actor.hpp"

//----------------------------------------------------------------------------------------------------
class WindowSubsystem
{
public:
    void StartUp();
    void BeginFrame();
    void Update();
    void UpdatePositionByWindowID(WindowID id);
    void UpdateDimensionByWindowID(WindowID id);
    void Render();
    void EndFrame();
    void ShutDown();

    WindowID createWindow(ActorID owner);
    void     RenderWindows() ;
    void     UpdateWindows(std::vector<Window>& windows);
    void     CreateAndRegisterMultipleWindows(int windowCount, const std::vector<ActorID>& owners);
    HWND     CreateGameWindow(ActorID owner, const wchar_t* title, int x, int y, int width, int height);
    Window*  FindWindowByWindowID(WindowID id);
    // void     CreateAndRegisterMultipleWindows(int windowCount);
    // HWND     CreateGameWindow(const wchar_t* title, int x, int y, int width, int height);
    // void     destroyWindow(WindowID id);
    // void     updateWindowPosition(WindowID id, Vec2 position);
    // void     updateWindowSize(WindowID id, Vec2 size);
    // Window*  getWindow(WindowID id);
    // WindowID getWindowForEntity(ActorID entityId);

private:
    std::unordered_map<WindowID, std::unique_ptr<Window>> m_windowList;
    std::unordered_map<ActorID, WindowID>                 actorWindowMapping;
    WindowID                                                   m_nextWindowID = 0;
};
