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
App*                   g_app             = nullptr;       // Created and owned by Main_Windows.cpp
AudioSystem*           g_audio           = nullptr;       // Created and owned by the App
BitmapFont*            g_bitmapFont      = nullptr;       // Created and owned by the App
Game*                  g_game            = nullptr;       // Created and owned by the App
Renderer*              g_renderer        = nullptr;       // Created and owned by the App
RandomNumberGenerator* g_rng             = nullptr;       // Created and owned by the App
Window*                g_window          = nullptr;       // Created and owned by the App
WidgetSubsystem*       g_widgetSubsystem = nullptr;       // Created and owned by the App
WindowSubsystem*       g_windowSubsystem = nullptr;       // Created and owned by the App

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

//----------------------------------------------------------------------------------------------------
/// @brief
/// Create all engine subsystems in a specific order.
void App::Startup()
{
    //-Start-of-EventSystem---------------------------------------------------------------------------

    sEventSystemConfig constexpr sEventSystemConfig;
    g_eventSystem = new EventSystem(sEventSystemConfig);
    g_eventSystem->SubscribeEventCallbackFunction("OnCloseButtonClicked", OnWindowClose);
    g_eventSystem->SubscribeEventCallbackFunction("quit", OnWindowClose);

    //-End-of-EventSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-InputSystem---------------------------------------------------------------------------

    sInputSystemConfig constexpr sInputSystemConfig;
    g_input = new InputSystem(sInputSystemConfig);

    //-End-of-InputSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Window--------------------------------------------------------------------------------

    sWindowConfig sWindowConfig;
    sWindowConfig.m_windowType             = eWindowType::FULLSCREEN_CROP;
    sWindowConfig.m_aspectRatio            = 2.f;
    sWindowConfig.m_inputSystem            = g_input;
    sWindowConfig.m_windowTitle            = "WindowKills";
    sWindowConfig.m_iconFilePath           = L"C:/p4/Personal/SD/WindowKills/Run/Data/Images/windowIcon.ico";
    sWindowConfig.m_supportMultipleWindows = true;
    g_window                           = new Window(sWindowConfig);

    //-End-of-Window----------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Renderer------------------------------------------------------------------------------

    sRendererConfig sRendererConfig;
    sRendererConfig.m_window = g_window;
    g_renderer           = new Renderer(sRendererConfig);

    //-End-of-Renderer--------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-DebugRender---------------------------------------------------------------------------

    sDebugRenderConfig sDebugRenderConfig;
    sDebugRenderConfig.m_renderer = g_renderer;
    sDebugRenderConfig.m_fontName = "SquirrelFixedFont";

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

    sAudioSystemConfig constexpr sAudioSystemConfig;
    g_audio = new AudioSystem(sAudioSystemConfig);

    //-End-of-AudioSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-WindowSubsystem-----------------------------------------------------------------------

    sWindowSubsystemConfig sWindowSubsystemConfig;
    sWindowSubsystemConfig.m_iconFilePath = L"C:/p4/Personal/SD/WindowKills/Run/Data/Images/windowIcon.ico";
    g_windowSubsystem                 = new WindowSubsystem(sWindowSubsystemConfig);

    //-End-of-WindowSubsystem-------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-WindowSubsystem-----------------------------------------------------------------------

    sWidgetSubsystemConfig constexpr sWidgetSubsystemConfig;
    g_widgetSubsystem = new WidgetSubsystem(sWidgetSubsystemConfig);

    //-End-of-WindowSubsystem-------------------------------------------------------------------------

    g_eventSystem->Startup();
    g_window->Startup();
    g_renderer->Startup();
    DebugRenderSystemStartup(sDebugRenderConfig);
    // g_theDevConsole->StartUp();
    g_input->Startup();
    g_audio->Startup();
    g_windowSubsystem->StartUp();
    g_widgetSubsystem->StartUp();

    g_bitmapFont = g_renderer->CreateOrGetBitmapFontFromFile("Data/Fonts/SquirrelFixedFont"); // DO NOT SPECIFY FILE .EXTENSION!!  (Important later on.)
    g_rng        = new RandomNumberGenerator();
    g_game       = new Game();
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    // Destroy all Engine Subsystem
    GAME_SAFE_RELEASE(g_game);
    GAME_SAFE_RELEASE(g_rng);
    GAME_SAFE_RELEASE(g_bitmapFont);

    g_widgetSubsystem->ShutDown();
    g_windowSubsystem->ShutDown();
    g_audio->Shutdown();
    g_input->Shutdown();
    // g_theDevConsole->Shutdown();

    GAME_SAFE_RELEASE(m_devConsoleCamera);

    DebugRenderSystemShutdown();
    g_renderer->Shutdown();
    g_window->Shutdown();
    g_eventSystem->Shutdown();

    GAME_SAFE_RELEASE(g_audio);
    GAME_SAFE_RELEASE(g_renderer);
    GAME_SAFE_RELEASE(g_window);
    GAME_SAFE_RELEASE(g_input);
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
    g_eventSystem->BeginFrame();
    g_window->BeginFrame();
    g_renderer->BeginFrame();
    DebugRenderBeginFrame();
    // g_theDevConsole->BeginFrame();
    g_input->BeginFrame();
    g_audio->BeginFrame();
    g_windowSubsystem->BeginFrame();
    g_widgetSubsystem->BeginFrame();
}

//----------------------------------------------------------------------------------------------------
void App::Update()
{
    Clock::TickSystemClock();

    UpdateCursorMode();

    g_windowSubsystem->Update();
    g_widgetSubsystem->Update();
    g_game->Update();
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
    g_game->Render();
    g_widgetSubsystem->Render();


    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));

    // g_theDevConsole->Render(box);
    g_windowSubsystem->Render();
}

//----------------------------------------------------------------------------------------------------
void App::EndFrame() const
{
    g_eventSystem->EndFrame();
    g_window->EndFrame();
    g_renderer->EndFrame();
    DebugRenderEndFrame();
    // g_theDevConsole->EndFrame();
    g_input->EndFrame();
    g_audio->EndFrame();
    g_windowSubsystem->EndFrame();
    g_widgetSubsystem->EndFrame();
}

//----------------------------------------------------------------------------------------------------
void App::UpdateCursorMode()
{
    bool const doesWindowHasFocus = GetActiveWindow() == g_window->GetWindowHandle();
    bool const isAttractState     = g_game->GetCurrentGameState() == eGameState::ATTRACT;
    // bool const shouldUsePointerMode = !doesWindowHasFocus || g_theDevConsole->IsOpen() || isAttractState;
    bool const shouldUsePointerMode = !doesWindowHasFocus || isAttractState;

    if (shouldUsePointerMode == true)
    {
        g_input->SetCursorMode(eCursorMode::POINTER);
    }
    else
    {
        g_input->SetCursorMode(eCursorMode::FPS);
    }
}
