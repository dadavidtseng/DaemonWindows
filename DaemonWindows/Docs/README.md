<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Apache 2.0 License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT TITLE -->
<div align="center">
  <h1>Daemon Windows</h1>
  <p>A multi-window action game that turns the Windows desktop into a shrinking battlefield</p>
</div>

<!-- TODO: Replace with actual gameplay GIF or screenshot -->
<!-- <div align="center">
  <img src="docs/images/demo.gif" alt="Daemon Windows Gameplay" width="720">
</div> -->

<!-- TECH STACK BADGES -->
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![DirectX](https://img.shields.io/badge/DirectX-11-107C10?style=for-the-badge&logo=xbox&logoColor=white)
![Windows API](https://img.shields.io/badge/Windows_API-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![FMOD](https://img.shields.io/badge/FMOD-000000?style=for-the-badge&logo=fmod&logoColor=white)

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How to Install](#how-to-install)
- [How to Use](#how-to-use)
- [Project Structure](#project-structure)
- [Future Roadmap](#future-roadmap)
- [Acknowledgements](#acknowledgements)
- [License](#license)
- [Contact](#contact)

---

## Overview

Daemon Windows is an experimental action game that breaks out of the traditional single-window paradigm. Instead of rendering everything inside one viewport, the game spawns real OS windows — the player lives inside one, enemies can each have their own, and the shop opens as a separate window. The player's window continuously shrinks, creating mounting pressure to eliminate enemies and collect coins before running out of space.

The core loop is a wave-based survival shooter: triangle-shaped enemies chase the player across the desktop, bullets fly between windows, defeated enemies drop coins, and between waves the player spends coins in a shop to upgrade fire rate, damage, projectile count, piercing, homing, and more. Boss waves arrive every 5 rounds with scaled difficulty.

Built as a personal project exploring unconventional game design, the game runs on a custom [Daemon Engine](https://github.com/dadavidtseng/Engine) providing DirectX 11 rendering, FMOD audio, and a widget UI system — extended here with a WindowSubsystem that manages the creation, animation, positioning, and destruction of native Win32 windows as first-class game objects.

## Features

- [Multi-Window Gameplay](#multi-window-gameplay)
- [Wave-Based Survival with Shop Upgrades](#wave-based-survival-with-shop-upgrades)
- [Window Shrinking Mechanic](#window-shrinking-mechanic)
- [Event-Driven Architecture](#event-driven-architecture)

---

### Multi-Window Gameplay

The WindowSubsystem creates and manages real Win32 child windows at runtime. Each entity can optionally own a window — the player always has one, and enemies spawn with or without windows randomly. Windows track their owning entities, follow entity positions on screen, and support smooth animated resizing and repositioning via interpolation.

Key capabilities:
- `CreateChildWindow` / `DestroyWindow` — spawn and tear down OS windows tied to entity IDs
- `AnimateWindowPositionAndDimensions` — smooth lerp-based window transitions
- Entity-to-window mapping with `WindowID` ↔ `EntityID` bidirectional lookup
- Transparent main window setup for desktop-as-battlefield rendering

<!-- ![Multi-Window](docs/images/feature-multi-window.png) -->

### Wave-Based Survival with Shop Upgrades

The WaveManager drives progressive difficulty: each wave spawns `base × 1.2^(wave-1)` enemies, with boss waves every 5 rounds. Between waves, the player opens the Shop (a separate window) to spend collected coins on 7 upgrade types managed by the UpgradeManager:

| Upgrade | Effect |
|---------|--------|
| Fire Rate | Faster bullet cooldown |
| Damage | Higher bullet damage |
| Projectile Count | More bullets per shot |
| Bullet Spread | Wider shot pattern |
| Bullet Size | Larger hit area |
| Piercing | Bullets pass through enemies |
| Homing | Bullets track nearest enemy |

Each upgrade has 5 levels with 1.5× cost scaling per level.

<!-- ![Shop](docs/images/feature-shop.png) -->

### Window Shrinking Mechanic

The player's window continuously shrinks frame-by-frame via `AnimateWindowPositionAndDimensions`, compressing the playable area. The shrink stops when the client dimensions reach 2.5× the player's physics radius, preventing a zero-size window. Enemy windows with the `hasChildWindow` flag also shrink. This creates a ticking clock — the player must clear enemies and progress before their window becomes too small to maneuver.

<!-- ![Shrink](docs/images/feature-shrink.png) -->

### Event-Driven Architecture

All game state transitions, collisions, wave progression, and upgrades flow through a centralized EventSystem with named events and typed arguments:

- `OnGameStateChanged` — triggers BGM switches, entity cleanup, shop visibility
- `OnCollisionEnter` — dispatches per-entity collision responses (bullet→triangle, player→coin, player→triangle)
- `OnWaveStart` / `OnWaveComplete` — wave lifecycle with enemy count and boss flag
- `OnBossSpawn` — boss wave notification
- `OnUpgradePurchased` — upgrade confirmation with type and new level
- `OnEntityDestroyed` — triggers coin drops from killed enemies

This decoupled design allows entities to subscribe/unsubscribe independently without direct references to each other.

<!-- ![Events](docs/images/feature-events.png) -->

---

## How to Install

### Prerequisites

- Visual Studio 2022 (or 2019) with C++ desktop development workload
- Windows 10 SDK (10.0.19041.0+)
- Windows 10/11 (x64) — required for advanced window management
- DirectX 11 compatible GPU
- [Daemon Engine](https://github.com/dadavidtseng/Engine) cloned as a sibling directory

### Installation

```bash
# Clone both repos side by side
git clone https://github.com/dadavidtseng/Engine.git
git clone https://github.com/dadavidtseng/DaemonWindows.git

# Directory layout should be:
# ├── Engine/
# └── DaemonWindows/
```

1. Open `DaemonWindows.sln` in Visual Studio
2. Set configuration to `Debug | x64`
3. Build the solution (the Engine project is referenced automatically)
4. The executable is deployed to `Run/` via post-build event

## How to Use

### Controls

| Mode | Action | Key |
|------|--------|-----|
| Attract | Start game | Space |
| Attract | Quit | ESC |
| Game | Move | W / A / S / D |
| Game | Shoot | Left Mouse Button (hold for auto-fire) |
| Game | Open shop | Space |
| Game | Return to title | ESC |
| Shop | Buy upgrades | 1 / 2 / 3 |
| Shop | Return to game | ESC |

### Debug Controls

| Action | Key |
|--------|-----|
| Pause / Unpause | P |
| Step single frame | O |
| Slow-mo (0.1×) | T (hold) |

### Game Flow

1. **Attract Mode** — Title screen with player window visible; press Space to begin
2. **Game Mode** — Survive waves of triangle enemies, collect coins from kills, window shrinks over time
3. **Shop Mode** — Spend coins on upgrades between waves (speed, health, max health)
4. **Death** — Player health reaches 0, returns to Attract Mode

### Running

Launch `Run/DaemonWindows_Debug_x64.exe` from the `Run/` directory (working directory must be `Run/` for asset loading).

## Project Structure

```
DaemonWindows/
├── Code/Game/
│   ├── Framework/                     # Application framework
│   │   ├── Main_Windows.cpp           # WinMain entry point
│   │   ├── App                        # Application lifecycle
│   │   └── GameCommon                 # Global pointers (g_game, g_windowSubsystem, etc.)
│   ├── Gameplay/                      # Core game logic
│   │   ├── Game                       # State machine (Attract → Game → Shop), spawning, collision
│   │   ├── Entity                     # Base class: position, health, physics/cosmetic radius, window ownership
│   │   ├── Player                     # Disc-shaped player (10 HP), WASD + mouse, auto-fire bullets
│   │   ├── Triangle                   # Enemy: chases player, 1–5 HP, optional child window
│   │   ├── Bullet                     # Projectile fired toward cursor position
│   │   ├── Coin                       # Dropped by killed enemies, collected on contact
│   │   ├── Shop                       # Upgrade purchase UI in its own window
│   │   ├── WaveManager                # Wave progression, difficulty scaling, boss waves
│   │   ├── UpgradeManager             # 7 upgrade types × 5 levels with cost scaling
│   │   ├── EnemyUtils                 # Stateless AI helpers (chase, spawn position)
│   │   ├── Debris                     # Environmental particle
│   │   ├── Circle                     # Enemy type (stub)
│   │   └── Octagon                    # Enemy type (stub)
│   └── Subsystem/
│       ├── Widget/                    # UI widget framework
│       │   └── ButtonWidget           # Text label rendered on child windows
│       └── Window/                    # Multi-window engine
│           └── WindowSubsystem        # Win32 window lifecycle, animation, entity mapping
├── Run/                               # Runtime directory
│   ├── Data/Audio/                    # BGM + SFX (MP3)
│   ├── Data/Images/                   # Backgrounds, title, icons
│   ├── Data/Fonts/                    # Bitmap fonts (DaemonFont)
│   └── Data/Shaders/                  # HLSL shaders (Default)
├── Docs/                              # Documentation
└── DaemonWindows.sln                  # Visual Studio solution
```

| Module | Description |
|--------|-------------|
| Framework | WinMain entry, App lifecycle, global subsystem pointers |
| Gameplay | Game state machine, entity system, wave/upgrade managers, collision |
| Widget Subsystem | ButtonWidget UI rendered on child windows via WidgetSubsystem |
| Window Subsystem | Win32 window creation, animation, entity↔window mapping, focus management |

## Future Roadmap

- [x] Core multi-window system with entity ownership
- [x] Player movement, shooting, and window-bounded physics
- [x] Triangle enemy AI with chase behavior
- [x] Coin drop and collection economy
- [x] Shop system with speed, health, max health upgrades
- [x] WaveManager with progressive difficulty and boss waves
- [x] UpgradeManager with 7 upgrade types and cost scaling
- [ ] Implement Circle and Octagon enemy types with unique behaviors
- [ ] Advanced enemy spawn patterns driven by WaveManager
- [ ] Visual effects and particle systems for kills and upgrades
- [ ] Multi-monitor support and window-to-window teleportation
- [ ] Performance optimization for high entity counts across windows

See the [open issues](https://github.com/dadavidtseng/DaemonWindows/issues) for a full list of proposed features and known issues.

## Acknowledgements

- [Daemon Engine](https://github.com/dadavidtseng/Engine) — Custom C++ engine providing DirectX 11 rendering, FMOD audio, input, and widget systems
- Microsoft Windows API documentation — Foundation for multi-window management
- SMU Guildhall — Academic environment and engine development curriculum

## License

Copyright 2025 Yu-Wei Tseng

Licensed under the [Apache License, Version 2.0](../LICENSE).

## Contact

**Yu-Wei Tseng**
- Portfolio: [dadavidtseng.info](https://dadavidtseng.info)
- GitHub: [@dadavidtseng](https://github.com/dadavidtseng)
- LinkedIn: [dadavidtseng](https://www.linkedin.com/in/dadavidtseng/)
- Email: dadavidtseng@gmail.com

Project Link: [github.com/dadavidtseng/DaemonWindows](https://github.com/dadavidtseng/DaemonWindows)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- REFERENCE-STYLE LINKS -->
[contributors-shield]: https://img.shields.io/github/contributors/dadavidtseng/DaemonWindows.svg?style=for-the-badge
[contributors-url]: https://github.com/dadavidtseng/DaemonWindows/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/dadavidtseng/DaemonWindows.svg?style=for-the-badge
[forks-url]: https://github.com/dadavidtseng/DaemonWindows/network/members
[stars-shield]: https://img.shields.io/github/stars/dadavidtseng/DaemonWindows.svg?style=for-the-badge
[stars-url]: https://github.com/dadavidtseng/DaemonWindows/stargazers
[issues-shield]: https://img.shields.io/github/issues/dadavidtseng/DaemonWindows.svg?style=for-the-badge
[issues-url]: https://github.com/dadavidtseng/DaemonWindows/issues
[license-shield]: https://img.shields.io/github/license/dadavidtseng/DaemonWindows.svg?style=for-the-badge
[license-url]: https://github.com/dadavidtseng/DaemonWindows/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/dadavidtseng
