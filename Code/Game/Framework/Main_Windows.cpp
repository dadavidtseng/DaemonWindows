//----------------------------------------------------------------------------------------------------
// Main_Windows.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include <cstdio>
#include <iostream>

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE const applicationInstanceHandle,
                   HINSTANCE,
                   LPSTR const commandLineString,
                   int)
{
    UNUSED(applicationInstanceHandle)
    UNUSED(commandLineString)

    g_app = new App();
    g_app->Startup();
    g_app->RunMainLoop();
    g_app->Shutdown();

    GAME_SAFE_RELEASE(g_app);

    return 0;
}
