[Root Directory](../../../../CLAUDE.md) > [Code](../../../) > [Game](../../) > [Subsystem](../) > **Window**

# Window Subsystem

## Module Responsibilities

The Window Subsystem is the core innovation of DaemonWindows, providing sophisticated multi-window management capabilities:

- **Dynamic Window Creation**: Runtime creation and management of child game windows
- **Entity-Window Association**: Tracking which entities belong to which windows
- **Window Animation System**: Smooth position and dimension transitions with timing control
- **Cross-Window Entity Management**: Supporting entity movement between windows
- **Window State Management**: Visibility, focus, and activity state control
- **OS Integration**: Direct Windows API interaction for advanced window manipulation

## Entry and Startup

### Primary Entry Point
- **File**: `WindowSubsystem.cpp`
- **Class**: `WindowSubsystem` - Central multi-window management system
- **Initialization**: Created and managed through global `g_windowSubsystem` pointer
- **Lifecycle**: StartUp() → BeginFrame() → Update() → Render() → EndFrame() → ShutDown()

### Configuration
```cpp
struct sWindowSubsystemConfig {
    wchar_t const* m_iconFilePath = nullptr;  // Window icon configuration
};
```

## External Interfaces

### Window Management API
```cpp
class WindowSubsystem {
    // Core window lifecycle
    WindowID CreateChildWindow(EntityID owner, String const& windowTitle, 
                              int x, int y, int width, int height);
    void DestroyWindow(WindowID windowID);
    void DestroyAllWindows();
    
    // Entity-window relationships
    bool AddEntityToWindow(WindowID windowID, EntityID entityID);
    bool RemoveEntityFromWindow(WindowID windowID, EntityID entityID);
    void RemoveEntityFromMappings(EntityID entityID);
    
    // Window state control
    void ShowWindowByWindowID(WindowID windowID);
    void HideWindowByWindowID(WindowID windowID);
    void SetWindowActive(WindowID windowID, bool active);
};
```

### Window Query API
```cpp
class WindowSubsystem {
    // Window and entity lookups
    Window* GetWindow(WindowID windowID);
    WindowData* GetWindowData(WindowID windowID);
    WindowID FindWindowIDByEntityID(EntityID entityID);
    std::vector<EntityID> GetWindowOwners(WindowID windowID);
    std::vector<WindowID> GetActorWindows(EntityID entityID);
    std::vector<WindowID> GetAllWindowIDs();
    
    // Relationship queries
    bool IsActorInWindow(WindowID windowID, EntityID entityID);
    bool WindowExists(WindowID windowID);
};
```

### Window Animation API
```cpp
class WindowSubsystem {
    // Smooth window transitions
    void AnimateWindowDimensions(WindowID id, Vec2 const& targetDimensions, float duration = 0.5f);
    void AnimateWindowPosition(WindowID id, Vec2 const& targetPosition, float duration = 0.5f);
    void AnimateWindowPositionAndDimensions(WindowID id, Vec2 const& targetPosition, 
                                           Vec2 const& targetDimensions, float duration = 0.5f);
    bool IsWindowAnimating(WindowID id) const;
};
```

## Key Dependencies and Configuration

### Engine Dependencies
- **Engine/Platform/Window.hpp**: Platform-abstracted window management
- **Game/Gameplay/Entity.hpp**: Entity system integration for ownership tracking

### Windows API Integration
- **Native Window Creation**: Direct OS window creation through `CreateOSWindow()`
- **Transparent Main Window**: Special transparent window setup for overlay effects
- **Window Handle Management**: HWND management for low-level window control

### Entity System Integration
```cpp
// Type definitions for cross-system communication
typedef uint32_t EntityID;  // Entity identification
typedef uint32_t WindowID;  // Window identification
```

## Data Models

### Window Data Structure
```cpp
struct WindowData {
    std::unique_ptr<Window> m_window;              // Platform window wrapper
    std::unordered_set<EntityID> m_owners;         // Entities in this window
    String m_name;                                 // Window display name
    bool m_isActive = true;                        // Window activity state
};
```

### Window Animation Data
```cpp
struct WindowAnimationData {
    bool m_isAnimatingSize = false;                // Size animation active
    bool m_isAnimatingPosition = false;            // Position animation active
    
    Vec2 m_targetWindowDimensions = Vec2::ZERO;    // Target size
    Vec2 m_startWindowDimensions = Vec2::ZERO;     // Starting size
    Vec2 m_targetWindowPosition = Vec2::ZERO;      // Target position
    Vec2 m_startWindowPosition = Vec2::ZERO;       // Starting position
    
    float m_animationDuration = 0.5f;              // Animation length
    float m_animationTimer = 0.0f;                 // Current progress
    
    bool IsAnimating() const;                       // Animation status check
};
```

### Core Data Structures
```cpp
class WindowSubsystem {
private:
    // Primary data storage
    std::unordered_map<WindowID, WindowData> m_windowList;
    
    // Entity-window relationship tracking  
    std::unordered_map<EntityID, WindowID> m_actorToWindow;
    
    // Animation management
    std::unordered_map<WindowID, WindowAnimationData> m_windowAnimations;
    
    WindowID m_nextWindowID = 1;  // ID generation (0 reserved for invalid)
};
```

## Testing and Quality

### Current Testing Status
- **Unit Tests**: None implemented
- **Integration Testing**: Manual multi-window gameplay testing
- **Performance Testing**: Window creation/destruction stress testing
- **Platform Testing**: Windows 10/11 compatibility verification

### Quality Assurance Features
- **Resource Management**: Smart pointers for automatic window cleanup
- **ID Generation**: Safe WindowID generation with invalid ID handling
- **Relationship Integrity**: Bidirectional entity-window relationship tracking
- **Animation Safety**: Animation state validation and proper cleanup

### Performance Considerations
- **Efficient Lookups**: Hash-based maps for O(1) window and entity lookups
- **Animation Optimization**: Per-frame animation updates with early termination
- **Memory Efficiency**: Unordered sets for entity ownership with minimal overhead

### Known Technical Challenges
- **Window Focus Management**: Complex focus handling across multiple game windows
- **Cross-Window Collision**: Entity collision detection spanning multiple windows  
- **Animation Synchronization**: Coordinating multiple simultaneous window animations
- **Memory Management**: Proper cleanup when entities move between windows

## FAQ

### Q: How does entity movement between windows work?
**A**: Entities have a `WindowID` that tracks their current window. The subsystem provides `RemoveEntityFromWindow()` and `AddEntityToWindow()` to transfer entities, updating both the entity's window association and the window's owner set.

### Q: What makes the window animation system unique?
**A**: The animation system provides smooth interpolation between window states with customizable duration. It supports simultaneous position and dimension changes, which is crucial for the dynamic window management that makes this game innovative.

### Q: How does the transparent main window work?
**A**: The `SetupTransparentMainWindow()` function creates a transparent overlay window that likely serves as a coordinator for multiple child windows, enabling seamless visual transitions and unified input handling.

### Q: Why use unordered_map for window storage?
**A**: Hash-based storage provides O(1) lookup performance for window access by ID, which is critical since window operations occur frequently during gameplay and need to be performant.

## Related File List

### Core Window System
- `WindowSubsystem.cpp/hpp` - Complete multi-window management implementation

### Integration Points
- Referenced in `Game/Framework/GameCommon.hpp` as global `g_windowSubsystem`
- Used extensively by `Game/Gameplay/Entity.hpp` for window association
- Integrated with `Game/Gameplay/Player.hpp` for player window movement

### Engine Dependencies
- Depends on external `Engine/Platform/Window.hpp` for platform abstraction
- Integrates with Engine's event system for window lifecycle events

## Changelog
- **2025-09-10**: Initial module documentation created, analyzed multi-window architecture, entity-window relationships, and animation system capabilities