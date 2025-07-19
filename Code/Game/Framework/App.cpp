//----------------------------------------------------------------------------------------------------
// App.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Framework/App.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
App*                   g_theApp             = nullptr;       // Created and owned by Main_Windows.cpp
AudioSystem*           g_theAudio           = nullptr;       // Created and owned by the App
BitmapFont*            g_theBitmapFont      = nullptr;       // Created and owned by the App
Game*                  g_theGame            = nullptr;       // Created and owned by the App
Renderer*              g_theRenderer        = nullptr;       // Created and owned by the App
RandomNumberGenerator* g_theRNG             = nullptr;       // Created and owned by the App
Window*                g_theWindow          = nullptr;       // Created and owned by the App
WidgetSubsystem*       g_theWidgetSubsystem = nullptr;       // Created and owned by the App
WindowSubsystem*       g_theWindowSubsystem = nullptr;       // Created and owned by the App

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

//----------------------------------------------------------------------------------------------------
/// @brief
/// Create all engine subsystems in a specific order.
void App::Startup()
{
    //-Start-of-EventSystem---------------------------------------------------------------------------

    sEventSystemConfig constexpr eventSystemConfig;
    g_theEventSystem = new EventSystem(eventSystemConfig);
    g_theEventSystem->SubscribeEventCallbackFunction("OnCloseButtonClicked", OnWindowClose);
    g_theEventSystem->SubscribeEventCallbackFunction("quit", OnWindowClose);

    //-End-of-EventSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-InputSystem---------------------------------------------------------------------------

    sInputSystemConfig constexpr inputConfig;
    g_theInput = new InputSystem(inputConfig);

    //-End-of-InputSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Window--------------------------------------------------------------------------------

    sWindowConfig windowConfig;
    windowConfig.m_windowType             = eWindowType::FULLSCREEN_CROP;
    windowConfig.m_aspectRatio            = 2.f;
    windowConfig.m_inputSystem            = g_theInput;
    windowConfig.m_windowTitle            = "WindowKills";
    windowConfig.m_iconFilePath           = L"C:/p4/Personal/SD/WindowKills/Run/Data/Images/windowIcon.ico";
    // windowConfig.m_supportMultipleWindows = true;
    g_theWindow                           = new Window(windowConfig);

    //-End-of-Window----------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Renderer------------------------------------------------------------------------------

    sRenderConfig renderConfig;
    renderConfig.m_window = g_theWindow;
    g_theRenderer         = new Renderer(renderConfig);

    //-End-of-Renderer--------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-DebugRender---------------------------------------------------------------------------

    sDebugRenderConfig debugConfig;
    debugConfig.m_renderer = g_theRenderer;
    debugConfig.m_fontName = "SquirrelFixedFont";

    //-End-of-DebugRender-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-DevConsole----------------------------------------------------------------------------

    // m_devConsoleCamera = new Camera();
    //
    // Vec2 const bottomLeft     = Vec2::ZERO;
    // Vec2 const screenTopRight = Window::s_mainWindow->GetScreenDimensions();
    //
    // m_devConsoleCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    //
    // sDevConsoleConfig devConsoleConfig;
    // devConsoleConfig.m_defaultRenderer = g_theRenderer;
    // devConsoleConfig.m_defaultFontName = "SquirrelFixedFont";
    // devConsoleConfig.m_defaultCamera   = m_devConsoleCamera;
    // g_theDevConsole                    = new DevConsole(devConsoleConfig);
    //
    // g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Controls");
    // g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "(LMB)   Shoot Bullet");
    // g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "(W/S)   Up/Down");
    // g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "(A/D)   Left/Right");

    //-End-of-DevConsole------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-AudioSystem---------------------------------------------------------------------------

    sAudioSystemConfig constexpr audioConfig;
    g_theAudio = new AudioSystem(audioConfig);

    //-End-of-AudioSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-WindowSubsystem-----------------------------------------------------------------------

    sWindowSubsystemConfig  windowSubsystemConfig;
    windowSubsystemConfig.m_iconFilePath           = L"C:/p4/Personal/SD/WindowKills/Run/Data/Images/windowIcon.ico";
    g_theWindowSubsystem = new WindowSubsystem(windowSubsystemConfig);

    //-End-of-WindowSubsystem-------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-WindowSubsystem-----------------------------------------------------------------------

    sWidgetSubsystemConfig constexpr widgetConfig;
    g_theWidgetSubsystem = new WidgetSubsystem(widgetConfig);

    //-End-of-WindowSubsystem-------------------------------------------------------------------------


    g_theEventSystem->Startup();
    g_theWindow->Startup();
    g_theRenderer->Startup();
    DebugRenderSystemStartup(debugConfig);
    // g_theDevConsole->StartUp();
    g_theInput->Startup();
    g_theAudio->Startup();
    g_theWindowSubsystem->StartUp();
    g_theWidgetSubsystem->StartUp();

    g_theBitmapFont = g_theRenderer->CreateOrGetBitmapFontFromFile("Data/Fonts/SquirrelFixedFont"); // DO NOT SPECIFY FILE .EXTENSION!!  (Important later on.)
    g_theRNG        = new RandomNumberGenerator();
    g_theGame       = new Game();
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    // Destroy all Engine Subsystem
    GAME_SAFE_RELEASE(g_theGame);
    GAME_SAFE_RELEASE(g_theRNG);
    GAME_SAFE_RELEASE(g_theBitmapFont);

    g_theWidgetSubsystem->ShutDown();
    g_theWindowSubsystem->ShutDown();
    g_theAudio->Shutdown();
    g_theInput->Shutdown();
    // g_theDevConsole->Shutdown();

    GAME_SAFE_RELEASE(m_devConsoleCamera);

    DebugRenderSystemShutdown();
    g_theRenderer->Shutdown();
    g_theWindow->Shutdown();
    g_theEventSystem->Shutdown();

    GAME_SAFE_RELEASE(g_theAudio);
    GAME_SAFE_RELEASE(g_theRenderer);
    GAME_SAFE_RELEASE(g_theWindow);
    GAME_SAFE_RELEASE(g_theInput);
}

//----------------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void App::RunFrame()
{
    BeginFrame();   // Engine pre-frame stuff
    Update();       // Game updates / moves / spawns / hurts / kills stuff
    Render();       // Game draws current state of things
    EndFrame();     // Engine post-frame stuff
}

//----------------------------------------------------------------------------------------------------
void App::RunMainLoop()
{
    // Program main loop; keep running frames until it's time to quit
    while (!m_isQuitting)
    {
        // Sleep(16); // Temporary code to "slow down" our app to ~60Hz until we have proper frame timing in
        RunFrame();
    }
}

//----------------------------------------------------------------------------------------------------
STATIC bool App::OnWindowClose(EventArgs& args)
{
    UNUSED(args)

    RequestQuit();

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC void App::RequestQuit()
{
    m_isQuitting = true;
}

//----------------------------------------------------------------------------------------------------
void App::BeginFrame() const
{
    g_theEventSystem->BeginFrame();
    g_theWindow->BeginFrame();
    g_theRenderer->BeginFrame();
    DebugRenderBeginFrame();
    // g_theDevConsole->BeginFrame();
    g_theInput->BeginFrame();
    g_theAudio->BeginFrame();
    g_theWindowSubsystem->BeginFrame();
    g_theWidgetSubsystem->BeginFrame();
}

//----------------------------------------------------------------------------------------------------
void App::Update()
{
    Clock::TickSystemClock();

    UpdateCursorMode();

    g_theWindowSubsystem->Update();
    g_theWidgetSubsystem->Update();
    g_theGame->Update();
}

//----------------------------------------------------------------------------------------------------
// Some simple OpenGL example drawing code.
// This is the graphical equivalent of printing "Hello, world."
//
// Ultimately this function (App::Render) will only call methods on Renderer (like Renderer::DrawVertexArray)
//	to draw things, never calling OpenGL (nor DirectX) functions directly.
//
void App::Render() const
{
    // g_theRenderer->ClearScreen(Rgba8::BLUE);
    g_theGame->Render();
    g_theWidgetSubsystem->Render();


    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));

    // g_theDevConsole->Render(box);
    g_theWindowSubsystem->Render();
}

//----------------------------------------------------------------------------------------------------
void App::EndFrame() const
{
    g_theEventSystem->EndFrame();
    g_theWindow->EndFrame();
    g_theRenderer->EndFrame();
    DebugRenderEndFrame();
    // g_theDevConsole->EndFrame();
    g_theInput->EndFrame();
    g_theAudio->EndFrame();
    g_theWindowSubsystem->EndFrame();
    g_theWidgetSubsystem->EndFrame();
}

//----------------------------------------------------------------------------------------------------
void App::UpdateCursorMode()
{
    bool const doesWindowHasFocus   = GetActiveWindow() == g_theWindow->GetWindowHandle();
    bool const isAttractState       = g_theGame->GetCurrentGameState() == eGameState::ATTRACT;
    // bool const shouldUsePointerMode = !doesWindowHasFocus || g_theDevConsole->IsOpen() || isAttractState;
    bool const shouldUsePointerMode = !doesWindowHasFocus ||  isAttractState;

    if (shouldUsePointerMode == true)
    {
        g_theInput->SetCursorMode(eCursorMode::POINTER);
    }
    else
    {
        g_theInput->SetCursorMode(eCursorMode::FPS);
    }
}
