//----------------------------------------------------------------------------------------------------
// GameCommon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <vector>

//-Forward-Declaration--------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
class App;
class AudioSystem;
class BitmapFont;
class Game;
class Renderer;
class Window;
class WidgetSubsystem;
class WindowSubsystem;
class RandomNumberGenerator;

//----------------------------------------------------------------------------------------------------
//-one-time declaration
extern App*                   g_app;
extern AudioSystem*           g_audio;
extern BitmapFont*            g_bitmapFont;
extern Game*                  g_game;
extern Renderer*              g_renderer;
extern RandomNumberGenerator* g_rng;
extern WidgetSubsystem*       g_widgetSubsystem;
extern WindowSubsystem*       g_windowSubsystem;



//-----------------------------------------------------------------------------------------------
// DebugRender-related
//
void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void DebugDrawGlowCircle(Vec2 const& center, float radius, Rgba8 const& color, float glowIntensity);
void DebugDrawGlowBox(Vec2 const& center, Vec2 const& dimensions, Rgba8 const& color, float glowIntensity);
void DebugDrawBoxRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);

//----------------------------------------------------------------------------------------------------
template <typename T>
void GAME_SAFE_RELEASE(T*& pointer)
{
    if (pointer != nullptr)
    {
        delete pointer;
        pointer = nullptr;
    }
}
//-----------------------------------------------------------------------------------------------
typedef uint32_t EntityID;
typedef uint32_t WindowID;