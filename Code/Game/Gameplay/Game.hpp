//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <cstdint>

#include "Engine/Core/EventSystem.hpp"
#include "Game/Gameplay/Entity.hpp"

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

    eGameState GetCurrentGameState() const;
    void       ChangeGameState(eGameState newGameState);
    std::vector<Entity*> m_entities;
private:
    void UpdateFromInput();
    void HandleEntityCollision();
    void AdjustForPauseAndTimeDistortion() const;
    void RenderAttractMode() const;
    void RenderGame() const;

    Camera*    m_screenCamera = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
    Clock*     m_gameClock    = nullptr;


};
