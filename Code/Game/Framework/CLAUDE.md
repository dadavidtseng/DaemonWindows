[Root Directory](../../../CLAUDE.md) > [Code](../../) > [Game](../) > **Framework**

# Framework Subsystem

## Module Responsibilities

The Framework subsystem serves as the foundational layer of the DaemonWindows application, providing essential infrastructure for:

- **Application Lifecycle Management**: Initialization, main loop execution, and cleanup
- **Windows Platform Integration**: Direct Windows API interaction and window creation
- **Global System Coordination**: Managing relationships between subsystems and engine components
- **Entry Point Management**: Providing the main Windows application entry point

## Entry and Startup

### Primary Entry Point
- **File**: `Main_Windows.cpp`
- **Function**: `WinMain()` - Standard Windows application entry point
- **Process Flow**:
  1. Creates global App instance (`g_app`)
  2. Calls `App::Startup()` for system initialization
  3. Executes `App::RunMainLoop()` for game execution
  4. Performs `App::Shutdown()` for cleanup
  5. Safe release of application resources

### Application Framework
- **File**: `App.cpp/hpp`
- **Core Functions**:
  - `Startup()`: Initializes all subsystems and engine components
  - `RunMainLoop()`: Main game loop with frame timing
  - `RunFrame()`: Single frame execution (BeginFrame → Update → Render → EndFrame)
  - `Shutdown()`: Clean shutdown of all systems

## External Interfaces

### System Events
```cpp
// Event handling for application lifecycle
static bool OnWindowClose(EventArgs& args);
static void RequestQuit();
```

### Global System Access
```cpp
// Framework provides access to core engine systems
extern App* g_app;
extern AudioSystem* g_audio;
extern Game* g_game;
extern Renderer* g_renderer;
extern WidgetSubsystem* g_widgetSubsystem;
extern WindowSubsystem* g_windowSubsystem;
```

## Key Dependencies and Configuration

### Engine Dependencies
- **Engine/Core/EngineCommon.hpp**: Core engine functionality
- **Engine/Platform/Window.hpp**: Window management abstractions
- **Engine/Core/EventSystem.hpp**: Event-driven communication system

### Windows Platform Dependencies  
- **windows.h**: Native Windows API access
- **WIN32_LEAN_AND_MEAN**: Optimized Windows header inclusion

### Build Configuration
- **Language Standard**: C++17
- **Character Set**: Unicode
- **Subsystem**: Windows (GUI application)
- **Platform Toolset**: v143 (Visual Studio 2022)

## Data Models

### Application State Management
```cpp
class App {
    static bool m_isQuitting;           // Application quit flag
    Camera* m_devConsoleCamera;         // Development console camera
};
```

### Global System Pointers
```cpp
// Defined in GameCommon.hpp
typedef uint32_t EntityID;              // Entity identification
typedef uint32_t WindowID;              // Window identification

// Global system instances
extern App* g_app;
extern Game* g_game;
extern Renderer* g_renderer;
// ... additional subsystem pointers
```

## Testing and Quality

### Current Testing Status
- **Unit Tests**: None implemented
- **Integration Testing**: Manual testing through application execution
- **Memory Management**: Custom `GAME_SAFE_RELEASE` template for safe pointer cleanup

### Quality Assurance Measures
- **Warning Level**: Level 4 (highest) in Visual Studio
- **SDL Check**: Enabled for security development lifecycle
- **Conformance Mode**: Strict C++ standard compliance
- **Debug Information**: Full debug info generation in all configurations

## FAQ

### Q: How does the Framework coordinate with the external Engine?
**A**: The Framework acts as a bridge between the game-specific code and the external Daemon Engine. It includes Engine headers and manages Engine subsystem lifecycles while keeping game logic separate.

### Q: Why use global pointers instead of dependency injection?
**A**: This follows a traditional game engine pattern for performance and simplicity. Global access to frequently-used systems avoids parameter passing overhead in performance-critical game loops.

### Q: How is the main loop structured?
**A**: The main loop follows a standard game pattern: BeginFrame() → Update() → Render() → EndFrame(), with proper timing and event handling integrated throughout.

## Related File List

### Source Files (*.cpp)
- `Main_Windows.cpp` - Windows application entry point
- `App.cpp` - Application framework implementation  
- `GameCommon.cpp` - Global system definitions and utilities

### Header Files (*.hpp)
- `App.hpp` - Application class interface
- `GameCommon.hpp` - Global system declarations and common types

### Configuration Files
- `../EngineBuildPreferences.hpp` - Engine build configuration for this game

## Changelog
- **2025-09-10**: Initial module documentation created, analyzed application lifecycle and Windows integration patterns