# Sprint 2 Tasks - Tier 1 Enemies (Weeks 3-4, 24 hours)

## Overview
Sprint 2 focuses on implementing 3 new Tier 1 enemy types and redesigning the 3 existing enemy types with improved behaviors.

## Tasks

- [ ] 2.1. Redesign Triangle enemy behavior
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

- [ ] 2.2. Redesign Circle enemy behavior
  - Files: Code/Game/Gameplay/Circle.cpp/hpp
  - Implement orbital movement using EnemyUtils::OrbitPlayer()
  - Add angle tracking for smooth orbiting
  - Update health and speed values
  - Purpose: Make Circle a unique "orbiter" enemy type
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: EnemyUtils::OrbitPlayer(), existing Circle class_
  - _Requirements: Requirement 4 (Tier 1 Enemy Types - Circle Redesign)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in circular motion and enemy behaviors | Task: Redesign Circle enemy with orbital movement following Requirement 4 acceptance criteria for Circle, implementing smooth circular patterns | Restrictions: Maintain existing Entity inheritance, keep separate window functionality, ensure orbit radius is appropriate for gameplay | Success: Circle orbits player in smooth circular pattern, movement is predictable but challenging, health and speed are balanced, separate window displays correctly | Instructions: After completing this task, use log-implementation tool to record redesign details (orbital behavior, angle tracking, movement parameters), then mark task as [x] in tasks.md_

- [ ] 2.3. Redesign Octagon enemy behavior
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

- [ ] 2.4. Implement Square enemy (Tanky)
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

- [ ] 2.5. Implement Pentagon enemy (Fast Zigzag)
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

- [ ] 2.6. Implement Hexagon enemy (Splits on Death)
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

- [x] 2.7. Add enemy type spawn weights to WaveManager
  - Files: Code/Game/Gameplay/WaveManager.cpp/hpp
  - Define spawn weight system for enemy types
  - Create spawn tables for Tier 1 enemies
  - Implement random enemy selection based on weights
  - Purpose: Enable controlled random enemy spawning
  - Time Estimate: [2.0, 3.0] hours
  - _Leverage: Existing random number generator (g_rng)_
  - _Requirements: Requirement 3 (Wave-Based Enemy Spawning System)_
  - _Prompt: Implement the task for spec windowkill-enhancement, first run spec-workflow-guide to get the workflow guide then implement the task: Role: Game Developer with expertise in procedural generation and spawn systems | Task: Implement spawn weight system in WaveManager following Requirement 3, enabling controlled random enemy selection | Restrictions: Use existing g_rng for randomization, ensure spawn weights are balanced, maintain performance with frequent random selections | Success: Spawn weight system works correctly, enemy types spawn with appropriate frequency, random selection is fair and balanced, system is extensible for Tier 2 enemies | Instructions: After completing this task, use log-implementation tool to record spawn system details (weight system design, spawn tables, selection algorithm), then mark task as [x] in tasks.md_

- [x] 2.8. Update Game::SpawnEntity() to use enemy types
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
