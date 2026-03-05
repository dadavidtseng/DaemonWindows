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
