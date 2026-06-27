//----------------------------------------------------------------------------------------------------
// GameCommon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <cstdint>


//-Forward-Declaration--------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
class App;
class Game;
class WidgetSubsystem;
class WindowSubsystem;


//----------------------------------------------------------------------------------------------------
//-one-time declaration
extern App*                   g_app;
extern Game*                  g_game;
extern WidgetSubsystem*       g_widgetSubsystem;
extern WindowSubsystem*       g_windowSubsystem;

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