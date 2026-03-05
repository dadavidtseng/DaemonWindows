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
