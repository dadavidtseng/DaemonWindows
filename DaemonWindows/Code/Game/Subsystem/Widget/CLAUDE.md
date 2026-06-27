[Root Directory](../../../../CLAUDE.md) > [Code](../../../) > [Game](../../) > [Subsystem](../) > **Widget**

# Widget Subsystem

**MIGRATION NOTICE**: As of 2025-09-30, the core Widget system (IWidget and WidgetSubsystem) has been migrated to `Engine/UI/`. This directory now contains only game-specific widget implementations.

## Module Responsibilities

The Widget directory now provides game-specific UI widget implementations for DaemonWindows:

- **ButtonWidget**: Text-based button widget used for player health/coin displays
- Game-specific widget customizations that use global rendering systems

## Architecture Changes

### What Moved to Engine
- **IWidget.hpp/cpp** → `Engine/UI/IWidget.hpp/cpp` - Abstract widget base class
- **WidgetSubsystem.hpp/cpp** → `Engine/UI/WidgetSubsystem.hpp/cpp` - Widget management system

### Key Refactoring Changes
1. **Owner Type Changed**: `void*` → `uint64_t` for type-safe owner IDs
2. **Include Paths Updated**: All files now use `#include "Engine/UI/WidgetSubsystem.hpp"`
3. **English Documentation**: All Engine code now has English comments

### What Stayed in Game Layer
- **ButtonWidget.hpp/cpp** - Game-specific button implementation using global `g_renderer` and `g_bitmapFont`

## Entry and Startup

### Widget Subsystem (Engine Level)
- **Location**: `Engine/UI/WidgetSubsystem.cpp`
- **Global Access**: `g_widgetSubsystem` defined in `GameCommon.hpp`
- **Initialization**: Created in `App::Startup()`
- **Lifecycle**: StartUp() → BeginFrame() → Update() → Render() → EndFrame() → ShutDown()

### ButtonWidget (Game Level)
- **File**: `ButtonWidget.cpp`
- **Usage**: Player health/coin displays
- **Creation**: Via `g_widgetSubsystem->CreateWidget<ButtonWidget>(...)`

## External Interfaces

### Engine Widget Management API (Engine/UI/WidgetSubsystem.hpp)
```cpp
class WidgetSubsystem {
    // Widget lifecycle
    void AddWidget(WidgetPtr const& widget, int zOrder = 0);
    void AddWidgetToOwner(WidgetPtr const& widget, uint64_t ownerID, int zOrder = 0);  // Changed from void*
    void RemoveWidget(WidgetPtr const& widget);
    void RemoveAllWidgetsFromOwner(uint64_t ownerID);  // Changed from void*
    void RemoveAllWidgets();

    // Widget queries
    WidgetPtr FindWidgetByName(String const& name) const;
    std::vector<WidgetPtr> GetWidgetsByOwner(uint64_t ownerID) const;  // Changed from void*
    std::vector<WidgetPtr> GetAllWidgets() const;

    // Template widget creation
    template <typename T, typename... Args>
    std::shared_ptr<T> CreateWidget(Args&&... args);
};
```

### Game-Specific ButtonWidget Interface (Game/Subsystem/Widget/ButtonWidget.hpp)
```cpp
class ButtonWidget : public IWidget {
    ButtonWidget(WidgetSubsystem* owner, String const& text, int x, int y,
                 int width, int height, Rgba8 const& color);

    void Draw() const override;
    void Update() override;

    void SetText(String const& text);
    void SetPosition(Vec2 const& newPosition);
    void SetDimensions(Vec2 const& newDimensions);
};
```

## Key Dependencies and Configuration

### Engine Dependencies (for WidgetSubsystem)
- **Engine/Core/StringUtils.hpp**: String manipulation utilities for widget names
- **Engine/UI/IWidget.hpp**: Abstract widget base class (now in Engine)

### Game Layer Dependencies (for ButtonWidget)
- **Engine/UI/IWidget.hpp**: Base widget interface
- **Engine/Renderer/Renderer.hpp**: Global `g_renderer` for drawing
- **Engine/Renderer/BitmapFont.hpp**: Global `g_bitmapFont` for text rendering
- **Game/Framework/GameCommon.hpp**: Global system access

### Memory Management
- **Smart Pointers**: Uses `std::shared_ptr<IWidget>` for automatic memory management
- **Type Alias**: `using WidgetPtr = std::shared_ptr<IWidget>;` defined in Engine

## Migration Impact

### Breaking Changes for Game Code
1. **Owner IDs**: Change from `void*` to `uint64_t`
   - Old: `AddWidgetToEntity(widget, entityPtr, zOrder)`
   - New: `AddWidgetToOwner(widget, entityID, zOrder)`

2. **Include Paths**: Update all widget includes
   - Old: `#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"`
   - New: `#include "Engine/UI/WidgetSubsystem.hpp"`

3. **Method Names**: Some methods renamed for clarity
   - `AddWidgetToEntity()` → `AddWidgetToOwner()`
   - `RemoveAllWidgetsFromEntity()` → `RemoveAllWidgetsFromOwner()`

### Backward Compatibility
- ButtonWidget constructor signature unchanged (keeps `WidgetSubsystem*` parameter for compatibility)
- All visual behavior preserved - no rendering changes
- Same z-order sorting and garbage collection behavior

## Testing and Quality

### Current Testing Status
- **Unit Tests**: None implemented
- **Integration Testing**: Manual testing through gameplay UI interactions
- **Widget Lifecycle Testing**: Implicit testing through entity creation/destruction

### Quality Assurance Features
- **Type Safety**: Template-based creation with compile-time type checking
- **Memory Safety**: Smart pointer-based automatic memory management
- **Owner Cleanup**: Automatic widget removal when entities are destroyed
- **Viewport Management**: Special handling for viewport widgets in multi-window scenarios

### Performance Considerations
- **Lazy Sorting**: Z-order sorting only performed when needed
- **Capacity Pre-allocation**: Initial capacity settings to minimize vector reallocations
- **Owner Mapping**: Efficient entity-to-widgets lookup using unordered_map

## FAQ

### Q: How do widgets integrate with the multi-window system?
**A**: Widgets can be owned by entities that exist in specific windows. The WidgetSubsystem tracks these relationships and can manage widget visibility and positioning based on window focus and entity location.

### Q: What is the viewport widget used for?
**A**: The viewport widget is a special UI component that handles multi-window viewport management, likely providing a unified view or control interface across multiple game windows.

### Q: How does widget cleanup work when entities are destroyed?
**A**: The `RemoveAllWidgetsFromEntity()` method allows complete cleanup of all widgets associated with a specific entity, ensuring no orphaned UI elements remain when game objects are destroyed.

### Q: Can widgets be shared between multiple entities?
**A**: The current design supports one-to-many relationships (one entity can own multiple widgets), but widget sharing between entities would require careful ownership management through the pointer system.

## Related File List

### Engine-Level Widget System (Migrated)
- `Engine/UI/WidgetSubsystem.cpp/hpp` - Core widget management system (migrated from Game)
- `Engine/UI/IWidget.cpp/hpp` - Abstract widget base class (migrated from Game)

### Game-Level Widget Implementations (Stayed)
- `ButtonWidget.cpp/hpp` - Text button widget for Player UI (health/coin displays)

### Integration Points
- Referenced in `Game/Framework/GameCommon.hpp` as global `g_widgetSubsystem`
- Used by `Game/Gameplay/Player.hpp` for health and coin display widgets
- Created in `Game/Framework/App.cpp` during application startup

## Changelog
- **2025-09-30**: Migrated IWidget and WidgetSubsystem to Engine/UI/, refactored void* to uint64_t owner IDs, updated all includes
- **2025-09-10**: Initial module documentation created, analyzed widget lifecycle management, template-based creation system, and entity integration patterns