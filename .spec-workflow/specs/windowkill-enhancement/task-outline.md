# Task Outline - Windowkill Enhancement

## Project Overview
- **Total Sprints:** 6
- **Total Tasks:** 71
- **Timeline:** 12 weeks (2 weeks per sprint)
- **Effort:** 12 hours per week = 24 hours per sprint

---

## Sprint 1: Foundation (Weeks 1-2, 24 hours)

- 1.1. Fix OpenSSL linking errors in Game.vcxproj [1.0-2.0h]
- 1.2. Fix g_widgetSubsystem duplicate symbol error [0.5-1.5h]
- 1.3. Verify compilation success and test build [0.5-1.0h]
- 1.4. Code refactoring - Extract collision handling logic [2.0-3.0h]
- 1.5. Code refactoring - Improve entity spawning organization [1.0-2.0h]
- 1.6. Create WaveManager class skeleton [2.0-3.0h]
- 1.7. Create UpgradeManager class skeleton [2.0-3.0h]
- 1.8. Integrate WaveManager with Game class [1.0-2.0h]
- 1.9. Integrate UpgradeManager with Game class [1.0-2.0h]
- 1.10. Create EnemyUtils namespace with shared behaviors [2.0-3.0h]
- 1.11. Add event system events for wave and upgrade systems [1.5-2.5h]
- 1.12. Code cleanup and documentation [1.5-2.0h]
- 1.13. Sprint 1 testing and verification [2.0-3.0h]

**Total Tasks:** 13 | **Estimated Hours:** 20.5-31.0

---

## Sprint 2: Tier 1 Enemies (Weeks 3-4, 24 hours)

- 2.1. Redesign Triangle enemy behavior [2.0-3.0h]
- 2.2. Redesign Circle enemy behavior [2.0-3.0h]
- 2.3. Redesign Octagon enemy behavior [3.0-4.0h]
- 2.4. Implement Square enemy (Tanky) [2.5-3.5h]
- 2.5. Implement Pentagon enemy (Fast Zigzag) [2.5-3.5h]
- 2.6. Implement Hexagon enemy (Splits on Death) [3.5-4.5h]
- 2.7. Add enemy type spawn weights to WaveManager [2.0-3.0h]
- 2.8. Update Game::SpawnEntity() to use enemy types [1.5-2.5h]
- 2.9. Update collision handling for new enemy types [1.5-2.5h]
- 2.10. Add visual variety to enemy types [1.0-2.0h]
- 2.11. Test all 6 Tier 1 enemy types [2.0-3.0h]
- 2.12. Sprint 2 integration testing [1.5-2.5h]

**Total Tasks:** 12 | **Estimated Hours:** 23.5-35.5

---

## Sprint 3: Tier 2 Enemies & Wave System (Weeks 5-6, 24 hours)

- 3.1. Implement Spawner enemy [3.0-4.0h]
- 3.2. Implement Teleporter enemy [2.5-3.5h]
- 3.3. Implement Shielded enemy [3.0-4.0h]
- 3.4. Complete WaveManager wave progression logic [3.0-4.0h]
- 3.5. Implement boss spawn system in WaveManager [2.5-3.5h]
- 3.6. Add wave UI display [2.0-3.0h]
- 3.7. Implement wave completion rewards [2.0-3.0h]
- 3.8. Update spawn tables for Tier 2 enemies [1.5-2.5h]
- 3.9. Test Tier 2 enemy behaviors [2.0-3.0h]
- 3.10. Test wave progression system [2.0-3.0h]
- 3.11. Sprint 3 integration testing [1.5-2.5h]

**Total Tasks:** 11 | **Estimated Hours:** 25.5-36.5

---

## Sprint 4: Combat Upgrades (Weeks 7-8, 24 hours)

- 4.1. Complete UpgradeManager implementation [3.0-4.0h]
- 4.2. Extend Shop with upgrade display [3.0-4.0h]
- 4.3. Implement shop refresh system [2.0-3.0h]
- 4.4. Extend Player with upgrade stat application [3.0-4.0h]
- 4.5. Extend Bullet with piercing behavior [2.5-3.5h]
- 4.6. Extend Bullet with homing behavior [2.5-3.5h]
- 4.7. Extend Bullet with size scaling [1.5-2.5h]
- 4.8. Add visual feedback for active upgrades [2.0-3.0h]
- 4.9. Test all 7 upgrade types [2.5-3.5h]
- 4.10. Test shop refresh system [1.0-2.0h]
- 4.11. Sprint 4 integration testing [2.0-3.0h]

**Total Tasks:** 11 | **Estimated Hours:** 25.0-36.0

---

## Sprint 5: Audio & Visual Polish (Weeks 9-10, 24 hours)

- 5.1. Create AudioManager class [2.0-3.0h]
- 5.2. Implement wave BGM system [2.5-3.5h]
- 5.3. Implement enemy SFX system [3.0-4.0h]
- 5.4. Implement combat SFX [2.0-3.0h]
- 5.5. Implement boss music system [2.0-3.0h]
- 5.6. Create ParticleSystem class [3.0-4.0h]
- 5.7. Implement death particle effects [2.0-3.0h]
- 5.8. Implement impact particle effects [2.0-3.0h]
- 5.9. Implement damage particle effects [1.5-2.5h]
- 5.10. Test audio system [2.0-3.0h]
- 5.11. Test particle system [1.5-2.5h]
- 5.12. Sprint 5 integration testing [1.5-2.5h]

**Total Tasks:** 12 | **Estimated Hours:** 25.5-37.0

---

## Sprint 6: Game Over & Polish (Weeks 11-12, 24 hours)

- 6.1. Implement game over system [2.5-3.5h]
- 6.2. Implement game over UI [2.0-3.0h]
- 6.3. Implement game restart functionality [2.0-3.0h]
- 6.4. Add game over audio/visual feedback [1.5-2.5h]
- 6.5. Bug fixing pass 1 - Compilation and crashes [3.0-4.0h]
- 6.6. Bug fixing pass 2 - Gameplay issues [3.0-4.0h]
- 6.7. Performance optimization pass [3.0-4.0h]
- 6.8. Code cleanup and documentation pass [2.5-3.5h]
- 6.9. Comprehensive gameplay testing [3.0-4.0h]
- 6.10. Edge case testing [2.0-3.0h]
- 6.11. Final integration testing [2.0-3.0h]
- 6.12. Create submission documentation [1.5-2.5h]

**Total Tasks:** 12 | **Estimated Hours:** 28.5-40.0

---

## Summary

| Sprint | Focus | Tasks | Est. Hours |
|--------|-------|-------|------------|
| Sprint 1 | Foundation | 13 | 20.5-31.0 |
| Sprint 2 | Tier 1 Enemies | 12 | 23.5-35.5 |
| Sprint 3 | Tier 2 Enemies & Wave System | 11 | 25.5-36.5 |
| Sprint 4 | Combat Upgrades | 11 | 25.0-36.0 |
| Sprint 5 | Audio & Visual Polish | 12 | 25.5-37.0 |
| Sprint 6 | Game Over & Polish | 12 | 28.5-40.0 |
| **Total** | | **71** | **148.5-216.0** |
