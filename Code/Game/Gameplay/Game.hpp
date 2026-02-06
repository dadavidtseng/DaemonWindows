//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Entity.hpp"
#include "Game/Gameplay/Shop.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Timer.hpp"
//----------------------------------------------------------------------------------------------------

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Clock;
class Player;
class Bullet;
class Triangle;
class Coin;

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
    //------------------------------------------------------------------------------------------------
    // Construct / Destruct
    //------------------------------------------------------------------------------------------------
    Game();
    ~Game();

    //------------------------------------------------------------------------------------------------
    // Life cycle
    //------------------------------------------------------------------------------------------------
    void Update();
    void Render() const;

    eGameState           GetCurrentGameState() const;
    void                 ChangeGameState(eGameState newGameState);
    Clock*               GetGameClock() const;
    Player*              GetPlayer() const;
    Shop*                GetShop() const;
    Entity*              GetEntityByEntityID(EntityID const& entityID) const;
    std::vector<Entity*> m_entityList;

private:
    //------------------------------------------------------------------------------------------------
    // Game state
    //------------------------------------------------------------------------------------------------
    static bool OnGameStateChanged(EventArgs& args);
    static bool OnEntityDestroyed(EventArgs& args);

    //------------------------------------------------------------------------------------------------
    // Update
    //------------------------------------------------------------------------------------------------
    void        UpdateFromInput();
    void        HandleEntityCollision();
    void        FireCollisionEvent(Entity* entityA, Entity* entityB);
    void        HandleBulletTriangleCollision(Bullet* bullet, Triangle* triangle);
    void        HandlePlayerCoinCollision(Player* player, Coin* coin);
    void        HandlePlayerTriangleCollision(Player* player, Triangle* triangle);
    void        AdjustForPauseAndTimeDistortion() const;
    void        RenderAttractMode() const;
    void        RenderGame() const;

    void       SpawnPlayer();
    void       SpawnShop();
    void       SpawnEntity();
    Triangle*  SpawnTriangle();
    void       DestroyEntity();
    void       ShowShop();
    void       DestroyShop();
    static int s_nextEntityID;
    Camera*    m_screenCamera = nullptr;
    eGameState m_gameState    = eGameState::ATTRACT;
    Clock*     m_gameClock    = nullptr;

    float m_spawnTimer    = 0.0f;          // 累積時間
    float m_spawnInterval = 10.0f;      // 生成間隔（10秒）

    SoundPlaybackID m_attractPlaybackID;
    SoundPlaybackID m_ingamePlaybackID;
};
