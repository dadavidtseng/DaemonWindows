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

using WindowID = uint32_t;

//----------------------------------------------------------------------------------------------------
class WindowSubsystem
{
public:
    void StartUp();
    void Update();
    void Render();
    void ShutDown();

    WindowID createWindow(ActorID owner);
    void     destroyWindow(WindowID id);
    void     updateWindowPosition(WindowID id, Vec2 position);
    void     updateWindowSize(WindowID id, Vec2 size);
    Window*  getWindow(WindowID id);
    WindowID getWindowForEntity(ActorID entityId);

private:
    std::unordered_map<WindowID, std::unique_ptr<Window>> windows;
    std::unordered_map<ActorID, WindowID>                 entityWindowMapping;
};
