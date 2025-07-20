//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <cstdint>


#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"
#include "Game/Gameplay/Entity.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;

//----------------------------------------------------------------------------------------------------
enum class eGameState : int8_t
{
    ATTRACT,
    GAME,
    SHOP,
};

//----------------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();

    void Update();
    void Render() const;

    eGameState           GetCurrentGameState() const;
    void                 ChangeGameState(eGameState newGameState);
    Clock*               GetGameClock() const;
    Player*              GetPlayer() const;
    std::vector<Entity*> m_entities;

private:
    static bool OnGameStateChanged(EventArgs& args);
    void UpdateFromInput();
    void HandleEntityCollision();
    void AdjustForPauseAndTimeDistortion() const;
    void RenderAttractMode() const;
    void RenderGame() const;

    void SpawnEntity();
    void DestroyEntity();
    void SpawnShop();
    void DestroyShop();

    Camera*    m_screenCamera = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
    Clock*     m_gameClock    = nullptr;
    Timer*     m_gameTimer    = nullptr;
    Vec2 m_titlePosition = Vec2::ZERO;
};
