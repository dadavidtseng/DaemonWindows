//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

void WindowSubsystem::update(float deltaTime)
{
    // 更新所有視窗動畫
    for (auto& [id, window] : windows)
    {
        window->updateAnimation(deltaTime);
    }

    // 處理視窗間的互動
    handleWindowCollisions();

    // 清理標記為銷毀的視窗
    cleanupDestroyedWindows();
}

// 視窗池優化
WindowID WindowSubsystem::createWindow(EntityID owner)
{
    if (!windowPool.empty())
    {
        // 重用現有視窗
        WindowID id = windowPool.back();
        windowPool.pop_back();
        windows[id]->reset();
        entityWindowMapping[owner] = id;
        return id;
    }
    // 創建新視窗...
}
