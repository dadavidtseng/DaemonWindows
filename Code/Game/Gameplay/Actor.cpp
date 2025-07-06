//----------------------------------------------------------------------------------------------------
// Actor.cpp
//----------------------------------------------------------------------------------------------------

#include "Game/Gameplay/Actor.hpp"

void Actor::requestWindow(WindowSubsystem& winSystem) {
    if (hasWindow()) {
        return; // 已經有視窗了
    }

    WindowID newWindowId = winSystem.createWindow(id);
    if (newWindowId != 0) {
        windowId = newWindowId;

        // 設定初始視窗位置（基於Actor的位置）
        if (Window* window = winSystem.getWindow(newWindowId)) {
            // 假設Actor有getPosition()方法

            winSystem.updateWindowPosition(newWindowId, m_position);

            // 可以根據Actor類型設定不同的視窗大小
            Vec2 windowSize = getDefaultWindowSize();
            winSystem.updateWindowSize(newWindowId, windowSize);
        }
    }
}

void Actor::releaseWindow(WindowSubsystem& winSystem) {
    if (!hasWindow()) {
        return; // 沒有視窗可以釋放
    }

    winSystem.destroyWindow(windowId.value());
    windowId.reset();
}

// 在Actor中可以加入這個輔助方法
Vec2 Actor::getDefaultWindowSize() const {
    // 可以根據Actor類型返回不同的預設大小
    return Vec2(400.0f, 300.0f);
}