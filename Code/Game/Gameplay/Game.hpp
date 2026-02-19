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
class Circle;
class Hexagon;
class Octagon;
class Pentagon;
class Square;
class Triangle;
class Coin;
class WaveManager;
class UpgradeManager;

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

    //------------------------------------------------------------------------------------------------
    // Accessors / Mutators
    //------------------------------------------------------------------------------------------------
    eGameState           GetCurrentGameState() const;
    void                 ChangeGameState(eGameState newGameState);
    Clock*               GetGameClock() const;
    Player*              GetPlayer() const;
    Shop*                GetShop() const;
    WaveManager*         GetWaveManager() const;
    UpgradeManager*      GetUpgradeManager() const;
    Entity*              GetEntityByEntityID(EntityID const& entityID) const;

    //------------------------------------------------------------------------------------------------
    // Public data
    //------------------------------------------------------------------------------------------------
    std::vector<Entity*> m_entityList;

private:
    //------------------------------------------------------------------------------------------------
    // Static
    //------------------------------------------------------------------------------------------------
    static int  s_nextEntityID;
    static bool OnGameStateChanged(EventArgs& args);
    static bool OnEntityDestroyed(EventArgs& args);
    static bool OnWaveStart(EventArgs& args);
    static bool OnWaveComplete(EventArgs& args);
    static bool OnBossSpawn(EventArgs& args);
    static bool OnUpgradePurchased(EventArgs& args);

    //------------------------------------------------------------------------------------------------
    // Update / Render
    //------------------------------------------------------------------------------------------------
    void UpdateFromInput();
    void HandleEntityCollision();
    void FireCollisionEvent(Entity* entityA, Entity* entityB);
    void HandleBulletTriangleCollision(Bullet* bullet, Triangle* triangle);
    void HandlePlayerCoinCollision(Player* player, Coin* coin);
    void HandlePlayerTriangleCollision(Player* player, Triangle* triangle);
    void AdjustForPauseAndTimeDistortion() const;
    void RenderAttractMode() const;
    void RenderGame() const;

    //------------------------------------------------------------------------------------------------
    // Entity management
    //------------------------------------------------------------------------------------------------
    void      SpawnPlayer();
    void      SpawnShop();
    void      SpawnEntity();
    Triangle* SpawnTriangle();
    Circle*   SpawnCircle();
    Octagon*  SpawnOctagon();
    Square*   SpawnSquare();
    Pentagon* SpawnPentagon();
    Hexagon*  SpawnHexagon();
    void      DestroyEntity();
    void      ShowShop();
    void      DestroyShop();

    //------------------------------------------------------------------------------------------------
    // Data members
    //------------------------------------------------------------------------------------------------
    Camera*      m_screenCamera  = nullptr;
    eGameState   m_gameState     = eGameState::ATTRACT;
    Clock*       m_gameClock     = nullptr;
    WaveManager*    m_waveManager    = nullptr;
    UpgradeManager* m_upgradeManager = nullptr;

    float m_spawnTimer    = 0.0f;
    float m_spawnInterval = 10.0f;

    SoundPlaybackID m_attractPlaybackID;
    SoundPlaybackID m_ingamePlaybackID;
};
