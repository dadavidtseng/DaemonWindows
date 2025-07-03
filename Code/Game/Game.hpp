//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <cstdint>

#include "Engine/Core/EventSystem.hpp"
#include "Engine/Math/Vec2.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Clock;

//----------------------------------------------------------------------------------------------------
enum class eGameState : int8_t
{
    ATTRACT,
    GAME
};

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();

    void Update();
    void Render() const;

    static bool OnGameStateChanged(EventArgs& args);
    static bool OnWindowSizeChanged(EventArgs& args);

    eGameState GetCurrentGameState() const;
    void       ChangeGameState(eGameState newGameState);
    Vec2 m_position = Vec2::ZERO;
    Vec2 m_windowPosition = Vec2::ZERO;
private:
    void UpdateFromInput();
    void AdjustForPauseAndTimeDistortion();
    void RenderAttractMode() const;
    void RenderGame() const;

    Camera*    m_screenCamera = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
    Clock*     m_gameClock    = nullptr;

};
