# DaemonWindows - Window-Based Action Game

![WindowKills Screenshot](https://img.shields.io/badge/C++-grey?style=for-the-badge&logo=cplusplus)

## 🎮 Game Overview
DaemonWindows is an innovative action game that transforms the traditional Windows desktop environment into a dynamic gaming battlefield. Players control a character that can move between multiple windows, creating a unique multi-window gaming experience that blurs the line between game interface and desktop applications. This experimental project explores unconventional game design by utilizing the Windows API to create an immersive gameplay experience that extends beyond the typical single-window constraint.

## 🎯 Core Gameplay
* **Multi-Window Navigation**: Players can seamlessly move between different game windows, each serving as interconnected game environments
* **Dynamic Entity System**: Entities spawn and interact across multiple windows, creating complex spatial relationships
* **Resource Management**: Collect coins and manage health while navigating the window-based battlefield
* **Progressive Shop System**: Upgrade player abilities using collected currency through an integrated shop interface

## 🌟 Key Features
* **Windows API Integration**: Advanced utilization of Windows system calls for multi-window management and focus control
* **Cross-Window Collision System**: Sophisticated physics and collision detection that works across multiple application windows
* **Real-time Window Manipulation**: Dynamic window resizing, positioning, and focus management as core gameplay mechanics
* **Entity Lifecycle Management**: Comprehensive system for spawning, updating, and destroying game entities across windows
* **Audio-Visual Feedback**: Integrated sound system with state-based audio management
* **Upgrade Progression**: RPG-like character improvement system with speed, health, and maximum health upgrades

## 🛠️ Technical Stack
* **Game Engine:** Custom Daemon Engine (C++)
* **Programming Languages:** C++, Windows API
* **Graphics Pipeline:** DirectX 11 with 2D sprite rendering
* **Audio Engine:** Custom audio system with positional sound
* **Networking:** Not applicable (single-player focused)
* **Platform:** Windows (x64) - leverages Windows-specific APIs for multi-window functionality

## 📁 Project Architecture
```
├── Code/
│   └── Game/
│       ├── Gameplay/
│       │   ├── Game.cpp/hpp        (Main game controller)
│       │   ├── Player.cpp/hpp      (Player entity with window management)
│       │   ├── Entity.cpp/hpp      (Base entity class)
│       │   ├── Bullet.cpp/hpp      (Projectile system)
│       │   ├── Circle.cpp/hpp      (Enemy entity type)
│       │   ├── Triangle.cpp/hpp    (Enemy entity type)
│       │   ├── Octagon.cpp/hpp     (Enemy entity type)
│       │   ├── Coin.cpp/hpp        (Collectible currency)
│       │   ├── Debris.cpp/hpp      (Environmental objects)
│       │   └── Shop.cpp/hpp        (Upgrade system)
│       ├── Framework/
│       └── Subsystem/
├── Docs/
│   └── README.md
├── Run/
├── Temporary/
└── DaemonWindows.sln
```

## 🚀 Getting Started

### Prerequisites
* **Visual Studio 2022** or later with C++ development tools
* **Windows 10 SDK** (10.0.19041.0 or later)
* **DirectX 11** compatible graphics card
* **Windows 10/11** (required for advanced window management features)

### Installation
1. Clone the repository
   ```bash
   git clone https://github.com/dadavidtseng/DaemonWindows.git
   cd DaemonWindows
   ```

2. Open the Visual Studio solution
   ```bash
   start DaemonWindows.sln
   ```

3. Build the project
   - Set platform to `x64`
   - Choose `Debug` or `Release` configuration
   - Press `Ctrl+Shift+B` to build

4. Run the game
   - Press `F5` to start with debugging or `Ctrl+F5` to run without debugging

## 🎮 How to Play

### Controls

#### Attract Mode
* **SPACE:** Enter the game
* **ESC:** Exit the application

#### In-Game Mode
* **WASD:** Move the player character
* **Left Mouse Button:** Shoot projectiles
* **SPACE:** Open the upgrade shop
* **ESC:** Return to attract mode

#### Shop Mode
* **1:** Increase player movement speed
* **2:** Restore player health
* **3:** Increase maximum health capacity
* **ESC:** Return to the game

### Game Modes
* **Attract Mode:** Main menu with game information and controls
* **Survival Mode:** Navigate multiple windows while avoiding enemies and collecting coins
* **Shop Mode:** Spend collected coins on character upgrades

## 📈 Development Progress

### Current Status: Alpha

### Milestones
* [x] **Phase 1:** Core multi-window system implementation
* [x] **Phase 2:** Entity management and collision detection across windows
* [x] **Phase 3:** Player controls and basic game mechanics
* [x] **Phase 4:** Shop system and progression mechanics
* [ ] **Phase 5:** Advanced enemy AI and spawn patterns
* [ ] **Phase 6:** Visual effects and particle systems
* [ ] **Phase 7:** Performance optimization and polish

### Known Issues
* **Window Focus Management:** Occasional focus conflicts when switching between game windows rapidly
* **Performance Scaling:** Frame rate may decrease with high entity counts across multiple windows
* **Window Positioning:** Some edge cases with window boundary collision detection

## 🎨 Media

### Gameplay Features
* Multi-window entity movement and interaction
* Dynamic collision detection across window boundaries
* Real-time shop interface with upgrade purchasing
* Smooth player movement with momentum-based physics

### Technical Highlights
* Advanced Windows API integration for window management
* Custom entity component system with cross-window capabilities
* Efficient collision detection optimized for multi-window scenarios

## 📊 Research Focus
This project explores several innovative areas in game development and human-computer interaction:

### Research Objectives
* **Multi-Window Game Design:** Investigating how traditional game mechanics can be adapted for multi-window environments
* **Desktop Integration Gaming:** Exploring the boundaries between game applications and desktop environment interaction
* **Window Management as Gameplay:** Using operating system window management as a core gameplay mechanic

### Methodology
The development approach emphasizes experimental game design with focus on:
- **User Experience Research:** Understanding player behavior when interacting with multi-window game interfaces
- **Technical Innovation:** Pushing the boundaries of traditional game window constraints
- **Performance Analysis:** Optimizing game systems for multi-window rendering and event handling

### Findings
* **Player Adaptation:** Users quickly adapt to multi-window navigation, finding it intuitive after initial learning curve
* **Technical Feasibility:** Windows API provides sufficient functionality for advanced window manipulation in games
* **Design Opportunities:** Multi-window gaming opens new possibilities for puzzle and strategy game mechanics

## 🤝 Contributing
Contributions are welcome, especially in areas related to Windows API optimization and multi-window game design patterns.

### Development Workflow
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/window-enhancement`)
3. Make your changes with clear commit messages
4. Test on multiple Windows versions if possible
5. Submit a pull request with detailed description

### Areas for Contribution
* **Performance Optimization:** Multi-window rendering efficiency improvements
* **Cross-Platform Exploration:** Investigating similar functionality on macOS/Linux
* **Advanced Window Effects:** Visual transitions and effects for window interactions
* **Accessibility Features:** Making multi-window gaming more accessible

## 📄 Documentation
* [Technical Design Document](Docs/technical-design.md) *(planned)*
* [Windows API Integration Guide](Docs/windows-api-guide.md) *(planned)*
* [Multi-Window Game Design Patterns](Docs/design-patterns.md) *(planned)*
* [Performance Optimization Guide](Docs/performance.md) *(planned)*

## 📝 License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments
* **Custom Daemon Engine:** Built as part of personal engine development journey
* **Windows API Documentation:** Microsoft's comprehensive system programming resources
* **Game Development Community:** Inspiration from experimental and indie game developers
* **Desktop Application Design:** Insights from modern desktop application UX patterns

## 📞 Contact
For questions about this project, please contact:
* **Developer:** Yu-Wei Tseng - [dadavidtseng@gmail.com](mailto:dadavidtseng@gmail.com)
* **GitHub:** [https://github.com/dadavidtseng](https://github.com/dadavidtseng)
* **Portfolio:** [https://dadavidtseng.info](https://dadavidtseng.info)

---
**Development Period:** 2023 - Present (Active Development)  
**Last Updated:** August 7, 2025