//----------------------------------------------------------------------------------------------------
// App.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Platform/Window.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Game;

//----------------------------------------------------------------------------------------------------
class App
{
public:
    App()  = default;
    ~App() = default;
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnWindowClose(EventArgs& args);
    static void RequestQuit();
    static bool m_isQuitting;

private:
    void BeginFrame() const;
    void Update();
    void Render() const;
    void EndFrame() const;
    void UpdateCursorMode();

    Camera* m_devConsoleCamera = nullptr;
};
