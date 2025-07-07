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
    void Render();
    void EndFrame();
    void ShutDown();

    WindowID createWindow(ActorID owner);
    void     RenderWindows() ;
    void     UpdateWindows(std::vector<Window>& windows);
    void     UpdateWindows(std::vector<Window>& windows) const;
    void     CreateAndRegisterMultipleWindows(std::vector<Window>& windows, int windowCount);
    HWND CreateGameWindow(const wchar_t* title, int x, int y, int width, int height);
    // void     destroyWindow(WindowID id);
    // void     updateWindowPosition(WindowID id, Vec2 position);
    // void     updateWindowSize(WindowID id, Vec2 size);
    // Window*  getWindow(WindowID id);
    // WindowID getWindowForEntity(ActorID entityId);

private:
    // std::unordered_map<WindowID, std::unique_ptr<Window>> m_windowList;
    std::vector<Window> m_windowList;
    std::unordered_map<ActorID, WindowID>                 actorWindowMapping;
    WindowID                                                   m_nextWindowID = 0;
};
