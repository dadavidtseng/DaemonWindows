//----------------------------------------------------------------------------------------------------
// Actor.hpp
//----------------------------------------------------------------------------------------------------

#pragma once
#include <cstdint>
#include <optional>

#include "Game/Subsystem/Window/WindowSubsystem.hpp"

typedef uint32_t ActorID;

class Actor
{
protected:
    ActorID                 id;
    std::optional<WindowID> windowId;

public:
    bool hasWindow() const { return windowId.has_value(); }
    void requestWindow(WindowSubsystem& winSystem);
    void releaseWindow(WindowSubsystem& winSystem);
    Vec2 getDefaultWindowSize() const;

    Vec2 m_position = Vec2::ZERO;
};
