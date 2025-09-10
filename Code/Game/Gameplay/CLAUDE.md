[Root Directory](../../../CLAUDE.md) > [Code](../../) > [Game](../) > **Gameplay**

# Gameplay System

## Module Responsibilities

The Gameplay system implements the core game logic and mechanics for the DaemonWindows multi-window action game:

- **Game State Management**: Controls transitions between Attract, Game, and Shop modes
- **Entity System**: Base entity framework with cross-window capabilities
- **Player Mechanics**: Player movement, shooting, health, and currency management  
- **Combat System**: Bullet physics, collision detection, and damage resolution
- **Enemy System**: Various enemy types (Circle, Triangle, Octagon) with AI behaviors
- **Shop System**: Currency-based upgrade purchasing (speed, health, max health)
- **Spawning System**: Timed enemy and item generation with progressive difficulty

## Entry and Startup

### Primary Entry Point
- **File**: `Game.cpp`
- **Class**: `Game` - Central game state controller
- **Initialization**: Called from `App::Startup()` through global `g_game` pointer

### Game State Machine
```cpp
enum class eGameState : int8_t {
    ATTRACT,    // Main menu/title screen
    GAME,       // Active gameplay
    SHOP        // Upgrade purchase interface
};
```

## External Interfaces

### Game State Management API
```cpp
class Game {
    eGameState GetCurrentGameState() const;
    void ChangeGameState(eGameState newGameState);
    
    // Entity management
    Entity* GetEntityByEntityID(EntityID const& entityID) const;
    std::vector<Entity*> m_entities;
    
    // System access
    Clock* GetGameClock() const;
    Player* GetPlayer() const;
    Shop* GetShop() const;
};
```

### Entity Base Interface
```cpp
class Entity {
    virtual void Update(float deltaSeconds);
    virtual void Render() const = 0;
    virtual void UpdateFromInput(float deltaSeconds) = 0;
    
    // State management
    virtual void MarkAsDead();
    virtual void MarkAsGarbage();
    virtual void MarkAsChildWindowInvisible();
    virtual void MarkAsChildWindowVisible();
};
```

### Player-Specific Interface
```cpp
class Player : public Entity {
    void FireBullet();
    void UpdateWindowFocus();
    
    // UI integration
    std::shared_ptr<ButtonWidget> m_healthWidget;
    std::shared_ptr<ButtonWidget> m_coinWidget;
};
```

## Key Dependencies and Configuration

### Engine Dependencies
- **Engine/Audio/AudioSystem.hpp**: Sound effects and music playback
- **Engine/Core/EventSystem.hpp**: Inter-system communication
- **Engine/Core/Timer.hpp**: Timing and animation systems

### Internal Dependencies
- **Game/Framework/GameCommon.hpp**: Global system access and type definitions
- **Game/Subsystem/Window/WindowSubsystem.hpp**: Multi-window entity management
- **Game/Subsystem/Widget/WidgetSubsystem.hpp**: UI component integration

### Audio Configuration
```cpp
// Game audio playback IDs
SoundPlaybackID m_attractPlaybackID;  // Menu background music
SoundPlaybackID m_ingamePlaybackID;   // Gameplay background music
```

### Spawn Configuration
```cpp
// Entity spawning parameters
float m_spawnTimer = 0.0f;      // Accumulation timer
float m_spawnInterval = 10.0f;   // 10-second spawn intervals
static int s_nextEntityID;       // Unique ID generation
```

## Data Models

### Core Entity Data
```cpp
class Entity {
    EntityID m_entityID;           // Unique identifier
    WindowID m_windowID;           // Associated window
    String m_name;                 // Debug/display name
    Vec2 m_position;               // World position
    Vec2 m_velocity;               // Movement vector
    Rgba8 m_color;                 // Render color
    int m_health;                  // Hit points
    int m_coinToDrop;              // Currency reward
    float m_orientationDegrees;    // Rotation
    float m_physicRadius;          // Collision radius
    float m_cosmeticRadius;        // Visual radius
    float m_thickness;             // Render thickness
    float m_speed;                 // Movement speed
};
```

### Player-Specific Data
```cpp
class Player : public Entity {
    int m_maxHealth;               // Maximum hit points
    int m_coin;                    // Currency amount (starts at 50)
    Timer m_bulletFireTimer;       // Bullet firing rate limiter
    
    // UI widgets for status display
    std::shared_ptr<ButtonWidget> m_healthWidget;
    std::shared_ptr<ButtonWidget> m_coinWidget;
};
```

### Entity Types
- **Player**: Controllable character with multi-window movement
- **Bullet**: Projectiles with physics and collision
- **Circle**: Basic circular enemy type
- **Triangle**: Triangular enemy variant  
- **Octagon**: Complex octagonal enemy
- **Coin**: Collectible currency items
- **Debris**: Environmental objects and particle effects

## Testing and Quality

### Current Testing Status
- **Unit Tests**: None implemented
- **Integration Testing**: Manual gameplay testing across multiple windows
- **Performance Testing**: Entity count stress testing with multi-window rendering

### Quality Assurance Features
- **Entity Lifecycle Management**: Proper cleanup with dead/garbage marking system
- **Memory Safety**: Entity destruction through event system
- **State Validation**: Game state transition validation through event handling

### Known Issues & Considerations
- **Cross-Window Collision**: Complex collision detection across multiple windows
- **Entity Synchronization**: Ensuring entity consistency when moving between windows
- **Performance Scaling**: Frame rate optimization with high entity counts

## FAQ

### Q: How do entities move between windows?
**A**: Entities have a `WindowID` that associates them with specific windows managed by the WindowSubsystem. When entities move, their window association can be updated through the WindowSubsystem API.

### Q: How does the spawning system work?
**A**: The Game class maintains a spawn timer that increments each frame. When it exceeds the spawn interval (10 seconds), new entities are created and added to the entity vector.

### Q: What is the entity lifecycle?
**A**: Entities follow a lifecycle: Creation → Active Updates → Death Marking → Garbage Marking → Cleanup. The cleanup is handled through the event system to ensure proper multi-system coordination.

### Q: How does the shop system integrate?
**A**: The Shop is a specialized entity that provides upgrade purchasing. It integrates with the Player's coin system and can modify player attributes (speed, health, max health).

## Related File List

### Core Game Logic
- `Game.cpp/hpp` - Main game controller and state machine
- `Entity.cpp/hpp` - Base entity class with cross-window support

### Player System
- `Player.cpp/hpp` - Player entity with input handling and UI integration

### Combat System  
- `Bullet.cpp/hpp` - Projectile physics and collision

### Enemy Types
- `Circle.cpp/hpp` - Basic circular enemy
- `Triangle.cpp/hpp` - Triangular enemy variant
- `Octagon.cpp/hpp` - Complex octagonal enemy

### Items & Environment
- `Coin.cpp/hpp` - Collectible currency system
- `Debris.cpp/hpp` - Environmental objects and effects

### Progression System
- `Shop.cpp/hpp` - Upgrade purchasing and player enhancement

## Changelog
- **2025-09-10**: Initial module documentation created, analyzed game state machine, entity system, and multi-window gameplay mechanics