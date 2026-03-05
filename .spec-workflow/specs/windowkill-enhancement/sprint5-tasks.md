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
