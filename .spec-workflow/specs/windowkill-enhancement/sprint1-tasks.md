# Sprint 1 Tasks - Foundation (Weeks 1-2, 24 hours)

## Overview
Sprint 1 focuses on fixing compilation errors, code refactoring, and creating the foundation for wave and upgrade systems.

## Tasks

- [ ] 1.1. Fix OpenSSL linking errors in Game.vcxproj
  - Files: Code/Game/Game.vcxproj
  - Add OpenSSL library path to AdditionalLibraryDirectories
  - Add libcrypto.lib and libssl.lib to AdditionalDependencies
  - Purpose: Resolve 26 unresolved external symbol errors
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: C:\\GitHub\\Protogame2D\\Code\\Game\\Game.vcxproj (reference pattern)_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Build Engineer with expertise in Visual Studio project configuration and C++ linking | Task: Fix OpenSSL linking errors by adding library paths and dependencies to Game.vcxproj following Requirement 1, referencing the working configuration from Protogame2D project | Restrictions: Do not modify Engine project files, maintain existing library order, ensure both Debug and Release configurations are updated | Success: Project compiles without LNK2019 errors for OpenSSL functions, both Debug and Release builds succeed, no new linking errors introduced | Instructions: After completing this task, use log-implementation tool to record what was changed (vcxproj modifications, library paths added), then mark task as [x] in tasks.md_

- [ ] 1.2. Fix g_widgetSubsystem duplicate symbol error
  - Files: Code/Game/Framework/GameCommon.hpp, Code/Game/Framework/App.cpp
  - Verify extern declaration in GameCommon.hpp
  - Verify single definition in App.cpp
  - Check Engine's EngineCommon.cpp for duplicate definition
  - Purpose: Resolve LNK2005 duplicate symbol error
  - Time Estimate: [0.5, 1.5] hours
  - _Leverage: C:\\GitHub\\Protogame2D project structure (reference pattern)_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in linkage and symbol resolution | Task: Fix g_widgetSubsystem duplicate symbol error by ensuring proper extern declaration pattern following Requirement 1, referencing Protogame2D's working pattern | Restrictions: Do not remove g_widgetSubsystem usage, maintain existing code functionality, only fix declaration/definition pattern | Success: Project compiles without LNK2005 error, g_widgetSubsystem is accessible throughout codebase, no runtime errors related to widget subsystem | Instructions: After completing this task, use log-implementation tool to record the fix (extern pattern used, files modified), then mark task as [x] in tasks.md_

- [ ] 1.3. Verify compilation success and test build
  - Files: N/A (build verification)
  - Build project in Debug configuration
  - Build project in Release configuration
  - Run executable to verify no missing DLL errors
  - Purpose: Confirm all compilation errors are resolved
  - Time Estimate: [0.5, 1.0] hours
  - _Leverage: Visual Studio build system_
  - _Requirements: Requirement 1 (Fix Compilation Errors)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: QA Engineer with expertise in build verification and testing | Task: Verify compilation success by building both Debug and Release configurations and testing executable launch following Requirement 1 | Restrictions: Do not modify code during verification, document any remaining issues, ensure clean build from scratch | Success: Both Debug and Release builds complete without errors or warnings, executable launches successfully, no missing DLL errors, game reaches attract mode | Instructions: After completing this task, use log-implementation tool to record build verification results (configurations tested, any issues found), then mark task as [x] in tasks.md_

- [ ] 1.4. Code refactoring - Extract collision handling logic
  - Files: Code/Game/Gameplay/Game.cpp, Code/Game/Gameplay/CollisionUtils.cpp/hpp (new)
  - Extract collision detection logic from Game::HandleEntityCollision()
  - Create CollisionUtils namespace with helper functions
  - Improve code readability and organization
  - Purpose: Clean up Game.cpp and make collision logic reusable
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing DoDiscsOverlap2D function_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Software Engineer with expertise in code refactoring and C++ best practices | Task: Extract collision handling logic from Game.cpp into reusable CollisionUtils following Requirement 2, improving code organization and readability | Restrictions: Do not change collision behavior, maintain existing collision detection accuracy, ensure all entity types still collide correctly | Success: Collision logic is extracted into CollisionUtils namespace, Game.cpp is cleaner and more readable, all existing collisions still work correctly, no regression in collision detection | Instructions: After completing this task, use log-implementation tool to record refactoring details (functions extracted, new files created, collision logic preserved), then mark task as [x] in tasks.md_

- [ ] 1.5. Code refactoring - Improve entity spawning organization
  - Files: Code/Game/Gameplay/Game.cpp
  - Organize SpawnEntity() method with better structure
  - Add comments explaining spawn logic
  - Prepare for wave-based spawning integration
  - Purpose: Make spawn code more maintainable and extensible
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing SpawnEntity() method_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Software Engineer with expertise in code organization and documentation | Task: Refactor SpawnEntity() method for better organization and prepare for wave-based spawning following Requirement 2 | Restrictions: Do not change spawning behavior yet, maintain existing spawn functionality, only improve organization and add comments | Success: SpawnEntity() is well-organized with clear structure, comments explain spawn logic, code is ready for wave system integration, no change in current spawning behavior | Instructions: After completing this task, use log-implementation tool to record refactoring changes (structure improvements, comments added), then mark task as [x] in tasks.md_

- [ ] 1.6. Create WaveManager class skeleton
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp (new)
  - Create WaveManager class with basic structure
  - Add member variables for wave state
  - Add placeholder methods (Update, StartWave, CompleteWave)
  - Purpose: Establish foundation for wave system
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game systems and class design | Task: Create WaveManager class skeleton with basic structure and placeholder methods following Requirement 3 and Design Component 1 | Restrictions: Do not implement full wave logic yet, focus on class structure and interface, ensure class integrates with existing Game class | Success: WaveManager class compiles successfully, has clear interface for wave management, integrates with Game class without errors, ready for full implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, methods defined, integration points), then mark task as [x] in tasks.md_

- [ ] 1.7. Create UpgradeManager class skeleton
  - Files: Code/Game/Gameplay/UpgradeManager.cpp/hpp (new)
  - Create UpgradeManager class with basic structure
  - Define eUpgradeType enum
  - Add member variables for upgrade tracking
  - Add placeholder methods (PurchaseUpgrade, GetUpgradeLevel, GetUpgradeCost)
  - Purpose: Establish foundation for upgrade system
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing Shop class structure_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game systems and data structures | Task: Create UpgradeManager class skeleton with upgrade tracking structure following Requirement 7 and Design Component 2 | Restrictions: Do not implement full upgrade logic yet, focus on class structure and data models, ensure class can be integrated with Player and Shop | Success: UpgradeManager class compiles successfully, eUpgradeType enum defines all 7 upgrade types, has clear interface for upgrade management, ready for full implementation | Instructions: After completing this task, use log-implementation tool to record class creation (files created, enum defined, methods declared), then mark task as [x] in tasks.md_

- [ ] 1.8. Integrate WaveManager with Game class
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add WaveManager member variable to Game class
  - Initialize WaveManager in Game constructor
  - Call WaveManager::Update() in Game::Update()
  - Purpose: Connect wave system to main game loop
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game architecture and system integration | Task: Integrate WaveManager with Game class following Requirement 3, connecting wave system to main game loop | Restrictions: Do not break existing game functionality, ensure proper initialization and cleanup, maintain game state management | Success: WaveManager is properly integrated with Game class, Update() is called each frame, no crashes or errors, game still functions normally | Instructions: After completing this task, use log-implementation tool to record integration details (member added, initialization code, update call), then mark task as [x] in tasks.md_

- [ ] 1.9. Integrate UpgradeManager with Game class
  - Files: Code/Game/Gameplay/Game.cpp/hpp
  - Add UpgradeManager member variable to Game class
  - Initialize UpgradeManager in Game constructor
  - Provide access method for Player and Shop
  - Purpose: Connect upgrade system to game
  - Time Estimate: [1.0, 2.0] hours
  - _Leverage: Existing Game class structure_
  - _Requirements: Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in game architecture and system integration | Task: Integrate UpgradeManager with Game class following Requirement 7, making it accessible to Player and Shop | Restrictions: Do not break existing game functionality, ensure proper initialization and cleanup, maintain encapsulation | Success: UpgradeManager is properly integrated with Game class, accessible via getter method, no crashes or errors, ready for Player and Shop integration | Instructions: After completing this task, use log-implementation tool to record integration details (member added, initialization code, accessor method), then mark task as [x] in tasks.md_

- [ ] 1.10. Create EnemyUtils namespace with shared behaviors
  - Files: Code/Game/Gameplay/EnemyUtils.cpp/hpp (new)
  - Create EnemyUtils namespace
  - Implement ChasePlayer() utility function
  - Implement GetDirectionToPlayer() utility function
  - Implement GetRandomSpawnPosition() utility function
  - Purpose: Provide reusable enemy behavior functions
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing enemy movement code from Triangle.cpp_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in AI behaviors and utility functions | Task: Create EnemyUtils namespace with shared enemy behavior functions following Requirement 4 and Design Component 3 | Restrictions: Keep functions pure and stateless, ensure functions work for all enemy types, maintain performance efficiency | Success: EnemyUtils namespace compiles successfully, utility functions are well-tested and reusable, functions provide correct movement calculations, ready for enemy implementation | Instructions: After completing this task, use log-implementation tool to record utility creation (namespace created, functions implemented, usage examples), then mark task as [x] in tasks.md_

- [ ] 1.11. Add event system events for wave and upgrade systems
  - Files: Code/Game/Gameplay/Game.cpp, Code/Game/Gameplay/WaveManager.cpp, Code/Game/Gameplay/UpgradeManager.cpp
  - Define new event names: OnWaveStart, OnWaveComplete, OnBossSpawn, OnUpgradePurchased
  - Add event firing in WaveManager and UpgradeManager
  - Add event subscription in Game class
  - Purpose: Enable decoupled communication for wave and upgrade events
  - Time Estimate: [1.5, 2.5] hours
  - _Leverage: Existing EventSystem usage (OnGameStateChanged, OnCollisionEnter)_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System), Requirement 7 (Combat Upgrade System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: C++ Developer with expertise in event-driven architecture and observer pattern | Task: Add event system events for wave and upgrade systems following Requirements 3 and 7, using existing EventSystem patterns | Restrictions: Follow existing event naming conventions, ensure proper event argument passing, do not break existing event subscriptions | Success: New events are properly defined and fired, event subscriptions work correctly, events enable decoupled communication, no interference with existing events | Instructions: After completing this task, use log-implementation tool to record event implementation (events defined, firing locations, subscription handlers), then mark task as [x] in tasks.md_

- [ ] 1.12. Code cleanup and documentation
  - Files: All files modified in Sprint 1
  - Add code comments explaining new systems
  - Update file headers with descriptions
  - Remove any debug code or commented-out code
  - Purpose: Ensure code is clean and well-documented
  - Time Estimate: [1.5, 2.0] hours
  - _Leverage: Existing code comment style_
  - _Requirements: Requirement 2 (Code Refactoring and Organization)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Senior Developer with expertise in code documentation and best practices | Task: Clean up and document all code modified in Sprint 1 following Requirement 2, ensuring code quality and maintainability | Restrictions: Do not change functionality, maintain existing comment language consistency, follow project documentation standards | Success: All new code has clear comments, file headers are updated, no debug or commented-out code remains, code meets project quality standards | Instructions: After completing this task, use log-implementation tool to record cleanup activities (comments added, files cleaned, documentation updated), then mark task as [x] in tasks.md_

- [ ] 1.13. Sprint 1 testing and verification
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

**Dependencies for Sprint 2:**
- WaveManager must be integrated before implementing wave-based spawning
- UpgradeManager must be integrated before implementing combat upgrades
- EnemyUtils must be ready before implementing new enemy types
