//----------------------------------------------------------------------------------------------------
// WindowSubsystem.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

void WindowSubsystem::StartUp()
{
}

void WindowSubsystem::Update()
{
}

void WindowSubsystem::Render()
{
}

void WindowSubsystem::ShutDown()
{
}

WindowID WindowSubsystem::createWindow(ActorID owner) {
    // 檢查實體是否已經擁有視窗
    auto it = entityWindowMapping.find(owner);
    if (it != entityWindowMapping.end()) {
        // 實體已經有視窗了，返回現有的
        return it->second;
    }

    // 生成新的視窗ID
    static WindowID nextId = 1;
    WindowID newId = nextId++;

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

void WindowSubsystem::destroyWindow(WindowID id) {
    auto windowIt = windows.find(id);
    if (windowIt == windows.end()) {
        return; // 視窗不存在
    }

    // 找到擁有者並移除映射
    // EntityID owner = windowIt->second->getOwnerId();
    // entityWindowMapping.erase(owner);

    // 移除視窗
    windows.erase(windowIt);
}

void WindowSubsystem::updateWindowPosition(WindowID id, Vec2 position) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        // it->second->setPosition(position);
    }
}

void WindowSubsystem::updateWindowSize(WindowID id, Vec2 size) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        // 可以加入最小/最大尺寸限制
        Vec2 clampedSize;
        // clampedSize.x = std::max(100.0f, std::min(1920.0f, size.x));
        // clampedSize.y = std::max(100.0f, std::min(1080.0f, size.y));

        // it->second->setSize(clampedSize);
    }
}

Window* WindowSubsystem::getWindow(WindowID id) {
    auto it = windows.find(id);
    if (it != windows.end()) {
        return it->second.get();
    }
    return nullptr;
}

WindowID WindowSubsystem::getWindowForEntity(ActorID entityId) {
    auto it = entityWindowMapping.find(entityId);
    if (it != entityWindowMapping.end()) {
        return it->second;
    }
    return 0; // 0 表示無效的WindowID
}