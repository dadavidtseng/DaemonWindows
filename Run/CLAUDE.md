[Root Directory](../CLAUDE.md) > **Run**

# Runtime Assets

## Module Responsibilities

The Run directory contains all runtime assets and executables required for the DaemonWindows application:

- **Executable Distribution**: Debug and Release build outputs
- **Asset Management**: Game assets organized by type (Audio, Images, Shaders, Fonts)
- **Runtime Dependencies**: Third-party libraries and DLLs
- **Development Resources**: Test assets and development icons

## Entry and Startup

### Executable Files
- **DaemonWindows_Debug_x64.exe**: Debug build with full debugging information
- **DaemonWindows_Release_x64.exe**: Optimized release build for distribution
- **Automatic Deployment**: Executables automatically copied here via PostBuildEvent in Visual Studio

### Asset Loading
Assets are loaded relative to the executable location using the following structure:
- `Data/Audio/`: Audio files for music and sound effects
- `Data/Images/`: Textures, sprites, and UI graphics
- `Data/Shaders/`: HLSL shader files for rendering
- `Data/Fonts/`: Bitmap font resources

## External Interfaces

### Audio Assets
```
Data/Audio/
├── attract.mp3          # Menu/title screen background music
├── ingame.mp3           # Gameplay background music  
├── coin.mp3             # Coin collection sound effect
├── shoot.mp3            # Bullet firing sound effect
├── hit.mp3              # Collision/damage sound effect
└── TestSound.mp3        # Development testing audio
```

### Visual Assets
```
Data/Images/
├── windowIcon.ico       # Application window icon
├── title.png            # Title screen graphics
├── goop.png             # Entity texture (likely enemy)
├── serenity.png         # Background or UI texture
├── Union.png            # UI component texture
├── ripple.png           # Visual effect texture
└── Test_StbiFlippedAndOpenGL.png/ico  # Development test images
```

### Shader Pipeline
```
Data/Shaders/
└── Default.hlsl         # Primary shader for 2D rendering with DirectX 11
```

### Font Resources
```
Data/Fonts/
└── SquirrelFixedFont.png  # Bitmap font for in-game text rendering
```

## Key Dependencies and Configuration

### Runtime Library Dependencies
- **fmod.dll / fmod64.dll**: FMOD audio engine libraries for sound playback
- **Platform Dependencies**: Windows 10/11 runtime components
- **DirectX 11**: Hardware-accelerated graphics rendering

### Asset Loading Pipeline
- **Relative Path Loading**: Assets loaded relative to executable location
- **Engine Integration**: Asset management handled by external Daemon Engine
- **Format Support**: 
  - Audio: MP3 format via FMOD
  - Images: PNG/ICO format via STBI library
  - Shaders: HLSL for DirectX 11
  - Fonts: Custom bitmap font format

## Data Models

### Audio Asset Categories
- **Background Music**: Looping tracks for different game states (attract/ingame)
- **Sound Effects**: Short audio clips triggered by game events (coin, shoot, hit)
- **Test Assets**: Development and debugging audio files

### Image Asset Categories
- **UI Graphics**: Icons, title screens, and interface elements
- **Entity Textures**: Sprites for game objects and characters
- **Effects**: Particle and visual effect textures
- **Development**: Testing and debugging images

### Shader Asset Structure
- **Default Pipeline**: Single HLSL shader handling primary 2D rendering
- **DirectX 11 Integration**: Vertex and pixel shaders for hardware acceleration
- **Multi-Window Support**: Shader designed to work across multiple rendering contexts

## Testing and Quality

### Current Testing Status
- **Asset Integrity**: Manual verification of asset loading and display
- **Performance Testing**: Asset streaming performance with multiple windows
- **Format Compatibility**: Testing across different Windows versions and hardware

### Quality Assurance Features
- **Asset Organization**: Clear directory structure for maintainable asset management
- **Development Assets**: Dedicated test assets for debugging and validation
- **Automated Deployment**: PostBuildEvent ensures executable distribution consistency

### Asset Pipeline Considerations
- **Loading Performance**: Efficient asset streaming for multi-window rendering
- **Memory Management**: Proper asset cleanup when switching between windows
- **Platform Compatibility**: Asset formats chosen for broad Windows compatibility

## FAQ

### Q: Why are there both 32-bit and 64-bit FMOD libraries?
**A**: The project supports both x86 and x64 architectures, though the current build configuration primarily targets x64. Both libraries are included for flexibility and potential future 32-bit builds.

### Q: How do assets work with the multi-window system?
**A**: Assets are shared across all windows since they're loaded into memory once. The rendering system can use the same textures and audio across multiple window contexts without duplication.

### Q: What is the SquirrelFixedFont.png file?
**A**: This is a bitmap font texture containing character glyphs laid out in a grid. The engine renders text by mapping characters to texture coordinates within this image, providing consistent text rendering across different systems.

### Q: How are shaders integrated with the multi-window rendering?
**A**: The Default.hlsl shader is compiled and used across all rendering contexts. Each window maintains its own DirectX rendering context but shares the same compiled shader resources for efficiency.

## Related File List

### Executables
- `DaemonWindows_Debug_x64.exe` - Debug build executable
- `DaemonWindows_Release_x64.exe` - Release build executable

### Runtime Libraries
- `fmod.dll` - FMOD audio library (32-bit)
- `fmod64.dll` - FMOD audio library (64-bit)

### Audio Assets
- `Data/Audio/attract.mp3` - Menu background music
- `Data/Audio/ingame.mp3` - Gameplay background music
- `Data/Audio/coin.mp3` - Coin collection sound
- `Data/Audio/shoot.mp3` - Shooting sound effect
- `Data/Audio/hit.mp3` - Impact/damage sound
- `Data/Audio/TestSound.mp3` - Development test audio

### Visual Assets
- `Data/Images/windowIcon.ico` - Application icon
- `Data/Images/title.png` - Title screen graphic
- `Data/Images/goop.png` - Entity texture
- `Data/Images/serenity.png` - Background texture
- `Data/Images/Union.png` - UI component
- `Data/Images/ripple.png` - Effect texture
- `Data/Images/Test_StbiFlippedAndOpenGL.png` - Test image

### Shaders & Fonts
- `Data/Shaders/Default.hlsl` - Primary rendering shader
- `Data/Fonts/SquirrelFixedFont.png` - Bitmap font texture

## Changelog
- **2025-09-10**: Initial module documentation created, analyzed runtime asset organization, dependency structure, and multi-window asset integration