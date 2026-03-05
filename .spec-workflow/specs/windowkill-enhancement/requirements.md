# Requirements Document - Windowkill Enhancement

## Introduction

This document outlines the requirements for enhancing the DaemonWindows project, a Windowkill-inspired multi-window action game. The project is being developed as part of the DFS2 (Development for Simulation 2) academic course at SMU Guildhall. The enhancement focuses on expanding the enemy system, implementing a comprehensive combat upgrade system, adding audio-visual polish, and fixing existing compilation errors.

**Project Context:**
- **Timeline:** 12 weeks (until May 12, 2026)
- **Effort:** 12 hours per week (144 total hours)
- **Sprint Duration:** 2 weeks (24 hours per sprint)
- **Total Sprints:** 6 main sprints + 1-2 stretch goal sprints

**Academic Requirements:**
- This is a continuation of DFS1 (Option A: Continue DFS1 to enhance Windowkill's enemy and shop system)
- Manual testing will be performed to verify functionality
- No automated test framework required
- No formal documentation deliverables beyond code comments

**Minimum Viable Product (MVP):**
- Fix all compilation errors
- 6 enemy types working (Tier 1 complete)
- 7 combat upgrades working
- Wave-based spawning system
- 10 minutes of stable gameplay

## Alignment with Product Vision

DaemonWindows is an experimental action game that transforms the Windows desktop environment into a dynamic gaming battlefield. This enhancement aligns with the product vision by:

1. **Deepening Core Gameplay:** Expanding enemy variety and combat upgrades creates more strategic depth and replayability
2. **Windowkill Inspiration:** Implementing mechanics inspired by the successful indie game Windowkill while maintaining unique identity
3. **Technical Excellence:** Fixing compilation errors and improving code organization demonstrates professional development practices
4. **Academic Achievement:** Meeting DFS2 requirements while showcasing technical complexity and game design skills

## Requirements

### Requirement 1: Fix Compilation Errors

**User Story:** As a developer, I want the project to compile without errors, so that I can build and run the game successfully.

#### Acceptance Criteria

1. WHEN the project is built in Visual Studio THEN the system SHALL compile without LNK2005 or LNK2019 errors
2. WHEN OpenSSL libraries are required THEN the system SHALL link libcrypto.lib and libssl.lib from Engine/Code/ThirdParty/openssl/lib/x64/$(Configuration)
3. WHEN g_widgetSubsystem is referenced THEN the system SHALL use extern declaration pattern to avoid duplicate symbol errors
4. WHEN the game executable is run THEN the system SHALL launch without missing DLL errors

**Technical Details:**
- Add OpenSSL library path to AdditionalLibraryDirectories in Game.vcxproj
- Add libcrypto.lib and libssl.lib to AdditionalDependencies
- Verify g_widgetSubsystem is declared as extern in GameCommon.hpp and defined only once in App.cpp
- Reference Protogame2D project for correct configuration pattern

### Requirement 2: Code Refactoring and Organization

**User Story:** As a developer, I want clean and readable code, so that I can maintain and extend the codebase efficiently.

#### Acceptance Criteria

1. WHEN code is reviewed THEN the system SHALL follow consistent naming conventions and formatting
2. WHEN files are examined THEN each file SHALL have a single, well-defined purpose
3. WHEN code comments are present THEN they SHALL be in the same language as the existing codebase
4. WHEN duplicate code is identified THEN it SHALL be refactored into reusable functions or classes
5. WHEN entity behaviors are implemented THEN they SHALL use the existing inheritance-based entity system

**Technical Details:**
- Maintain current Entity base class architecture
- Improve code organization and readability without major architectural changes
- Keep existing multi-window system and collision detection intact
- Preserve current game state management (ATTRACT, GAME, SHOP)

### Requirement 3: Wave-Based Enemy Spawning System

**User Story:** As a player, I want enemies to spawn in waves with increasing difficulty, so that the game provides progressive challenge.

#### Acceptance Criteria

1. WHEN a wave starts THEN the system SHALL spawn regular enemies gradually using the existing spawn timer
2. WHEN all regular enemies are killed THEN the system SHALL spawn a boss enemy for that wave
3. WHEN the boss is killed THEN the system SHALL complete the wave and start the next wave
4. WHEN wave number increases THEN the system SHALL spawn more enemies and stronger enemy types
5. WHEN wave 1 starts THEN the system SHALL spawn only Tier 1 enemies (Triangle, Circle, Octagon, Square, Pentagon, Hexagon)
6. WHEN waves 2-5 are active THEN the system SHALL spawn mix of Tier 1 and Tier 2 enemies
7. WHEN wave 6+ is active THEN the system SHALL spawn all enemy types including Tier 3 bosses (if implemented)
8. WHEN a wave boss spawns THEN it SHALL be a Tier 2 enemy (Spawner, Teleporter, Shielded, or Kamikaze) for waves 1-5
9. WHEN wave 6+ boss spawns THEN it SHALL be a Tier 3 boss (Mini-Boss or Final Boss) if stretch goals are implemented

**Technical Details:**
- Extend existing m_spawnTimer and m_spawnInterval system in Game.cpp
- Implement wave counter and wave state management
- Define enemy spawn tables per wave (which enemy types can spawn)
- Implement boss spawn logic after regular enemies are cleared
- Scale difficulty by increasing enemy count and introducing stronger types

### Requirement 4: Tier 1 Enemy Types (6 types - MVP Required)

**User Story:** As a player, I want to encounter diverse regular enemies with different behaviors, so that combat remains engaging and requires different strategies.

#### Acceptance Criteria - Triangle (Redesign)

1. WHEN Triangle spawns THEN it SHALL appear in a separate window
2. WHEN Triangle is active THEN it SHALL chase the player with moderate speed
3. WHEN Triangle has health THEN it SHALL display health value on its window
4. WHEN Triangle is killed THEN it SHALL drop coins based on its health value
5. WHEN Triangle dies THEN it SHALL play death sound effect and particle effect

#### Acceptance Criteria - Circle (Redesign)

1. WHEN Circle spawns THEN it SHALL appear in a separate window
2. WHEN Circle is active THEN it SHALL move in circular/orbital patterns around the player
3. WHEN Circle has health THEN it SHALL display health value on its window
4. WHEN Circle is killed THEN it SHALL drop coins based on its health value
5. WHEN Circle dies THEN it SHALL play death sound effect and particle effect

#### Acceptance Criteria - Octagon (Redesign)

1. WHEN Octagon spawns THEN it SHALL appear in a separate window
2. WHEN Octagon is active THEN it SHALL maintain distance and shoot projectiles at the player
3. WHEN Octagon has health THEN it SHALL display health value on its window
4. WHEN Octagon is killed THEN it SHALL drop coins based on its health value
5. WHEN Octagon dies THEN it SHALL play death sound effect and particle effect

#### Acceptance Criteria - Square (NEW - Tanky)

1. WHEN Square spawns THEN it SHALL appear in a separate window
2. WHEN Square is active THEN it SHALL move slowly toward the player
3. WHEN Square spawns THEN it SHALL have high health (3-5x normal enemy health)
4. WHEN Square has health THEN it SHALL display health value on its window
5. WHEN Square is killed THEN it SHALL drop more coins than regular enemies
6. WHEN Square dies THEN it SHALL play death sound effect and particle effect

#### Acceptance Criteria - Pentagon (NEW - Fast Zigzag)

1. WHEN Pentagon spawns THEN it SHALL appear in a separate window
2. WHEN Pentagon is active THEN it SHALL move in fast zigzag patterns toward the player
3. WHEN Pentagon has health THEN it SHALL display health value on its window
4. WHEN Pentagon is killed THEN it SHALL drop coins based on its health value
5. WHEN Pentagon dies THEN it SHALL play death sound effect and particle effect

#### Acceptance Criteria - Hexagon (NEW - Splits on Death)

1. WHEN Hexagon spawns THEN it SHALL appear in a separate window
2. WHEN Hexagon is active THEN it SHALL move toward the player with moderate speed
3. WHEN Hexagon is killed THEN it SHALL split into 2-3 smaller Hexagon entities
4. WHEN smaller Hexagon entities are killed THEN they SHALL NOT split further
5. WHEN Hexagon has health THEN it SHALL display health value on its window
6. WHEN Hexagon dies THEN it SHALL play death sound effect and particle effect

**Technical Details:**
- All Tier 1 enemies inherit from Entity base class
- Each enemy type has unique movement behavior in Update() method
- Health values scale with wave number
- Coin drops calculated based on enemy health and type
- Visual design should reference Windowkill art style (open source assets available)

### Requirement 5: Tier 2 Elite Enemy Types (4 types - 3 MVP Required, 1 Stretch)

**User Story:** As a player, I want to face elite enemies with special abilities, so that I must adapt my tactics and feel challenged.

#### Acceptance Criteria - Spawner (NEW - Creates Enemies)

1. WHEN Spawner spawns THEN it SHALL appear in a separate window
2. WHEN Spawner is active THEN it SHALL periodically spawn Tier 1 enemies (every 5-10 seconds)
3. WHEN Spawner spawns enemies THEN it SHALL create Triangle, Circle, or Octagon entities
4. WHEN Spawner has health THEN it SHALL display health value on its window
5. WHEN Spawner is killed THEN it SHALL stop spawning and drop bonus coins
6. WHEN Spawner dies THEN it SHALL play death sound effect and particle effect
7. WHEN Spawner is used as wave boss THEN it SHALL have increased health and faster spawn rate

#### Acceptance Criteria - Teleporter (NEW - Blinks Around)

1. WHEN Teleporter spawns THEN it SHALL appear in a separate window
2. WHEN Teleporter is active THEN it SHALL teleport to random positions every 2-4 seconds
3. WHEN Teleporter teleports THEN it SHALL play teleport sound effect and visual effect
4. WHEN Teleporter has health THEN it SHALL display health value on its window
5. WHEN Teleporter is killed THEN it SHALL drop bonus coins
6. WHEN Teleporter dies THEN it SHALL play death sound effect and particle effect
7. WHEN Teleporter is used as wave boss THEN it SHALL have increased health and teleport more frequently

#### Acceptance Criteria - Shielded (NEW - Multiple Hits Required)

1. WHEN Shielded spawns THEN it SHALL appear in a separate window with shield visual indicator
2. WHEN Shielded is hit by bullets THEN it SHALL require multiple hits to break shield before taking health damage
3. WHEN Shielded shield is active THEN it SHALL display shield health on its window
4. WHEN Shielded shield breaks THEN it SHALL play shield break sound and visual effect
5. WHEN Shielded has health THEN it SHALL display health value on its window
6. WHEN Shielded is killed THEN it SHALL drop bonus coins
7. WHEN Shielded dies THEN it SHALL play death sound effect and particle effect
8. WHEN Shielded is used as wave boss THEN it SHALL have increased shield and health values

#### Acceptance Criteria - Kamikaze (STRETCH - Rush and Explode)

1. WHEN Kamikaze spawns THEN it SHALL appear in a separate window
2. WHEN Kamikaze detects player THEN it SHALL rush toward player at high speed
3. WHEN Kamikaze reaches player THEN it SHALL explode dealing area damage
4. WHEN Kamikaze explodes THEN it SHALL play explosion sound and visual effect
5. WHEN Kamikaze is killed before reaching player THEN it SHALL drop coins
6. WHEN Kamikaze is used as wave boss THEN it SHALL have increased health and explosion radius

**Technical Details:**
- Tier 2 enemies can serve as wave bosses with enhanced stats
- Spawner needs timer system for enemy creation
- Teleporter needs position randomization within screen bounds
- Shielded needs shield health system separate from entity health
- Kamikaze needs proximity detection and explosion damage system

### Requirement 6: Tier 3 Boss Enemy Types (2 types - STRETCH GOALS)

**User Story:** As a player, I want to face epic boss battles, so that I experience climactic challenges and memorable moments.

#### Acceptance Criteria - Mini-Boss (STRETCH)

1. WHEN Mini-Boss spawns THEN it SHALL appear in a separate dedicated window
2. WHEN Mini-Boss is active THEN it SHALL have multiple attack phases
3. WHEN Mini-Boss health drops below thresholds THEN it SHALL change attack patterns
4. WHEN Mini-Boss has health THEN it SHALL display health bar on its window
5. WHEN Mini-Boss is killed THEN it SHALL drop significant coins
6. WHEN Mini-Boss dies THEN it SHALL play boss death sound and particle effect

#### Acceptance Criteria - Final Boss (STRETCH)

1. WHEN Final Boss spawns THEN it SHALL appear in a separate dedicated window
2. WHEN Final Boss is active THEN it SHALL have complex attack patterns
3. WHEN Final Boss health drops below thresholds THEN it SHALL change attack patterns and spawn minions
4. WHEN Final Boss has health THEN it SHALL display health bar on its window
5. WHEN Final Boss is killed THEN it SHALL trigger victory condition
6. WHEN Final Boss dies THEN it SHALL play boss death sound and particle effect

**Technical Details:**
- Tier 3 bosses only appear in wave 6+ as wave bosses
- Require phase system based on health percentage
- Need dedicated boss window management
- Complex attack patterns may include projectile patterns, minion spawning, area attacks

### Requirement 7: Combat Upgrade System (7 upgrades - ALL MVP Required)

**User Story:** As a player, I want to purchase combat upgrades that stack infinitely, so that I can customize my playstyle and become progressively more powerful.

#### Acceptance Criteria - Fire Rate Increase

1. WHEN player purchases fire rate upgrade THEN the system SHALL decrease bullet fire cooldown
2. WHEN fire rate is upgraded THEN bullets SHALL fire more frequently
3. WHEN fire rate upgrade is purchased multiple times THEN the effect SHALL stack
4. WHEN fire rate is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN fire rate is upgraded THEN player SHALL see visual feedback (faster bullet trails)

#### Acceptance Criteria - Damage Multiplier

1. WHEN player purchases damage upgrade THEN the system SHALL increase bullet damage
2. WHEN damage is upgraded THEN enemies SHALL take more damage per hit
3. WHEN damage upgrade is purchased multiple times THEN the effect SHALL stack
4. WHEN damage is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN damage is upgraded THEN bullets SHALL have visual feedback (larger/brighter bullets)

#### Acceptance Criteria - Projectile Count

1. WHEN player purchases projectile count upgrade THEN the system SHALL increase bullets fired per shot
2. WHEN projectile count is 2 THEN player SHALL fire 2 bullets in spread pattern
3. WHEN projectile count is 3 THEN player SHALL fire 3 bullets in spread pattern
4. WHEN projectile count is 4+ THEN player SHALL fire 4+ bullets in spread pattern
5. WHEN projectile count is upgraded THEN the cost SHALL increase by the pricing formula
6. WHEN projectile count is upgraded THEN player SHALL see multiple bullet trails

#### Acceptance Criteria - Bullet Spread/Pattern

1. WHEN player purchases spread upgrade THEN the system SHALL modify bullet firing pattern
2. WHEN spread is upgraded THEN bullets SHALL fire in wider arc or different patterns
3. WHEN spread upgrade is purchased multiple times THEN the pattern SHALL evolve
4. WHEN spread is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN spread is upgraded THEN player SHALL see visual feedback in bullet trajectories

#### Acceptance Criteria - Bullet Size

1. WHEN player purchases bullet size upgrade THEN the system SHALL increase bullet collision radius
2. WHEN bullet size is upgraded THEN bullets SHALL appear visually larger
3. WHEN bullet size upgrade is purchased multiple times THEN the effect SHALL stack
4. WHEN bullet size is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN bullet size is upgraded THEN bullets SHALL have larger visual representation

#### Acceptance Criteria - Piercing Bullets

1. WHEN player purchases piercing upgrade THEN bullets SHALL pass through enemies
2. WHEN piercing bullets hit enemies THEN they SHALL continue traveling and hit additional enemies
3. WHEN piercing upgrade is purchased multiple times THEN bullets SHALL pierce more enemies
4. WHEN piercing is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN piercing is upgraded THEN bullets SHALL have visual feedback (glowing trail effect)

#### Acceptance Criteria - Homing Bullets

1. WHEN player purchases homing upgrade THEN bullets SHALL track nearest enemy
2. WHEN homing bullets are fired THEN they SHALL curve toward enemy positions
3. WHEN homing upgrade is purchased multiple times THEN homing strength SHALL increase
4. WHEN homing is upgraded THEN the cost SHALL increase by the pricing formula
5. WHEN homing is upgraded THEN bullets SHALL have visual feedback (particle trail following curve)

**Technical Details:**
- All upgrades modify player stats or bullet behavior
- Upgrades stack infinitely (no max level)
- Pricing formula: Base cost increases with each purchase (specific formula TBD in design phase)
- Visual feedback depends on upgrade type (size, color, particles, trails)
- Shop displays 3 random upgrades at a time (existing shop UI pattern)

### Requirement 8: Shop Refresh System

**User Story:** As a player, I want to refresh the shop to see different upgrade options, so that I can find the upgrades I want.

#### Acceptance Criteria

1. WHEN player opens shop THEN the system SHALL display 3 random upgrades from available pool
2. WHEN player clicks refresh button THEN the system SHALL reroll the 3 displayed upgrades
3. WHEN player refreshes shop THEN the system SHALL deduct refresh cost from player coins
4. WHEN shop is refreshed THEN the refresh cost SHALL increase by 10 coins
5. WHEN shop is opened for first time THEN the refresh cost SHALL be 1 coin
6. WHEN shop displays upgrades THEN each upgrade SHALL show its current level and cost
7. WHEN player cannot afford refresh THEN the refresh button SHALL be disabled

**Technical Details:**
- Extend existing Shop.cpp implementation
- Add refresh button widget to shop window
- Track refresh cost per shop session
- Reset refresh cost when shop is closed and reopened
- Upgrade cost formula separate from refresh cost formula

### Requirement 9: Coin and Currency System

**User Story:** As a player, I want to earn coins by killing enemies, so that I can purchase upgrades and progress.

#### Acceptance Criteria

1. WHEN enemy is killed THEN the system SHALL spawn coin entity at enemy position
2. WHEN coin entity is spawned THEN it SHALL be collectible by player
3. WHEN player collects coin THEN the system SHALL increase player coin count
4. WHEN wave is completed THEN the system SHALL award bonus coins based on performance
5. WHEN coin count changes THEN the system SHALL update coin display widget
6. WHEN player has insufficient coins THEN upgrade purchase SHALL be disabled
7. WHEN player purchases upgrade THEN the system SHALL deduct upgrade cost from coin count

**Technical Details:**
- Existing Coin.cpp entity system already implemented
- Coin drop amount scales with enemy health and type
- Wave completion bonus calculated based on enemies killed and time taken
- Coin widget already displays on player window

### Requirement 10: Audio System (Later Phase)

**User Story:** As a player, I want immersive audio feedback, so that the game feels polished and engaging.

#### Acceptance Criteria

1. WHEN wave starts THEN the system SHALL play wave start sound effect
2. WHEN wave is completed THEN the system SHALL play wave complete sound effect
3. WHEN enemy spawns THEN the system SHALL play enemy-specific spawn sound
4. WHEN enemy dies THEN the system SHALL play enemy-specific death sound
5. WHEN boss spawns THEN the system SHALL play boss music track
6. WHEN player purchases upgrade THEN the system SHALL play purchase sound effect
7. WHEN player health is low THEN the system SHALL play low health warning sound
8. WHEN wave number changes THEN the system SHALL change background music track
9. WHEN audio is implemented THEN each wave SHALL have unique BGM track

**Technical Details:**
- Use existing AudioSystem from Engine
- Source audio assets from open source resources
- Implement audio manager for BGM track switching
- Add SFX triggers to enemy spawn/death events
- Add SFX triggers to shop purchase events
- Low health warning plays when health < 30% of max health

### Requirement 11: Visual Effects System (Later Phase)

**User Story:** As a player, I want visual feedback for game events, so that the game feels responsive and satisfying.

#### Acceptance Criteria

1. WHEN enemy dies THEN the system SHALL play simple particle effect (colored circles fading out)
2. WHEN bullet impacts enemy THEN the system SHALL play impact particle effect
3. WHEN player takes damage THEN the system SHALL play damage particle effect on player
4. WHEN significant event occurs THEN the system SHALL trigger screen shake effect
5. WHEN particle effects are created THEN they SHALL fade out over time and self-destruct
6. WHEN upgrade is purchased THEN visual feedback SHALL appear on player (depends on upgrade type)

**Technical Details:**
- Simple particle system using colored circles/squares
- Particles fade alpha over lifetime
- Screen shake implemented as camera position offset
- Upgrade visual feedback varies by type (larger bullets, trails, glows, etc.)
- Reuse existing rendering system for particle drawing

### Requirement 12: Game Over System

**User Story:** As a player, I want to see my performance when I die, so that I can track my progress and feel motivated to improve.

#### Acceptance Criteria

1. WHEN player health reaches 0 THEN the system SHALL trigger game over state
2. WHEN game over is triggered THEN the system SHALL display "Game Over" screen
3. WHEN game over screen is shown THEN it SHALL display waves survived count
4. WHEN game over screen is shown THEN it SHALL display total enemies killed count
5. WHEN game over screen is shown THEN it SHALL display total coins earned count
6. WHEN game over screen is displayed for 3 seconds THEN the system SHALL return to attract mode
7. WHEN returning to attract mode THEN the system SHALL reset all game state

**Technical Details:**
- Add GAME_OVER state to eGameState enum
- Track statistics during gameplay (waves survived, enemies killed, coins earned)
- Create game over screen rendering in RenderGameOver() method
- Use timer to automatically transition back to attract mode after 3 seconds
- Reset all entities, player stats, and wave progress on state transition

### Requirement 13: Stretch Goal - Menu System

**User Story:** As a player, I want to customize game settings, so that I can adjust the experience to my preferences.

#### Acceptance Criteria

1. WHEN player accesses menu THEN the system SHALL display settings options
2. WHEN player adjusts audio volume THEN the system SHALL update master volume
3. WHEN player changes settings THEN the system SHALL persist settings between sessions
4. WHEN menu is opened THEN the game SHALL pause

**Technical Details:**
- Add MENU state to eGameState enum
- Implement settings UI using existing widget system
- Audio volume control for master volume
- Settings saved to config file

### Requirement 14: Stretch Goal - Endless Mode

**User Story:** As a player, I want an endless survival mode, so that I can test my skills and compete for high scores.

#### Acceptance Criteria

1. WHEN player selects endless mode THEN waves SHALL continue infinitely
2. WHEN endless mode is active THEN difficulty SHALL scale continuously
3. WHEN player dies in endless mode THEN the system SHALL display final wave reached
4. WHEN endless mode is active THEN enemy spawn rate SHALL increase over time

**Technical Details:**
- Add mode selection to attract screen
- Remove wave cap in endless mode
- Implement continuous difficulty scaling formula
- Track high score for endless mode

## Non-Functional Requirements

### Code Architecture and Modularity

- **Single Responsibility Principle**: Each enemy type in separate .cpp/.hpp file pair
- **Modular Design**: Wave system, upgrade system, and audio system as separate modules
- **Dependency Management**: Minimize coupling between enemy types and game systems
- **Clear Interfaces**: Entity base class provides consistent interface for all enemy types
- **Inheritance-Based Design**: Maintain existing Entity inheritance architecture
- **Code Comments**: All comments in same language as existing codebase (appears to be English with some Chinese)

### Performance

- **Frame Rate**: Maintain 60 FPS with 50+ entities on screen
- **Memory Management**: Use existing GAME_SAFE_RELEASE pattern for entity cleanup
- **Window Management**: Efficiently handle multiple windows without performance degradation
- **Collision Detection**: Optimize collision checks for large entity counts

### Reliability

- **Stability**: Game must run for 10+ minutes without crashes (MVP requirement)
- **Error Handling**: Graceful handling of edge cases (e.g., no enemies to spawn, invalid upgrade purchases)
- **State Management**: Proper cleanup when transitioning between game states
- **Memory Leaks**: No memory leaks during extended gameplay sessions

### Usability

- **Controls**: Maintain existing control scheme (WASD movement, mouse shooting, SPACE for shop)
- **Visual Clarity**: Enemy health and player stats clearly visible on windows
- **Feedback**: Clear audio and visual feedback for all player actions
- **Learning Curve**: Progressive difficulty allows players to learn mechanics gradually

### Maintainability

- **Code Readability**: Clean, well-organized code following existing project conventions
- **Documentation**: Code comments explaining complex logic and design decisions
- **Consistency**: Consistent naming conventions and code style throughout
- **Extensibility**: Easy to add new enemy types and upgrades in the future

## Success Criteria

### Minimum Viable Product (MVP) - Required for Passing DFS2

1. ✅ All compilation errors fixed (OpenSSL linking, g_widgetSubsystem duplicate)
2. ✅ 6 enemy types working (Tier 1: Triangle, Circle, Octagon, Square, Pentagon, Hexagon)
3. ✅ 7 combat upgrades working (fire rate, damage, projectile count, spread, size, piercing, homing)
4. ✅ Wave-based spawning system functional
5. ✅ Shop refresh system implemented
6. ✅ Game runs stable for 10+ minutes without crashes
7. ✅ Code refactoring completed for improved readability

### Target Goals - Full Feature Set

1. ✅ 9 enemy types (Tier 1 + Tier 2: add Spawner, Teleporter, Shielded)
2. ✅ Audio system (BGM per wave, SFX per enemy type)
3. ✅ Visual effects (particle effects for deaths, impacts, damage)
4. ✅ Game over screen with statistics
5. ✅ All upgrades have visual feedback

### Stretch Goals - Beyond Semester

1. ⭐ 12 enemy types (add Kamikaze, Mini-Boss, Final Boss)
2. ⭐ Menu system with audio volume controls
3. ⭐ Endless mode with high score tracking

## Project Timeline

### Sprint 1 (Weeks 1-2, 24 hours)
- Fix compilation errors
- Code refactoring and cleanup
- Wave system foundation

### Sprint 2 (Weeks 3-4, 24 hours)
- Implement 3 new Tier 1 enemies (Square, Pentagon, Hexagon)
- Redesign existing enemies (Triangle, Circle, Octagon)

### Sprint 3 (Weeks 5-6, 24 hours)
- Implement 3 Tier 2 enemies (Spawner, Teleporter, Shielded)
- Wave boss system

### Sprint 4 (Weeks 7-8, 24 hours)
- Implement 7 combat upgrades
- Shop refresh system

### Sprint 5 (Weeks 9-10, 24 hours)
- Audio system implementation
- Visual effects system

### Sprint 6 (Weeks 11-12, 24 hours)
- Game over system
- Polish and bug fixes
- Final testing

### Stretch Sprint 7-8 (Beyond semester)
- Menu system
- Endless mode
- Optional enemies (Kamikaze, Mini-Boss, Final Boss)

## Risk Assessment

### High Risk
- **Wave system complexity**: Managing enemy spawning, boss triggers, and difficulty scaling
- **Upgrade stacking**: Ensuring infinite stacking doesn't break game balance or performance
- **Multi-window performance**: Maintaining 60 FPS with many entities across multiple windows

### Medium Risk
- **Enemy behavior variety**: Creating distinct, interesting behaviors for 9+ enemy types
- **Audio asset sourcing**: Finding appropriate open source audio that fits game style
- **Visual feedback implementation**: Making upgrades feel impactful without complex VFX

### Low Risk
- **Compilation fixes**: Clear solution pattern from Protogame2D reference
- **Shop refresh**: Simple extension of existing shop system
- **Code refactoring**: Incremental improvements without architectural changes

## Definition of Done

A requirement is considered "Done" when:

1. ✅ Code is implemented and compiles without errors
2. ✅ Feature works as described in acceptance criteria
3. ✅ Code is manually tested and verified functional
4. ✅ Code follows existing project conventions and style
5. ✅ Code comments are added for complex logic
6. ✅ No new crashes or memory leaks introduced
7. ✅ Feature is integrated with existing systems
8. ✅ Performance remains acceptable (60 FPS target)

## Approval

This requirements document will be reviewed and approved before proceeding to the design phase. Approval will be managed through the spec-workflow approval system.

**Approval Status:** Pending Review

**Next Steps After Approval:**
1. Create design.md with technical architecture
2. Create tasks.md with implementation breakdown
3. Begin Sprint 1 implementation
