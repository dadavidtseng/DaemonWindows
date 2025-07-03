//----------------------------------------------------------------------------------------------------
// WindowSubsystem.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <memory>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------
class WindowSubsystem
{
public:
    void StartUp();
    void Update();
    void Render();
    void ShutDown();

    WindowID createWindow(EntityID owner);
    void     destroyWindow(WindowID id);
    void     updateWindowPosition(WindowID id, Vec2 position);
    void     updateWindowSize(WindowID id, Vec2 size);
    Window*  getWindow(WindowID id);
    WindowID getWindowForEntity(EntityID entityId);

private:
    std::unordered_map<WindowID, std::unique_ptr<Window>> windows;
    std::unordered_map<EntityID, WindowID>                entityWindowMapping;
};
