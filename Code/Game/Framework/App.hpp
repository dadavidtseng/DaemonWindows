//----------------------------------------------------------------------------------------------------
// App.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "GameCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Platform/Window.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Game;

//----------------------------------------------------------------------------------------------------
class App
{
public:
    App(HINSTANCE const& hInstance);
    ~App() = default;
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnWindowClose(EventArgs& args);
    static void RequestQuit();
    static bool m_isQuitting;

    void                AddWindow(HWND const& hwnd);
    std::vector<Window> windows;
    void                UpdateWindows(std::vector<Window>& windows) const;
    void                RenderWindows(const std::vector<Window>& windows) const;
    void                UpdateWindowsResizeIfNeeded(std::vector<Window>& windows);

private:
    void BeginFrame() const;
    void Update();
    void Render() const;
    void EndFrame() const;
    void UpdateCursorMode();


    HINSTANCE m_hInstance;
    Camera*   m_devConsoleCamera = nullptr;
};
