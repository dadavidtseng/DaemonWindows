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
