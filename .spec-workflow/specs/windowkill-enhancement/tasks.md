# Tasks Document - Windowkill Enhancement

## Project Overview

This tasks document breaks down the Windowkill Enhancement project into 6 main sprints (12 weeks, 24 hours per sprint). Each sprint represents a 2-week milestone aligned with the DFS2 course timeline.

**Timeline:** 12 weeks (6 sprints × 2 weeks)
**Effort:** 12 hours per week = 24 hours per sprint
**Total Estimated Hours:** 144-216 hours (target: 144 hours)

## Sprint Overview

| Sprint | Weeks | Focus | Tasks | Est. Hours |
|--------|-------|-------|-------|------------|
| Sprint 1 | 1-2 | Foundation (compilation fixes, refactoring, manager skeletons) | 13 | 20.5-31.0 |
| Sprint 2 | 3-4 | Tier 1 Enemies (6 enemy types) | 12 | 23.5-35.5 |
| Sprint 3 | 5-6 | Tier 2 Enemies & Wave System (3 elite enemies + wave logic) | 11 | 25.5-36.5 |
| Sprint 4 | 7-8 | Combat Upgrades (7 upgrades + shop/player/bullet extensions) | 11 | 25.0-36.0 |
| Sprint 5 | 9-10 | Audio & Visual Polish (AudioManager + ParticleSystem) | 12 | 25.5-37.0 |
| Sprint 6 | 11-12 | Game Over & Polish (game over system + final testing) | 12 | 28.5-40.0 |

---

# Sprint 1: Foundation (Weeks 1-2, 24 hours)

## Overview
Sprint 1 focuses on fixing compilation errors, code refactoring, and creating the foundation for wave and upgrade systems.

## Tasks

- [x] 1.1. Fix OpenSSL linking errors in Game.vcxproj
  - Files: Code/Game/Game.vcxproj
  - Add OpenSSL library path to AdditionalLibraryDirectories
  - Add libcrypto.lib and libssl.lib to AdditionalDependencies
  - Purpose: Resolve 26 unresolved external symbol errors
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: C:\\GitHub\\Protogame2D\\Code\\Game\\Game.vcxproj (reference pattern)_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Build Engineer with expertise in Visual Studio project configuration and C++ linking | Task: Fix OpenSSL linking errors by adding library paths and dependencies to Game.vcxproj following Requirement 1, referencing the working configuration from Protogame2D project | Restrictions: Do not modify Engine project files, maintain existing library order, ensure both Debug and Release configurations are updated | Success: Project compiles without LNK2019 errors for OpenSSL functions, both Debug and Release builds succeed, no new linking errors introduced | Instructions: After completing this task, use log-implementation tool to record what was changed (vcxproj modifications, library paths added), then mark task as [x] in tasks.md_

- [x] 1.2. Fix g_widgetSubsystem duplicate symbol error
  - Files: Code/Game/Framework/GameCommon.hpp, Code/Game/Framework/App.cpp
  - Verify extern declaration in GameCommon.hpp
  - Verify single definition in App.cpp
  - Check Engine's EngineCommon.cpp for duplicate definition
  - Purpose: Resolve LNK2005 duplicate symbol error
  - Time Estimate: [0.5, 1.5] hours
  - _Leverage: C:\\GitHub\\Protogame2D project structure (reference pattern)_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in linkage and symbol resolution | Task: Fix g_widgetSubsystem duplicate symbol error by ensuring proper extern declaration pattern following Requirement 1, referencing Protogame2D's working pattern | Restrictions: Do not remove g_widgetSubsystem usage, maintain existing code functionality, only fix declaration/definition pattern | Success: Project compiles without LNK2005 error, g_widgetSubsystem is accessible throughout codebase, no runtime errors related to widget subsystem | Instructions: After completing this task, use log-implementation tool to record the fix (extern pattern used, files modified), then mark task as [x] in tasks.md_

- [x] 1.3. Verify compilation success and test build
  - Files: N/A (build verification)
  - Build project in Debug configuration
  - Build project in Release configuration
  - Run executable to verify no missing DLL errors
  - Purpose: Confirm all compilation errors are resolved
  - Time Estimate: [0.5, 1.0] hours
  - _Leverage: Visual Studio build system_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in build verification and testing | Task: Verify compilation success by building both Debug and Release configurations and testing executable launch following Requirement 1 | Restrictions: Do not modify code during verification, document any remaining issues, ensure clean build from scratch | Success: Both Debug and Release builds complete without errors or warnings, executable launches successfully, no missing DLL errors, game reaches attract mode | Instructions: After completing this task, use log-implementation tool to record build verification results (configurations tested, any issues found), then mark task as [x] in tasks.md_

- [x] 1.4. Code refactoring - Extract collision handling logic
  - Files: Code/Game/Gameplay/Game.cpp, Code/Game/Gameplay/CollisionUtils.cpp/hpp (new)
  - Extract collision detection logic from Game::HandleEntityCollision()
  - Create CollisionUtils namespace with helper functions
  - Improve code readability and organization
  - Purpose: Clean up Game.cpp and make collision logic reusable
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing DoDiscsOverlap2D function_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Software Engineer with expertise in code refactoring and C++ best practices | Task: Extract collision handling logic from Game.cpp into reusable CollisionUtils following Requirement 2, improving code organization and readability | Restrictions: Do not change collision behavior, maintain existing collision detection accuracy, ensure all entity types still collide correctly | Success: Collision logic is extracted into CollisionUtils namespace, Game.cpp is cleaner and more readable, all existing collisions still work correctly, no regression in collision detection | Instructions: After completing this task, use log-implementation tool to record refactoring details (functions extracted, new files created, collision logic preserved), then mark task as [x] in tasks.md_

- [x] 1.5. Code refactoring - Improve entity spawning organization
  - Files: Code/Game/Gameplay/Game.cpp
  - Organize SpawnEntity() method with better structure
  - Add comments explaining spawn logic
  - Prepare for wave-based spawning integration
  - Purpose: Make spawn code more maintainable and extensible
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing SpawnEntity() method_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Software Engineer with expertise in code organization and documentation | Task: Refactor SpawnEntity() method for better organization and prepare for wave-based spawning following Requirement 2 | Restrictions: Do not change spawning behavior yet, maintain existing spawn functionality, only improve organization and add comments | Success: SpawnEntity() is well-organized with clear structure, comments explain spawn logic, code is ready for wave system integration, no change in current spawning behavior | Instructions: After completing this task, use log-implementation tool to record refactoring changes (structure improvements, comments added), then mark task as [x] in tasks.md_

- [x] 1.6. Create WaveManager class skeleton
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp (new)
  - Create WaveManager class with basic structure
  - Add member variables for wave state
  - Add placeholder methods (Update, StartWave, CompleteWave)
  - Purpose: Establish foundation for wave system
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game systems and class design | Task: Create WaveManager class skeleton with basic structure and placeholder methods following Requirement 3 and Design Component 1 | Restrictions: Do not implement full wave logic yet, focus on class structure and interface, ensure class integrates with existing Game class | Success: WaveManager class compiles successfully, has clear interface for wave management, integrates with Game class without errors, ready for full implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, methods defined, integration points), then mark task as [x] in tasks.md_

- [x] 1.7. Create UpgradeManager class skeleton
  - Files: Code/Game/Gameplay/UpgradeManager.cpp/hpp (new)
  - Create UpgradeManager class with basic structure
  - Define eUpgradeType enum
  - Add member variables for upgrade tracking
  - Add placeholder methods (PurchaseUpgrade, GetUpgradeLevel, GetUpgradeCost)
  - Purpose: Establish foundation for upgrade system
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing Shop class structure_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Implementation: Created UpgradeManager.hpp with eUpgradeType enum (7 types: FIRE_RATE, DAMAGE, PROJECTILE_COUNT, BULLET_SPREAD, BULLET_SIZE, PIERCING, HOMING), Upgrade struct with level/cost tracking, and UpgradeManager class with upgrade management methods. Created UpgradeManager.cpp with constructor initialization, placeholder implementations for PurchaseUpgrade(), GetUpgradeCost(), IsUpgradeAvailable(), GetUpgradeLevel(), IsUpgradeMaxed(), and GetUpgrade(). Added both files to Game.vcxproj. Class follows SOLID principles with single responsibility for upgrade management, uses const correctness, and includes detailed English comments._
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game systems and data structures | Task: Create UpgradeManager class skeleton with upgrade tracking structure following Requirement 7 and Design Component 2 | Restrictions: Do not implement full upgrade logic yet, focus on class structure and data models, ensure class can be integrated with Player and Shop | Success: UpgradeManager class compiles successfully, eUpgradeType enum defines all 7 upgrade types, has clear interface for upgrade management, ready for full implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, enum defined, methods declared), then mark task as [x] in tasks.md_

- [x] 1.8. Integrate WaveManager with Game class
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add WaveManager member variable to Game class
  - Initialize WaveManager in Game constructor
  - Call WaveManager::Update() in Game::Update()
  - Purpose: Connect wave system to main game loop
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Implementation: Added WaveManager forward declaration and m_waveManager member variable to Game.hpp. Added GetWaveManager() accessor. In Game.cpp: included WaveManager.hpp, initialized m_waveManager in constructor after game clock, added GAME_SAFE_RELEASE cleanup in destructor, and called m_waveManager->Update() during GAME state in Game::Update(). Follows existing codebase patterns for memory management and lifecycle._
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game architecture and system integration | Task: Integrate WaveManager with Game class following Requirement 3, connecting wave system to main game loop | Restrictions: Do not break existing game functionality, ensure proper initialization and cleanup, maintain game state management | Success: WaveManager is properly integrated with Game class, Update() is called each frame, no crashes or errors, game still functions normally | Instructions: After completing this task, use log-implementation tool to record integration details (member added, initialization code, update call), then mark task as [x] in tasks.md_

- [x] 1.9. Integrate UpgradeManager with Game class
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add UpgradeManager member variable to Game class
  - Initialize UpgradeManager in Game constructor
  - Provide access method for Player and Shop
  - Purpose: Connect upgrade system to game
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Implementation: Added UpgradeManager forward declaration and m_upgradeManager member variable to Game.hpp. Added GetUpgradeManager() accessor for Player and Shop access. In Game.cpp: included UpgradeManager.hpp, initialized m_upgradeManager in constructor after WaveManager, added GAME_SAFE_RELEASE cleanup in destructor. Player and Shop can access upgrades via game->GetUpgradeManager()._
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game architecture and system integration | Task: Integrate UpgradeManager with Game class following Requirement 7, making it accessible to Player and Shop | Restrictions: Do not break existing game functionality, ensure proper initialization and cleanup, maintain encapsulation | Success: UpgradeManager is properly integrated with Game class, accessible via getter method, no crashes or errors, ready for Player and Shop integration | Instructions: After completing this task, use log-implementation tool to record integration details (member added, initialization code, accessor method), then mark task as [x] in tasks.md_

- [x] 1.10. Create EnemyUtils namespace with shared behaviors
  - Files: Code/Game/Gameplay/EnemyUtils.cpp/hpp (new)
  - Create EnemyUtils namespace
  - Implement ChasePlayer() utility function
  - Implement GetDirectionToPlayer() utility function
  - Implement GetRandomSpawnPosition() utility function
  - Purpose: Provide reusable enemy behavior functions
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing enemy movement code from Triangle.cpp_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types)_
  - _Implementation: Created EnemyUtils namespace with 3 pure utility functions: GetDirectionToPlayer() returns normalized direction vector, ChasePlayer() updates position and orientation toward player, GetRandomSpawnPosition() spawns enemies from screen edges to avoid player overlap. All functions are stateless and work with any enemy type. Added both files to Game.vcxproj._
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in AI behaviors and utility functions | Task: Create EnemyUtils namespace with shared enemy behavior functions following Requirement 4 and Design Component 3 | Restrictions: Keep functions pure and stateless, ensure functions work for all enemy types, maintain performance efficiency | Success: EnemyUtils namespace compiles successfully, utility functions are well-tested and reusable, functions provide correct movement calculations, ready for enemy implementation | Instructions: After completing this task, use log-implementation tool to record utility creation (namespace created, functions implemented, usage examples), then mark task as [x] in tasks.md_

- [x] 1.11. Add event system events for wave and upgrade systems
  - Files: Code/Game/Gameplay/Game.cpp, Code/Game/Gameplay/WaveManager.cpp, Code/Game/Gameplay/UpgradeManager.cpp
  - Define new event names: OnWaveStart, OnWaveComplete, OnBossSpawn, OnUpgradePurchased
  - Add event firing in WaveManager and UpgradeManager
  - Add event subscription in Game class
  - Purpose: Enable decoupled communication for wave and upgrade events
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing EventSystem usage (OnGameStateChanged, OnCollisionEnter)_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 7 (Combat Upgrade System)_
  - _Implementation: Added 4 new events following existing EventSystem patterns. WaveManager fires OnWaveStart (with waveNumber, totalEnemies, isBossWave args) in StartWave(), OnWaveComplete in CompleteWave(), and OnBossSpawn for boss waves (every 5 waves). UpgradeManager fires OnUpgradePurchased (with upgradeType, newLevel, cost args) in PurchaseUpgrade(). Added UpgradeTypeToString() helper. Game class subscribes to all 4 events with debug logging handlers, properly unsubscribes in destructor. Also implemented StartWave/CompleteWave logic with wave number tracking and difficulty scaling._
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in event-driven architecture and observer pattern | Task: Add event system events for wave and upgrade systems following Requirements 3 and 7, using existing EventSystem patterns | Restrictions: Follow existing event naming conventions, ensure proper event argument passing, do not break existing event subscriptions | Success: New events are properly defined and fired, event subscriptions work correctly, events enable decoupled communication, no interference with existing events | Instructions: After completing this task, use log-implementation tool to record event implementation (events defined, firing locations, subscription handlers), then mark task as [x] in tasks.md_

- [x] 1.12. Code cleanup and documentation
  - Files: All files modified in Sprint 1
  - Add code comments explaining new systems
  - Update file headers with descriptions
  - Remove any debug code or commented-out code
  - Purpose: Ensure code is clean and well-documented
  - Time Estimate: [1.5, 2.0] hours
  - _Leverage: Existing code comment style_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Senior Developer with expertise in code documentation and best practices | Task: Clean up and document all code modified in Sprint 1 following Requirement 2, ensuring code quality and maintainability | Restrictions: Do not change functionality, maintain existing comment language consistency, follow project documentation standards | Success: All new code has clear comments, file headers are updated, no debug or commented-out code remains, code meets project quality standards | Instructions: After completing this task, use log-implementation tool to record cleanup activities (comments added, files cleaned, documentation updated), then mark task as [x] in tasks.md_

- [x] 1.13. Sprint 1 testing and verification
  - Files: N/A (testing)
  - Test compilation in Debug and Release
  - Test game launch and basic functionality
  - Verify WaveManager and UpgradeManager integrate without errors
  - Test existing gameplay (player movement, shooting, enemies)
  - Purpose: Ensure Sprint 1 changes don't break existing functionality
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: All Sprint 1 requirements_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in manual testing and regression testing | Task: Perform comprehensive testing of Sprint 1 changes covering all Sprint 1 requirements, ensuring no regressions in existing functionality | Restrictions: Do not modify code during testing, document all issues found, verify all acceptance criteria | Success: All Sprint 1 tasks are verified working, no regressions in existing gameplay, compilation errors are fixed, foundation systems are ready for Sprint 2 | Instructions: After completing this task, use log-implementation tool to record testing results (tests performed, issues found, verification status), then mark task as [x] in tasks.md_

## Sprint 1 Summary

**Total Tasks:** 13
**Estimated Hours:** 20.5 - 31.0 hours (target: 24 hours)
**Key Deliverables:**
- ✅ Compilation errors fixed
- ✅ Code refactored and cleaned up
- ✅ WaveManager skeleton created and integrated
- ✅ UpgradeManager skeleton created and integrated
- ✅ EnemyUtils namespace with shared behaviors
- ✅ Event system extended for new systems
- ✅ Foundation ready for enemy and upgrade implementation

---

# Sprint 2 Tasks - Tier 1 Enemies (Weeks 3-4, 24 hours)

## Overview
Sprint 2 focuses on implementing 3 new Tier 1 enemy types and redesigning the 3 existing enemy types with improved behaviors.

## Tasks

- [x] 2.1. Redesign Triangle enemy behavior
  - Files: Code/Game/Gameplay/Triangle.cpp/hpp
  - Implement chase behavior using EnemyUtils::ChasePlayer()
  - Update health scaling based on wave number
  - Add proper coin drop calculation
  - Improve movement smoothness
  - Purpose: Make Triangle a proper "chaser" enemy type
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: EnemyUtils::ChasePlayer(), existing Triangle class_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Triangle Redesign)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in enemy AI and behavior implementation | Task: Redesign Triangle enemy with chase behavior following Requirement 4 acceptance criteria for Triangle, using EnemyUtils functions | Restrictions: Maintain existing Entity inheritance, keep separate window functionality, ensure smooth movement without jitter | Success: Triangle chases player smoothly, health scales with wave number, coin drops work correctly, behavior feels responsive and challenging | Instructions: After completing this task, use log-implementation tool to record redesign details (behavior changes, EnemyUtils usage, health scaling formula), then mark task as [x] in tasks.md_

- [x] 2.2. Redesign Circle enemy behavior
  - Files: Code/Game/Gameplay/Circle.cpp/hpp
  - Implement orbital movement using EnemyUtils::OrbitPlayer()
  - Add angle tracking for smooth orbiting
  - Update health and speed values
  - Purpose: Make Circle a unique "orbiter" enemy type
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: EnemyUtils::OrbitPlayer(), existing Circle class_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Circle Redesign)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in circular motion and enemy behaviors | Task: Redesign Circle enemy with orbital movement following Requirement 4 acceptance criteria for Circle, implementing smooth circular patterns | Restrictions: Maintain existing Entity inheritance, keep separate window functionality, ensure orbit radius is appropriate for gameplay | Success: Circle orbits player in smooth circular pattern, movement is predictable but challenging, health and speed are balanced, separate window displays correctly | Instructions: After completing this task, use log-implementation tool to record redesign details (orbital behavior, angle tracking, movement parameters), then mark task as [x] in tasks.md_

- [x] 2.3. Redesign Octagon enemy behavior
  - Files: Code/Game/Gameplay/Octagon.cpp/hpp
  - Implement ranged shooting behavior
  - Add distance maintenance logic (stay away from player)
  - Implement projectile spawning at intervals
  - Add shoot cooldown timer
  - Purpose: Make Octagon a "ranged shooter" enemy type
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: EnemyUtils::ShouldShootAtPlayer(), existing Bullet class_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Octagon Redesign)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in ranged combat and AI behaviors | Task: Redesign Octagon enemy with ranged shooting behavior following Requirement 4 acceptance criteria for Octagon, implementing distance maintenance and projectile spawning | Restrictions: Maintain existing Entity inheritance, reuse Bullet class for projectiles, ensure shooting frequency is balanced | Success: Octagon maintains distance from player, shoots projectiles at regular intervals, projectiles are visible and damaging, behavior creates interesting combat challenge | Instructions: After completing this task, use log-implementation tool to record redesign details (shooting behavior, distance logic, projectile spawning, cooldown system), then mark task as [x] in tasks.md_

- [x] 2.4. Implement Square enemy (Tanky)
  - Files: Code/Game/Gameplay/Square.cpp/hpp (new)
  - Create Square class inheriting from Entity
  - Implement slow chase behavior
  - Set high health (3-5x normal enemies)
  - Add separate window with health display
  - Purpose: Add tanky enemy type that absorbs damage
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Entity base class, EnemyUtils::ChasePlayer(), Triangle pattern_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Square)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in enemy design and balancing | Task: Implement Square enemy as tanky slow chaser following Requirement 4 acceptance criteria for Square, creating high-health enemy type | Restrictions: Inherit from Entity base class, use separate window, ensure movement is noticeably slower than other enemies | Success: Square enemy spawns correctly, moves slowly toward player, has high health (10-15), drops more coins than regular enemies, separate window displays health | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, health values, movement speed, coin drops), then mark task as [x] in tasks.md_

- [x] 2.5. Implement Pentagon enemy (Fast Zigzag)
  - Files: Code/Game/Gameplay/Pentagon.cpp/hpp (new)
  - Create Pentagon class inheriting from Entity
  - Implement zigzag movement using EnemyUtils::ZigZagToward()
  - Add phase tracking for zigzag pattern
  - Set fast movement speed
  - Purpose: Add fast, unpredictable enemy type
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Entity base class, EnemyUtils::ZigZagToward()_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Pentagon)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in movement patterns and enemy behaviors | Task: Implement Pentagon enemy with fast zigzag movement following Requirement 4 acceptance criteria for Pentagon, creating unpredictable movement pattern | Restrictions: Inherit from Entity base class, use separate window, ensure zigzag amplitude creates interesting dodging challenge | Success: Pentagon enemy spawns correctly, moves in fast zigzag pattern toward player, movement is unpredictable but fair, health and speed are balanced, separate window displays health | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, zigzag behavior, phase tracking, movement parameters), then mark task as [x] in tasks.md_

- [x] 2.6. Implement Hexagon enemy (Splits on Death)
  - Files: Code/Game/Gameplay/Hexagon.cpp/hpp (new)
  - Create Hexagon class inheriting from Entity
  - Implement chase behavior
  - Override MarkAsDead() to spawn 2-3 smaller hexagons
  - Add size tracking (large vs small hexagon)
  - Prevent infinite splitting (small hexagons don't split)
  - Purpose: Add enemy that creates tactical challenge through splitting
  - Time Estimate: [3.5, 4.5] hours
  - _Leverage: Entity base class, EnemyUtils::ChasePlayer(), Game::SpawnEntity()_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Hexagon)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in complex enemy behaviors and spawning systems | Task: Implement Hexagon enemy with split-on-death behavior following Requirement 4 acceptance criteria for Hexagon, creating tactical splitting mechanic | Restrictions: Inherit from Entity base class, use separate window, ensure split spawning doesn't cause performance issues or spawn failures | Success: Hexagon enemy spawns correctly, chases player, splits into 2-3 smaller hexagons on death, smaller hexagons don't split further, splitting creates interesting tactical challenge | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, split behavior, size tracking, spawn prevention logic), then mark task as [x] in tasks.md_

- [ ] 2.7. Add enemy type spawn weights to WaveManager
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp
  - Define spawn weight system for enemy types
  - Create spawn tables for Tier 1 enemies
  - Implement random enemy selection based on weights
  - Purpose: Enable controlled random enemy spawning
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing random number generator (g_rng)_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in procedural generation and spawn systems | Task: Implement spawn weight system in WaveManager following Requirement 3, enabling controlled random enemy selection | Restrictions: Use existing g_rng for randomization, ensure spawn weights are balanced, maintain performance with frequent random selections | Success: Spawn weight system works correctly, enemy types spawn with appropriate frequency, random selection is fair and balanced, system is extensible for Tier 2 enemies | Instructions: After completing this task, use log-implementation tool to record spawn system details (weight system design, spawn tables, selection algorithm), then mark task as [x] in tasks.md_

- [ ] 2.8. Update Game::SpawnEntity() to use enemy types
  - Files: Code/Game/Gameplay/Game.cpp
  - Modify SpawnEntity() to accept enemy type string parameter
  - Add switch/if-else to spawn correct enemy class
  - Integrate with WaveManager for enemy type selection
  - Purpose: Enable dynamic enemy type spawning
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing SpawnEntity() method, new enemy classes_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in factory patterns and dynamic object creation | Task: Update SpawnEntity() to support dynamic enemy type spawning following Requirements 3 and 4, integrating with WaveManager | Restrictions: Maintain existing spawn functionality, ensure proper entity ID assignment, handle invalid enemy type strings gracefully | Success: SpawnEntity() accepts enemy type parameter, spawns correct enemy class, integrates with WaveManager, all 6 Tier 1 enemy types can be spawned dynamically | Instructions: After completing this task, use log-implementation tool to record spawn system changes (parameter added, type selection logic, WaveManager integration), then mark task as [x] in tasks.md_

- [ ] 2.9. Update collision handling for new enemy types
  - Files: Code/Game/Gameplay/Game.cpp (or CollisionUtils if refactored)
  - Add collision handlers for Square, Pentagon, Hexagon
  - Ensure bullet-enemy collisions work for all types
  - Ensure player-enemy collisions work for all types
  - Purpose: Enable combat with new enemy types
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing collision handling code_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in collision detection and physics | Task: Update collision handling for new enemy types following Requirement 4, ensuring all enemy types interact correctly with bullets and player | Restrictions: Maintain existing collision behavior for Triangle/Circle/Octagon, use same collision detection method (DoDiscsOverlap2D), ensure performance with multiple enemy types | Success: All 6 Tier 1 enemy types collide correctly with bullets, all enemy types collide correctly with player, collision detection is performant, no collision bugs or edge cases | Instructions: After completing this task, use log-implementation tool to record collision updates (handlers added, enemy types supported, collision testing results), then mark task as [x] in tasks.md_

- [ ] 2.10. Add visual variety to enemy types
  - Files: Code/Game/Gameplay/[Enemy].cpp files
  - Set unique colors for each enemy type
  - Adjust cosmetic radius for visual distinction
  - Ensure health widgets display correctly
  - Purpose: Make enemy types visually distinguishable
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing Entity rendering system_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in visual design and rendering | Task: Add visual variety to enemy types following Requirement 4, making each enemy type easily distinguishable | Restrictions: Use existing rendering system, maintain performance, ensure colors are distinct and readable | Success: Each enemy type has unique color, sizes are visually distinct, health widgets display correctly, players can easily identify enemy types at a glance | Instructions: After completing this task, use log-implementation tool to record visual changes (colors assigned, sizes adjusted, rendering verified), then mark task as [x] in tasks.md_

- [ ] 2.11. Test all 6 Tier 1 enemy types
  - Files: N/A (testing)
  - Spawn each enemy type individually and verify behavior
  - Test enemy health, movement, and collision
  - Test coin drops for each enemy type
  - Test window creation and display
  - Purpose: Verify all Tier 1 enemies work correctly
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in gameplay testing and enemy behavior verification | Task: Test all 6 Tier 1 enemy types following Requirement 4 acceptance criteria, verifying each enemy behaves correctly | Restrictions: Do not modify code during testing, document all issues found, test each enemy type thoroughly | Success: All 6 enemy types spawn correctly, behaviors match design specifications, health/damage/coins work correctly, no crashes or visual bugs, enemies create interesting gameplay variety | Instructions: After completing this task, use log-implementation tool to record testing results (enemies tested, behaviors verified, issues found), then mark task as [x] in tasks.md_

- [ ] 2.12. Sprint 2 integration testing
  - Files: N/A (testing)
  - Test multiple enemy types spawning together
  - Test enemy variety in gameplay
  - Verify performance with 10+ enemies on screen
  - Test wave progression with new enemy types
  - Purpose: Ensure all Tier 1 enemies work together correctly
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in integration testing and performance verification | Task: Perform integration testing of all Tier 1 enemies following Requirements 3 and 4, ensuring enemies work together correctly | Restrictions: Do not modify code during testing, document performance issues, verify gameplay balance | Success: Multiple enemy types spawn and behave correctly together, performance is acceptable with 10+ enemies, enemy variety creates interesting gameplay, no integration bugs or crashes | Instructions: After completing this task, use log-implementation tool to record integration testing results (scenarios tested, performance metrics, balance feedback), then mark task as [x] in tasks.md_

## Sprint 2 Summary

**Total Tasks:** 12
**Estimated Hours:** 23.5 - 35.5 hours (target: 24 hours)
**Key Deliverables:**
- ✅ 3 existing enemy types redesigned (Triangle, Circle, Octagon)
- ✅ 3 new enemy types implemented (Square, Pentagon, Hexagon)
- ✅ Spawn weight system in WaveManager
- ✅ Dynamic enemy spawning in Game::SpawnEntity()
- ✅ Collision handling for all enemy types
- ✅ Visual variety for enemy identification
- ✅ All 6 Tier 1 enemies tested and working

**Dependencies for Sprint 3:**
- All Tier 1 enemies must be working before implementing Tier 2 enemies
- Spawn system must be functional before adding boss spawning
- WaveManager must support enemy type selection before wave progression
# Sprint 3 Tasks - Tier 2 Enemies & Wave System (Weeks 5-6, 24 hours)

## Overview
Sprint 3 focuses on implementing 3 Tier 2 elite enemy types and completing the wave-based spawning system with boss mechanics.

## Tasks

- [ ] 3.1. Implement Spawner enemy
  - Files: Code/Game/Gameplay/Spawner.cpp/hpp (new)
  - Create Spawner class inheriting from Entity
  - Implement stationary behavior (no movement)
  - Add timer system for spawning Tier 1 enemies every 5-10 seconds
  - Limit maximum spawned enemies (3 at a time)
  - Purpose: Add enemy that creates tactical challenge through spawning
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Entity base class, Game::SpawnEntity(), WaveManager spawn tables_
  - _Requirements: Requirement 5 (Tier 2 Elite Enemy Types - Spawner)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in spawning systems and timer-based behaviors | Task: Implement Spawner enemy with timer-based spawning following Requirement 5 acceptance criteria for Spawner, creating tactical spawning mechanic | Restrictions: Inherit from Entity base class, use separate window, ensure spawned enemies don't exceed limit, prevent spawn spam | Success: Spawner enemy spawns correctly, remains stationary, spawns Tier 1 enemies at intervals, respects spawn limit, separate window displays health and spawn count | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, timer system, spawn logic, spawn limit), then mark task as [x] in tasks.md_

- [ ] 3.2. Implement Teleporter enemy
  - Files: Code/Game/Gameplay/Teleporter.cpp/hpp (new)
  - Create Teleporter class inheriting from Entity
  - Implement teleport behavior (random position every 2-4 seconds)
  - Add teleport timer and cooldown
  - Add visual effect for teleportation (optional particle effect)
  - Purpose: Add enemy with unpredictable positioning
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Entity base class, EnemyUtils::GetRandomSpawnPosition()_
  - _Requirements: Requirement 5 (Tier 2 Elite Enemy Types - Teleporter)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in teleportation mechanics and position randomization | Task: Implement Teleporter enemy with random teleportation following Requirement 5 acceptance criteria for Teleporter, creating unpredictable positioning | Restrictions: Inherit from Entity base class, use separate window, ensure teleport positions are valid, prevent teleporting into player | Success: Teleporter enemy spawns correctly, teleports to random positions at intervals, teleport positions are valid and fair, separate window displays health | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, teleport behavior, timer system, position validation), then mark task as [x] in tasks.md_

- [ ] 3.3. Implement Shielded enemy
  - Files: Code/Game/Gameplay/Shielded.cpp/hpp (new)
  - Create Shielded class inheriting from Entity
  - Implement chase behavior
  - Add dual health system (shield health + regular health)
  - Shield absorbs damage first, then health
  - Add visual distinction for shield (different color or effect)
  - Purpose: Add enemy with defensive mechanic
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Entity base class, EnemyUtils::ChasePlayer()_
  - _Requirements: Requirement 5 (Tier 2 Elite Enemy Types - Shielded)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in health systems and defensive mechanics | Task: Implement Shielded enemy with dual health system following Requirement 5 acceptance criteria for Shielded, creating defensive mechanic | Restrictions: Inherit from Entity base class, use separate window, ensure shield absorbs damage before health, display both shield and health | Success: Shielded enemy spawns correctly, chases player, shield absorbs damage first, health takes damage after shield breaks, separate window displays both shield and health | Instructions: After completing this task, use log-implementation tool to record implementation details (class created, dual health system, damage absorption logic, visual distinction), then mark task as [x] in tasks.md_

- [ ] 3.4. Complete WaveManager wave progression logic
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp
  - Implement StartWave() method with enemy count calculation
  - Implement CompleteWave() method with coin rewards
  - Add wave difficulty scaling (more enemies, faster spawns)
  - Implement wave state tracking (active, complete, boss phase)
  - Purpose: Enable full wave-based gameplay loop
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing spawn timer pattern, Game::SpawnEntity()_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in wave systems and difficulty scaling | Task: Complete WaveManager wave progression logic following Requirement 3 acceptance criteria, implementing full wave loop | Restrictions: Use existing spawn patterns, ensure difficulty scales appropriately, maintain 60 FPS with increased enemy counts | Success: Waves start and complete correctly, enemy count scales with wave number, spawn intervals decrease appropriately, coin rewards work, wave state tracking is accurate | Instructions: After completing this task, use log-implementation tool to record wave system details (progression logic, difficulty scaling formulas, state management), then mark task as [x] in tasks.md_

- [ ] 3.5. Implement boss spawn system in WaveManager
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp
  - Add boss phase detection (all regular enemies cleared)
  - Implement boss selection based on wave number
  - Add boss spawn logic with special positioning
  - Fire OnBossSpawn event
  - Purpose: Enable boss battles at end of waves
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing spawn system, EventSystem_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in boss mechanics and event systems | Task: Implement boss spawn system in WaveManager following Requirement 3 acceptance criteria for boss spawning | Restrictions: Use existing spawn patterns, ensure boss spawns only after regular enemies cleared, fire appropriate events | Success: Boss phase triggers correctly after regular enemies cleared, correct boss type spawns based on wave number, OnBossSpawn event fires, boss positioning is appropriate | Instructions: After completing this task, use log-implementation tool to record boss system details (phase detection, boss selection logic, spawn positioning, event firing), then mark task as [x] in tasks.md_

- [ ] 3.6. Add wave UI display
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add wave number display to main window
  - Add enemies remaining counter
  - Add boss phase indicator
  - Update UI each frame based on WaveManager state
  - Purpose: Provide player feedback on wave progress
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing WidgetSubsystem, ButtonWidget for text display_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: UI Developer with expertise in game HUD and widget systems | Task: Add wave UI display following Requirement 3, providing player feedback on wave progress | Restrictions: Use existing WidgetSubsystem, maintain performance, ensure UI is readable and non-intrusive | Success: Wave number displays correctly, enemies remaining counter updates accurately, boss phase indicator appears when appropriate, UI is clear and readable | Instructions: After completing this task, use log-implementation tool to record UI implementation (widgets created, update logic, positioning), then mark task as [x] in tasks.md_

- [ ] 3.7. Implement wave completion rewards
  - Files: Code/Game/Gameplay/WaveManager.cpp, Code/Game/Gameplay/Game.cpp
  - Add coin reward calculation based on wave number
  - Spawn coins at wave completion
  - Add wave completion visual/audio feedback
  - Fire OnWaveComplete event
  - Purpose: Reward players for completing waves
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing coin spawning, EventSystem_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in reward systems and player feedback | Task: Implement wave completion rewards following Requirement 3, rewarding players for wave completion | Restrictions: Use existing coin spawning, ensure rewards scale with difficulty, fire appropriate events | Success: Coins spawn at wave completion, reward amount scales with wave number, visual/audio feedback plays, OnWaveComplete event fires correctly | Instructions: After completing this task, use log-implementation tool to record reward system details (coin calculation, spawn logic, feedback implementation), then mark task as [x] in tasks.md_

- [ ] 3.8. Update spawn tables for Tier 2 enemies
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp
  - Add Tier 2 enemies to spawn tables
  - Set minimum wave requirements (Tier 2 starts at wave 3+)
  - Adjust spawn weights for balanced variety
  - Purpose: Enable Tier 2 enemy spawning in later waves
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing spawn weight system_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 5 (Tier 2 Elite Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Designer with expertise in spawn balancing and difficulty curves | Task: Update spawn tables for Tier 2 enemies following Requirements 3 and 5, enabling balanced enemy variety | Restrictions: Use existing spawn weight system, ensure Tier 2 enemies don't spawn too early, maintain gameplay balance | Success: Tier 2 enemies spawn in appropriate waves, spawn frequency is balanced, enemy variety creates interesting gameplay, difficulty curve feels appropriate | Instructions: After completing this task, use log-implementation tool to record spawn table updates (Tier 2 entries added, wave requirements, weight balancing), then mark task as [x] in tasks.md_

- [ ] 3.9. Test Tier 2 enemy behaviors
  - Files: N/A (testing)
  - Spawn each Tier 2 enemy individually and verify behavior
  - Test Spawner spawning mechanics and limits
  - Test Teleporter teleportation and positioning
  - Test Shielded dual health system
  - Purpose: Verify all Tier 2 enemies work correctly
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 5 (Tier 2 Elite Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in gameplay testing and enemy behavior verification | Task: Test all 3 Tier 2 enemy types following Requirement 5 acceptance criteria, verifying each enemy behaves correctly | Restrictions: Do not modify code during testing, document all issues found, test each enemy type thoroughly | Success: All 3 Tier 2 enemy types spawn correctly, behaviors match design specifications, special mechanics work correctly, no crashes or visual bugs | Instructions: After completing this task, use log-implementation tool to record testing results (enemies tested, behaviors verified, issues found), then mark task as [x] in tasks.md_

- [ ] 3.10. Test wave progression system
  - Files: N/A (testing)
  - Play through multiple waves (1-10)
  - Verify wave start and completion
  - Verify boss spawning at end of waves
  - Verify difficulty scaling
  - Verify coin rewards
  - Purpose: Ensure wave system works correctly
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in system testing and gameplay flow verification | Task: Test wave progression system following Requirement 3 acceptance criteria, verifying full wave loop | Restrictions: Do not modify code during testing, document all issues found, test multiple wave scenarios | Success: Waves progress correctly, boss spawning works, difficulty scales appropriately, coin rewards work, UI displays correctly, no crashes or progression bugs | Instructions: After completing this task, use log-implementation tool to record testing results (waves tested, progression verified, issues found), then mark task as [x] in tasks.md_

- [ ] 3.11. Sprint 3 integration testing
  - Files: N/A (testing)
  - Test Tier 1 and Tier 2 enemies spawning together
  - Test wave progression with mixed enemy types
  - Verify performance with 15+ enemies on screen
  - Test boss battles with various boss types
  - Purpose: Ensure all Sprint 3 features work together
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 5 (Tier 2 Elite Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in integration testing and performance verification | Task: Perform integration testing of Sprint 3 features following Requirements 3 and 5, ensuring all systems work together | Restrictions: Do not modify code during testing, document performance issues, verify gameplay balance | Success: All enemy types work together correctly, wave progression is smooth, performance is acceptable with 15+ enemies, boss battles are challenging and fair, no integration bugs | Instructions: After completing this task, use log-implementation tool to record integration testing results (scenarios tested, performance metrics, balance feedback), then mark task as [x] in tasks.md_

## Sprint 3 Summary

**Total Tasks:** 11
**Estimated Hours:** 25.5 - 36.5 hours (target: 24 hours)
**Key Deliverables:**
- ✅ 3 Tier 2 elite enemy types implemented (Spawner, Teleporter, Shielded)
- ✅ Wave progression system completed
- ✅ Boss spawn system implemented
- ✅ Wave UI display added
- ✅ Wave completion rewards working
- ✅ Spawn tables updated for Tier 2 enemies
- ✅ Full wave-based gameplay loop functional

**Dependencies for Sprint 4:**
- Wave system must be working before implementing combat upgrades
- All enemy types must be functional before testing upgrade effectiveness
- Spawn system must support all enemy types before upgrade balancing
# Sprint 4 Tasks - Combat Upgrades (Weeks 7-8, 24 hours)

## Overview
Sprint 4 focuses on implementing the 7 combat upgrade types and extending the Shop, Player, and Bullet systems to support upgrades.

## Tasks

- [ ] 4.1. Complete UpgradeManager implementation
  - Files: Code/Game/Gameplay/UpgradeManager.cpp/hpp
  - Implement PurchaseUpgrade() with cost calculation
  - Implement GetUpgradeLevel() and GetUpgradeCost()
  - Implement all stat getter methods (fire rate, damage, etc.)
  - Add upgrade level tracking and persistence
  - Purpose: Enable full upgrade system functionality
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing UpgradeManager skeleton, Shop cost patterns_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in upgrade systems and stat management | Task: Complete UpgradeManager implementation following Requirement 7 acceptance criteria, enabling full upgrade functionality | Restrictions: Use existing skeleton structure, ensure cost scaling is balanced, maintain upgrade level persistence | Success: All upgrade methods work correctly, cost calculation scales appropriately, stat getters return correct values, upgrade levels persist correctly | Instructions: After completing this task, use log-implementation tool to record implementation details (methods completed, cost formulas, stat calculations), then mark task as [x] in tasks.md_

- [ ] 4.2. Extend Shop with upgrade display
  - Files: Code/Game/Gameplay/Shop.cpp/hpp
  - Add 3 random upgrade slots to shop UI
  - Display upgrade name, level, and cost
  - Add purchase buttons for each upgrade
  - Integrate with UpgradeManager for purchase logic
  - Purpose: Enable players to purchase upgrades
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing Shop UI, WidgetSubsystem, ButtonWidget_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: UI Developer with expertise in shop systems and widget management | Task: Extend Shop with upgrade display following Requirement 7 acceptance criteria, enabling upgrade purchases | Restrictions: Use existing Shop structure, maintain existing shop functionality, ensure UI is clear and readable | Success: Shop displays 3 random upgrades, upgrade info is accurate, purchase buttons work correctly, upgrades integrate with UpgradeManager | Instructions: After completing this task, use log-implementation tool to record shop extension details (UI widgets added, purchase logic, UpgradeManager integration), then mark task as [x] in tasks.md_

- [ ] 4.3. Implement shop refresh system
  - Files: Code/Game/Gameplay/Shop.cpp/hpp
  - Add refresh button to shop UI
  - Implement refresh cost calculation (1 coin base + 10 per refresh)
  - Randomize 3 new upgrades on refresh
  - Update refresh cost display
  - Purpose: Enable players to reroll upgrade options
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing Shop UI, random number generator (g_rng)_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in shop systems and randomization | Task: Implement shop refresh system following Requirement 7 acceptance criteria, enabling upgrade rerolling | Restrictions: Use existing Shop structure, ensure refresh cost scales appropriately, maintain randomization fairness | Success: Refresh button works correctly, cost calculation is accurate, upgrades randomize properly, cost display updates correctly | Instructions: After completing this task, use log-implementation tool to record refresh system details (button added, cost calculation, randomization logic), then mark task as [x] in tasks.md_

- [ ] 4.4. Extend Player with upgrade stat application
  - Files: Code/Game/Gameplay/Player.cpp/hpp
  - Add UpgradeManager reference to Player
  - Apply fire rate multiplier to shooting cooldown
  - Apply damage multiplier to bullet damage
  - Apply projectile count to bullet spawning
  - Apply bullet spread to firing angles
  - Purpose: Enable upgrades to affect player combat
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing Player shooting logic, UpgradeManager stat getters_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in player systems and stat application | Task: Extend Player with upgrade stat application following Requirement 7 acceptance criteria, enabling upgrades to affect combat | Restrictions: Maintain existing Player functionality, ensure stat application is accurate, balance upgrade effects | Success: Fire rate upgrade affects shooting speed, damage upgrade affects bullet damage, projectile count spawns multiple bullets, spread upgrade affects firing angles | Instructions: After completing this task, use log-implementation tool to record player extension details (UpgradeManager integration, stat application logic, shooting modifications), then mark task as [x] in tasks.md_

- [ ] 4.5. Extend Bullet with piercing behavior
  - Files: Code/Game/Gameplay/Bullet.cpp/hpp
  - Add piercing count member variable
  - Track enemies hit by bullet
  - Prevent bullet destruction until piercing count reached
  - Apply piercing upgrade from UpgradeManager
  - Purpose: Enable piercing bullet upgrade
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing Bullet collision handling, UpgradeManager_
  - _Requirements: Requirement 7 (Combat Upgrade System - Piercing)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in projectile systems and collision handling | Task: Extend Bullet with piercing behavior following Requirement 7 acceptance criteria for piercing upgrade | Restrictions: Maintain existing Bullet functionality, ensure piercing doesn't cause infinite loops, track hit enemies correctly | Success: Bullets pierce through enemies based on upgrade level, piercing count is tracked correctly, bullets destroy after piercing limit reached | Instructions: After completing this task, use log-implementation tool to record bullet extension details (piercing logic, hit tracking, collision modifications), then mark task as [x] in tasks.md_

- [ ] 4.6. Extend Bullet with homing behavior
  - Files: Code/Game/Gameplay/Bullet.cpp/hpp
  - Add homing strength member variable
  - Implement homing logic in Update() method
  - Find nearest enemy and adjust velocity toward it
  - Apply homing upgrade from UpgradeManager
  - Purpose: Enable homing bullet upgrade
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing Bullet Update(), EnemyUtils::GetDirectionToPlayer()_
  - _Requirements: Requirement 7 (Combat Upgrade System - Homing)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in homing projectiles and AI targeting | Task: Extend Bullet with homing behavior following Requirement 7 acceptance criteria for homing upgrade | Restrictions: Maintain existing Bullet functionality, ensure homing is balanced and not overpowered, find nearest enemy efficiently | Success: Bullets home toward nearest enemy based on upgrade level, homing strength is appropriate, targeting is accurate and performant | Instructions: After completing this task, use log-implementation tool to record bullet extension details (homing logic, targeting algorithm, strength application), then mark task as [x] in tasks.md_

- [ ] 4.7. Extend Bullet with size scaling
  - Files: Code/Game/Gameplay/Bullet.cpp/hpp
  - Add size multiplier member variable
  - Apply size scaling to cosmetic radius
  - Apply size scaling to collision radius
  - Apply bullet size upgrade from UpgradeManager
  - Purpose: Enable bullet size upgrade
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing Bullet rendering, Entity radius system_
  - _Requirements: Requirement 7 (Combat Upgrade System - Bullet Size)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in rendering and collision systems | Task: Extend Bullet with size scaling following Requirement 7 acceptance criteria for bullet size upgrade | Restrictions: Maintain existing Bullet functionality, ensure size scaling affects both visual and collision, balance size increase | Success: Bullets scale in size based on upgrade level, both cosmetic and collision radius scale correctly, size increase is visually clear | Instructions: After completing this task, use log-implementation tool to record bullet extension details (size scaling logic, radius modifications, visual verification), then mark task as [x] in tasks.md_

- [ ] 4.8. Add visual feedback for active upgrades
  - Files: Code/Game/Gameplay/Player.cpp, Code/Game/Gameplay/Bullet.cpp
  - Add visual effects for piercing bullets (trail effect)
  - Add visual effects for homing bullets (targeting indicator)
  - Add visual effects for large bullets (glow effect)
  - Add player visual feedback for high fire rate
  - Purpose: Provide visual feedback for upgrade effects
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing rendering system, particle effects (if available)_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in visual effects and rendering | Task: Add visual feedback for active upgrades following Requirement 7, providing clear upgrade indicators | Restrictions: Use existing rendering system, maintain performance, ensure effects are clear but not distracting | Success: Piercing bullets have trail effect, homing bullets have targeting indicator, large bullets have glow, high fire rate has visual feedback | Instructions: After completing this task, use log-implementation tool to record visual feedback details (effects added, rendering modifications, visual verification), then mark task as [x] in tasks.md_

- [ ] 4.9. Test all 7 upgrade types
  - Files: N/A (testing)
  - Purchase each upgrade type and verify effect
  - Test upgrade stacking (purchase multiple times)
  - Test upgrade cost scaling
  - Test visual feedback for each upgrade
  - Purpose: Verify all upgrades work correctly
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in gameplay testing and upgrade system verification | Task: Test all 7 upgrade types following Requirement 7 acceptance criteria, verifying each upgrade works correctly | Restrictions: Do not modify code during testing, document all issues found, test each upgrade thoroughly | Success: All 7 upgrades work correctly, stacking works, cost scaling is appropriate, visual feedback is clear, upgrades create interesting gameplay variety | Instructions: After completing this task, use log-implementation tool to record testing results (upgrades tested, effects verified, issues found), then mark task as [x] in tasks.md_

- [ ] 4.10. Test shop refresh system
  - Files: N/A (testing)
  - Test shop refresh with sufficient coins
  - Test shop refresh with insufficient coins
  - Verify refresh cost increases correctly
  - Verify upgrade randomization works
  - Purpose: Ensure shop refresh works correctly
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in shop system testing and UI verification | Task: Test shop refresh system following Requirement 7 acceptance criteria, verifying refresh functionality | Restrictions: Do not modify code during testing, document all issues found, test various refresh scenarios | Success: Refresh works with sufficient coins, disabled with insufficient coins, cost increases correctly, upgrades randomize properly, UI updates correctly | Instructions: After completing this task, use log-implementation tool to record testing results (refresh scenarios tested, cost verification, randomization verified), then mark task as [x] in tasks.md_

- [ ] 4.11. Sprint 4 integration testing
  - Files: N/A (testing)
  - Test upgrades with wave progression
  - Test upgrade effectiveness against various enemy types
  - Verify upgrade balance and gameplay feel
  - Test performance with multiple active upgrades
  - Purpose: Ensure all Sprint 4 features work together
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in integration testing and gameplay balance verification | Task: Perform integration testing of Sprint 4 features following Requirement 7, ensuring upgrades integrate with wave system | Restrictions: Do not modify code during testing, document balance issues, verify gameplay feel | Success: Upgrades work correctly with wave progression, upgrades are effective against all enemy types, balance feels appropriate, performance is acceptable with multiple upgrades | Instructions: After completing this task, use log-implementation tool to record integration testing results (scenarios tested, balance feedback, performance metrics), then mark task as [x] in tasks.md_

## Sprint 4 Summary

**Total Tasks:** 11
**Estimated Hours:** 25.0 - 36.0 hours (target: 24 hours)
**Key Deliverables:**
- ✅ UpgradeManager fully implemented
- ✅ Shop extended with upgrade display and refresh
- ✅ Player extended with upgrade stat application
- ✅ Bullet extended with piercing, homing, and size upgrades
- ✅ Visual feedback for active upgrades
- ✅ All 7 upgrade types tested and working
- ✅ Shop refresh system functional
- ✅ Upgrades integrated with wave system

**Dependencies for Sprint 5:**
- Upgrade system must be working before adding upgrade purchase SFX
- All combat features must be functional before adding audio/visual polish
- Wave system must be stable before adding wave BGM
# Sprint 5 Tasks - Audio & Visual Polish (Weeks 9-10, 24 hours)

## Overview
Sprint 5 focuses on implementing audio systems (BGM and SFX) and visual effects (particles) to enhance game feel and polish.

## Tasks

- [ ] 5.1. Create AudioManager class
  - Files: Code/Game/Gameplay/AudioManager.cpp/hpp (new)
  - Create AudioManager class with basic structure
  - Add member variables for current BGM and SFX maps
  - Add placeholder methods for BGM and SFX playback
  - Integrate with existing AudioSystem (Engine)
  - Purpose: Establish foundation for audio management
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing AudioSystem usage in Game.cpp_
  - _Requirements: Requirement 8 (Audio System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio Engineer with expertise in game audio systems and sound management | Task: Create AudioManager class following Requirement 8 and Design Component 10, establishing audio management foundation | Restrictions: Use existing AudioSystem patterns, ensure proper sound cleanup, maintain performance with multiple sounds | Success: AudioManager class compiles successfully, integrates with AudioSystem, has clear interface for audio playback, ready for full implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, methods defined, AudioSystem integration), then mark task as [x] in tasks.md_

- [ ] 5.2. Implement wave BGM system
  - Files: Code/Game/Gameplay/AudioManager.cpp/hpp
  - Implement PlayWaveBGM() method
  - Add BGM transition logic (fade out old, fade in new)
  - Map wave numbers to BGM tracks
  - Integrate with WaveManager events (OnWaveStart)
  - Purpose: Add dynamic background music for waves
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing AudioSystem, EventSystem_
  - _Requirements: Requirement 8 (Audio System - Wave BGM)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio Engineer with expertise in music systems and audio transitions | Task: Implement wave BGM system following Requirement 8 acceptance criteria for wave BGM, adding dynamic background music | Restrictions: Use existing AudioSystem, ensure smooth transitions, prevent audio overlap | Success: BGM plays for each wave, transitions are smooth, wave-specific tracks play correctly, no audio glitches or overlaps | Instructions: After completing this task, use log-implementation tool to record BGM system details (playback logic, transition system, wave mapping), then mark task as [x] in tasks.md_

- [ ] 5.3. Implement enemy SFX system
  - Files: Code/Game/Gameplay/AudioManager.cpp/hpp, Code/Game/Gameplay/[Enemy].cpp files
  - Implement PlayEnemySpawnSFX() and PlayEnemyDeathSFX()
  - Map enemy types to SFX sounds
  - Add SFX calls to enemy spawn and death events
  - Limit simultaneous SFX to prevent audio spam
  - Purpose: Add audio feedback for enemy events
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing AudioSystem, EventSystem (OnEntityDestroyed)_
  - _Requirements: Requirement 8 (Audio System - Enemy SFX)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio Engineer with expertise in sound effects and event-driven audio | Task: Implement enemy SFX system following Requirement 8 acceptance criteria for enemy SFX, adding audio feedback for enemy events | Restrictions: Use existing AudioSystem, limit simultaneous sounds, ensure SFX don't overlap excessively | Success: Enemy spawn SFX plays correctly, enemy death SFX plays correctly, SFX are distinct per enemy type, audio spam is prevented | Instructions: After completing this task, use log-implementation tool to record SFX system details (playback logic, enemy mapping, spam prevention), then mark task as [x] in tasks.md_

- [ ] 5.4. Implement combat SFX
  - Files: Code/Game/Gameplay/AudioManager.cpp/hpp, Code/Game/Gameplay/Player.cpp, Code/Game/Gameplay/Bullet.cpp
  - Add player shooting SFX
  - Add bullet impact SFX
  - Add player damage SFX
  - Add upgrade purchase SFX
  - Purpose: Add audio feedback for combat actions
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing AudioSystem, EventSystem_
  - _Requirements: Requirement 8 (Audio System - Combat SFX)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio Engineer with expertise in combat audio and player feedback | Task: Implement combat SFX following Requirement 8 acceptance criteria for combat SFX, adding audio feedback for combat actions | Restrictions: Use existing AudioSystem, ensure SFX are clear and responsive, prevent audio spam | Success: Shooting SFX plays on bullet fire, impact SFX plays on bullet hit, damage SFX plays on player hit, upgrade purchase SFX plays on purchase | Instructions: After completing this task, use log-implementation tool to record combat SFX details (SFX added, playback triggers, spam prevention), then mark task as [x] in tasks.md_

- [ ] 5.5. Implement boss music system
  - Files: Code/Game/Gameplay/AudioManager.cpp/hpp
  - Implement PlayBossMusicSFX() method
  - Add boss music transition (fade out wave BGM, play boss music)
  - Integrate with WaveManager events (OnBossSpawn)
  - Return to wave BGM after boss defeated
  - Purpose: Add dramatic music for boss battles
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing wave BGM system, EventSystem_
  - _Requirements: Requirement 8 (Audio System - Boss Music)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio Engineer with expertise in dynamic music systems and boss battle audio | Task: Implement boss music system following Requirement 8 acceptance criteria for boss music, adding dramatic boss battle music | Restrictions: Use existing AudioSystem, ensure smooth transitions, return to wave BGM after boss | Success: Boss music plays on boss spawn, transitions are smooth, music returns to wave BGM after boss defeated, no audio glitches | Instructions: After completing this task, use log-implementation tool to record boss music details (playback logic, transition system, event integration), then mark task as [x] in tasks.md_

- [ ] 5.6. Create ParticleSystem class
  - Files: Code/Game/Gameplay/ParticleSystem.cpp/hpp (new)
  - Create ParticleSystem class with Particle struct
  - Implement Update() method for particle lifetime
  - Implement Render() method for particle drawing
  - Add particle spawning methods (death, impact, damage)
  - Purpose: Establish foundation for visual effects
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Existing rendering system, Entity rendering patterns_
  - _Requirements: Requirement 9 (Visual Effects System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Graphics Programmer with expertise in particle systems and visual effects | Task: Create ParticleSystem class following Requirement 9 and Design Component 11, establishing visual effects foundation | Restrictions: Use existing rendering system, maintain 60 FPS with many particles, cap particle count at 1000 | Success: ParticleSystem class compiles successfully, particles update and render correctly, performance is acceptable with many particles, ready for effect implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, particle struct, update/render logic), then mark task as [x] in tasks.md_

- [ ] 5.7. Implement death particle effects
  - Files: Code/Game/Gameplay/ParticleSystem.cpp/hpp, Code/Game/Gameplay/[Enemy].cpp files
  - Implement SpawnDeathParticles() method
  - Add death particle spawning to enemy death events
  - Use enemy color for particle color
  - Add particle burst pattern (radial explosion)
  - Purpose: Add visual feedback for enemy deaths
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing ParticleSystem, EventSystem (OnEntityDestroyed)_
  - _Requirements: Requirement 9 (Visual Effects System - Death Effects)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Graphics Programmer with expertise in particle effects and visual feedback | Task: Implement death particle effects following Requirement 9 acceptance criteria for death effects, adding visual feedback for enemy deaths | Restrictions: Use existing ParticleSystem, maintain performance, ensure particles are visible and satisfying | Success: Death particles spawn on enemy death, particles use enemy color, burst pattern is visually appealing, performance is acceptable | Instructions: After completing this task, use log-implementation tool to record death effect details (spawn logic, burst pattern, color usage), then mark task as [x] in tasks.md_

- [ ] 5.8. Implement impact particle effects
  - Files: Code/Game/Gameplay/ParticleSystem.cpp/hpp, Code/Game/Gameplay/Bullet.cpp
  - Implement SpawnImpactParticles() method
  - Add impact particle spawning to bullet-enemy collisions
  - Use bullet color for particle color
  - Add particle spray pattern (directional spray)
  - Purpose: Add visual feedback for bullet impacts
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing ParticleSystem, collision handling_
  - _Requirements: Requirement 9 (Visual Effects System - Impact Effects)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Graphics Programmer with expertise in impact effects and collision feedback | Task: Implement impact particle effects following Requirement 9 acceptance criteria for impact effects, adding visual feedback for bullet impacts | Restrictions: Use existing ParticleSystem, maintain performance, ensure particles are visible and responsive | Success: Impact particles spawn on bullet hit, particles use bullet color, spray pattern is visually appealing, performance is acceptable | Instructions: After completing this task, use log-implementation tool to record impact effect details (spawn logic, spray pattern, color usage), then mark task as [x] in tasks.md_

- [ ] 5.9. Implement damage particle effects
  - Files: Code/Game/Gameplay/ParticleSystem.cpp/hpp, Code/Game/Gameplay/Player.cpp
  - Implement SpawnDamageParticles() method
  - Add damage particle spawning to player damage events
  - Use red color for damage particles
  - Add particle scatter pattern (random directions)
  - Purpose: Add visual feedback for player damage
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing ParticleSystem, player damage handling_
  - _Requirements: Requirement 9 (Visual Effects System - Damage Effects)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Graphics Programmer with expertise in damage feedback and visual effects | Task: Implement damage particle effects following Requirement 9 acceptance criteria for damage effects, adding visual feedback for player damage | Restrictions: Use existing ParticleSystem, maintain performance, ensure particles are visible and clear | Success: Damage particles spawn on player hit, particles use red color, scatter pattern is visually clear, performance is acceptable | Instructions: After completing this task, use log-implementation tool to record damage effect details (spawn logic, scatter pattern, color usage), then mark task as [x] in tasks.md_

- [ ] 5.10. Test audio system
  - Files: N/A (testing)
  - Test wave BGM playback and transitions
  - Test enemy SFX for all enemy types
  - Test combat SFX (shooting, impact, damage)
  - Test boss music transitions
  - Verify audio doesn't spam or overlap excessively
  - Purpose: Ensure audio system works correctly
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 8 (Audio System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in audio testing and sound quality verification | Task: Test audio system following Requirement 8 acceptance criteria, verifying all audio features work correctly | Restrictions: Do not modify code during testing, document all audio issues, test various audio scenarios | Success: All audio features work correctly, transitions are smooth, SFX are distinct and clear, no audio spam or glitches, audio enhances gameplay feel | Instructions: After completing this task, use log-implementation tool to record testing results (audio features tested, issues found, quality assessment), then mark task as [x] in tasks.md_

- [ ] 5.11. Test particle system
  - Files: N/A (testing)
  - Test death particles for all enemy types
  - Test impact particles for bullet collisions
  - Test damage particles for player hits
  - Verify particle performance with many particles
  - Verify particle colors and patterns
  - Purpose: Ensure particle system works correctly
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 9 (Visual Effects System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in visual effects testing and performance verification | Task: Test particle system following Requirement 9 acceptance criteria, verifying all particle effects work correctly | Restrictions: Do not modify code during testing, document performance issues, test various particle scenarios | Success: All particle effects work correctly, particles are visible and satisfying, performance is acceptable with many particles, colors and patterns are appropriate | Instructions: After completing this task, use log-implementation tool to record testing results (particle effects tested, performance metrics, visual quality assessment), then mark task as [x] in tasks.md_

- [ ] 5.12. Sprint 5 integration testing
  - Files: N/A (testing)
  - Test audio and visual effects together
  - Verify audio/visual sync (death SFX + particles)
  - Test performance with full audio/visual load
  - Verify overall game feel and polish
  - Purpose: Ensure all Sprint 5 features work together
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: Requirement 8 (Audio System), Requirement 9 (Visual Effects System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in integration testing and game feel verification | Task: Perform integration testing of Sprint 5 features following Requirements 8 and 9, ensuring audio and visual effects enhance gameplay | Restrictions: Do not modify code during testing, document polish issues, verify game feel | Success: Audio and visual effects work together seamlessly, sync is accurate, performance is acceptable with full load, game feel is significantly improved | Instructions: After completing this task, use log-implementation tool to record integration testing results (scenarios tested, sync verification, game feel assessment), then mark task as [x] in tasks.md_

## Sprint 5 Summary

**Total Tasks:** 12
**Estimated Hours:** 25.5 - 37.0 hours (target: 24 hours)
**Key Deliverables:**
- ✅ AudioManager class implemented
- ✅ Wave BGM system working
- ✅ Enemy SFX for all enemy types
- ✅ Combat SFX (shooting, impact, damage, upgrade)
- ✅ Boss music system working
- ✅ ParticleSystem class implemented
- ✅ Death, impact, and damage particle effects
- ✅ Audio and visual systems tested and polished

**Dependencies for Sprint 6:**
- Audio system must be working before game over screen
- Visual effects must be working before final polish
- All systems must be stable before final testing
# Sprint 6 Tasks - Game Over & Polish (Weeks 11-12, 24 hours)

## Overview
Sprint 6 focuses on implementing the game over system, statistics tracking, final bug fixes, optimization, and comprehensive testing.

## Tasks

- [ ] 6.1. Implement game over system
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add GAME_OVER state to eGameState enum
  - Add game statistics tracking (waves survived, enemies killed, coins earned)
  - Implement TriggerGameOver() method
  - Add game over detection (player health <= 0)
  - Purpose: Enable game over functionality
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing game state system, Player health tracking_
  - _Requirements: Requirement 10 (Game Over System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in game state management and statistics tracking | Task: Implement game over system following Requirement 10 acceptance criteria, enabling game over functionality | Restrictions: Use existing game state patterns, ensure statistics are tracked accurately, trigger game over only on player death | Success: Game over triggers on player death, statistics are tracked correctly, game state transitions to GAME_OVER, no crashes or state bugs | Instructions: After completing this task, use log-implementation tool to record game over system details (state added, statistics tracking, trigger logic), then mark task as [x] in tasks.md_

- [ ] 6.2. Implement game over UI
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add RenderGameOver() method
  - Display "GAME OVER" text
  - Display statistics (waves survived, enemies killed, coins earned)
  - Display restart instructions (SPACE to restart, ESC to attract mode)
  - Purpose: Provide game over screen with statistics
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing WidgetSubsystem, ButtonWidget for text display_
  - _Requirements: Requirement 10 (Game Over System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: UI Developer with expertise in game over screens and statistics display | Task: Implement game over UI following Requirement 10 acceptance criteria, displaying game over screen with statistics | Restrictions: Use existing WidgetSubsystem, ensure UI is clear and readable, display all required statistics | Success: Game over screen displays correctly, statistics are accurate and readable, restart instructions are clear, UI is visually appealing | Instructions: After completing this task, use log-implementation tool to record game over UI details (widgets created, statistics display, restart instructions), then mark task as [x] in tasks.md_

- [ ] 6.3. Implement game restart functionality
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Implement ResetGameStats() method
  - Reset player health and position
  - Clear all entities
  - Reset wave manager state
  - Reset upgrade manager state
  - Return to PLAYING state
  - Purpose: Enable game restart after game over
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing game initialization code_
  - _Requirements: Requirement 10 (Game Over System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in game state management and reset logic | Task: Implement game restart functionality following Requirement 10 acceptance criteria, enabling game restart after game over | Restrictions: Reset all game state properly, ensure no memory leaks, maintain proper initialization order | Success: Game restarts correctly after game over, all state is reset properly, no crashes or memory leaks, game plays normally after restart | Instructions: After completing this task, use log-implementation tool to record restart functionality details (reset logic, state cleanup, initialization), then mark task as [x] in tasks.md_

- [ ] 6.4. Add game over audio/visual feedback
  - Files: Code/Game/Gameplay/AudioManager.cpp, Code/Game/Gameplay/ParticleSystem.cpp
  - Add game over SFX
  - Add game over particle effect (screen flash or explosion)
  - Stop all BGM on game over
  - Purpose: Enhance game over feedback
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing AudioManager, ParticleSystem_
  - _Requirements: Requirement 10 (Game Over System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Audio/Visual Designer with expertise in game over feedback and dramatic effects | Task: Add game over audio/visual feedback following Requirement 10, enhancing game over experience | Restrictions: Use existing AudioManager and ParticleSystem, ensure effects are dramatic but not excessive | Success: Game over SFX plays on death, particle effect is visible and dramatic, BGM stops correctly, feedback enhances game over experience | Instructions: After completing this task, use log-implementation tool to record game over feedback details (SFX added, particle effect, BGM stop), then mark task as [x] in tasks.md_

- [ ] 6.5. Bug fixing pass 1 - Compilation and crashes
  - Files: Various (as needed)
  - Fix any remaining compilation warnings
  - Fix any crashes discovered during testing
  - Fix memory leaks (if any)
  - Fix entity cleanup issues
  - Purpose: Ensure game is stable and crash-free
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Visual Studio debugger, memory profiling tools_
  - _Requirements: All requirements (stability)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Senior Developer with expertise in debugging and stability fixes | Task: Perform bug fixing pass focusing on compilation warnings, crashes, and memory issues, ensuring game stability | Restrictions: Do not change functionality, focus on stability fixes only, document all fixes made | Success: No compilation warnings remain, no crashes occur during normal gameplay, no memory leaks detected, entity cleanup works correctly | Instructions: After completing this task, use log-implementation tool to record bug fixes (issues fixed, crash fixes, memory leak fixes), then mark task as [x] in tasks.md_

- [ ] 6.6. Bug fixing pass 2 - Gameplay issues
  - Files: Various (as needed)
  - Fix enemy behavior bugs (if any)
  - Fix collision detection issues (if any)
  - Fix upgrade application bugs (if any)
  - Fix wave progression bugs (if any)
  - Purpose: Ensure gameplay is bug-free
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Manual testing, gameplay verification_
  - _Requirements: All requirements (gameplay correctness)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in gameplay debugging and behavior fixes | Task: Perform bug fixing pass focusing on gameplay issues, ensuring all game features work correctly | Restrictions: Do not change intended functionality, focus on bug fixes only, document all fixes made | Success: Enemy behaviors work correctly, collision detection is accurate, upgrades apply correctly, wave progression works smoothly, no gameplay bugs remain | Instructions: After completing this task, use log-implementation tool to record bug fixes (gameplay issues fixed, behavior corrections, collision fixes), then mark task as [x] in tasks.md_

- [ ] 6.7. Performance optimization pass
  - Files: Various (as needed)
  - Profile game performance with 50+ entities
  - Optimize collision detection (if needed)
  - Optimize particle system (if needed)
  - Optimize rendering (if needed)
  - Purpose: Ensure 60 FPS with high entity counts
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Visual Studio profiler, performance monitoring tools_
  - _Requirements: Requirement 11 (Performance Requirements)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Performance Engineer with expertise in game optimization and profiling | Task: Perform performance optimization pass following Requirement 11, ensuring 60 FPS with high entity counts | Restrictions: Do not change functionality, focus on performance improvements only, maintain code readability | Success: Game maintains 60 FPS with 50+ entities, collision detection is optimized, particle system is performant, rendering is efficient | Instructions: After completing this task, use log-implementation tool to record optimization details (profiling results, optimizations made, performance improvements), then mark task as [x] in tasks.md_

- [ ] 6.8. Code cleanup and documentation pass
  - Files: All files modified in project
  - Review all code for clarity and consistency
  - Add missing code comments
  - Update file headers
  - Remove debug code and commented-out code
  - Purpose: Ensure code quality and maintainability
  - Time Estimate: [2.5, 3.5] hours
  - _Leverage: Existing code comment style_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Senior Developer with expertise in code quality and documentation | Task: Perform code cleanup and documentation pass following Requirement 2, ensuring code quality and maintainability | Restrictions: Do not change functionality, maintain existing comment language consistency, follow project documentation standards | Success: All code is clean and well-documented, file headers are updated, no debug or commented-out code remains, code meets project quality standards | Instructions: After completing this task, use log-implementation tool to record cleanup activities (comments added, files cleaned, documentation updated), then mark task as [x] in tasks.md_

- [ ] 6.9. Comprehensive gameplay testing
  - Files: N/A (testing)
  - Play through 20+ waves
  - Test all enemy types in various combinations
  - Test all upgrades at high levels
  - Test game over and restart
  - Verify 10+ minute stable gameplay
  - Purpose: Ensure game meets MVP requirements
  - Time Estimate: [3.0, 4.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: All requirements (comprehensive verification)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in comprehensive gameplay testing and MVP verification | Task: Perform comprehensive gameplay testing covering all requirements, ensuring game meets MVP criteria | Restrictions: Do not modify code during testing, document all issues found, verify all acceptance criteria | Success: Game is playable for 10+ minutes without crashes, all enemy types work correctly, all upgrades work correctly, game over and restart work, MVP requirements are met | Instructions: After completing this task, use log-implementation tool to record testing results (gameplay sessions, features verified, MVP status), then mark task as [x] in tasks.md_

- [ ] 6.10. Edge case testing
  - Files: N/A (testing)
  - Test with maximum enemy count (50+)
  - Test with all upgrades at max level
  - Test rapid shop refresh
  - Test hexagon split with many entities
  - Test window management with many windows
  - Purpose: Ensure game handles edge cases
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: All requirements (edge case verification)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in edge case testing and stress testing | Task: Perform edge case testing covering extreme scenarios, ensuring game handles edge cases gracefully | Restrictions: Do not modify code during testing, document all edge case issues, test extreme scenarios | Success: Game handles maximum enemy count, max level upgrades work correctly, rapid shop refresh works, hexagon split doesn't break, window management is stable | Instructions: After completing this task, use log-implementation tool to record edge case testing results (scenarios tested, edge cases verified, issues found), then mark task as [x] in tasks.md_

- [ ] 6.11. Final integration testing
  - Files: N/A (testing)
  - Test all systems working together
  - Verify audio/visual sync throughout gameplay
  - Test performance under full load
  - Verify game feel and polish
  - Test on different screen resolutions (if possible)
  - Purpose: Final verification before submission
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Manual testing procedures_
  - _Requirements: All requirements (final verification)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Lead with expertise in final integration testing and release verification | Task: Perform final integration testing covering all systems, ensuring game is ready for submission | Restrictions: Do not modify code during testing, document any remaining issues, verify all requirements are met | Success: All systems work together seamlessly, audio/visual sync is perfect, performance is excellent, game feel is polished, game is ready for submission | Instructions: After completing this task, use log-implementation tool to record final testing results (integration verified, performance confirmed, submission readiness), then mark task as [x] in tasks.md_

- [ ] 6.12. Create submission documentation
  - Files: README.md, CHANGELOG.md (optional)
  - Document game features and controls
  - Document known issues (if any)
  - Document build instructions
  - Document testing results
  - Purpose: Provide documentation for submission
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing project documentation_
  - _Requirements: All requirements (documentation)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Technical Writer with expertise in game documentation and submission materials | Task: Create submission documentation covering game features, controls, and testing results, preparing for DFS2 submission | Restrictions: Follow DFS2 submission requirements, ensure documentation is clear and complete, include all required information | Success: Documentation is complete and clear, features are documented, controls are explained, build instructions are accurate, testing results are included | Instructions: After completing this task, use log-implementation tool to record documentation details (files created, content included, submission readiness), then mark task as [x] in tasks.md_

## Sprint 6 Summary

**Total Tasks:** 12
**Estimated Hours:** 28.5 - 40.0 hours (target: 24 hours)
**Key Deliverables:**
- ✅ Game over system implemented
- ✅ Game over UI with statistics
- ✅ Game restart functionality
- ✅ Game over audio/visual feedback
- ✅ Bug fixes (compilation, crashes, gameplay)
- ✅ Performance optimization
- ✅ Code cleanup and documentation
- ✅ Comprehensive testing (gameplay, edge cases, integration)
- ✅ Submission documentation
- ✅ Game ready for DFS2 submission

**Final MVP Status:**
- ✅ Compilation errors fixed
- ✅ 6 Tier 1 enemy types working
- ✅ 3 Tier 2 elite enemy types working
- ✅ 7 combat upgrades working
- ✅ Wave-based spawning system working
- ✅ Audio system (BGM + SFX) working
- ✅ Visual effects (particles) working
- ✅ Game over system working
- ✅ 10+ minute stable gameplay verified
- ✅ 60 FPS with 50+ entities verified
