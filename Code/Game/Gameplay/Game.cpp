//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/Circle.hpp"
#include "Game/Gameplay/Coin.hpp"
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Hexagon.hpp"
#include "Game/Gameplay/Octagon.hpp"
#include "Game/Gameplay/Pentagon.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Gameplay/Shop.hpp"
#include "Game/Gameplay/Square.hpp"
#include "Game/Gameplay/Triangle.hpp"
#include "Game/Gameplay/UpgradeManager.hpp"
#include "Game/Gameplay/WaveManager.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Triangle2.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/VertexUtils.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"

// Start entity IDs from 1 (0 is reserved as INVALID_ENTITY_ID)
int Game::s_nextEntityID = 1;

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    g_eventSystem->SubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_eventSystem->SubscribeEventCallbackFunction("OnEntityDestroyed", OnEntityDestroyed);
    g_eventSystem->SubscribeEventCallbackFunction("OnWaveStart", OnWaveStart);
    g_eventSystem->SubscribeEventCallbackFunction("OnWaveComplete", OnWaveComplete);
    g_eventSystem->SubscribeEventCallbackFunction("OnBossSpawn", OnBossSpawn);
    g_eventSystem->SubscribeEventCallbackFunction("OnUpgradePurchased", OnUpgradePurchased);
    m_screenCamera = new Camera();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Window::s_mainWindow->GetScreenDimensions();

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    m_screenCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);

    m_gameClock = new Clock(Clock::GetSystemClock());

    m_waveManager    = new WaveManager(this);
    m_upgradeManager = new UpgradeManager(this);

    SpawnPlayer();
    // TODO: spawn before firing the event will cause nullptr
    SpawnShop();
    Shop* shop = GetShop();
    if (shop != nullptr)
    {
        shop->MarkAsChildWindowInvisible();
    }

    SoundID const attractBGM = g_audio->CreateOrGetSound("Data/Audio/attract.mp3", eAudioSystemSoundDimension::Sound2D);
    m_attractPlaybackID      = g_audio->StartSound(attractBGM, true, 1.f, 0.f, 1.f);
}

//----------------------------------------------------------------------------------------------------
Game::~Game()
{
    g_eventSystem->UnsubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnEntityDestroyed", OnEntityDestroyed);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnWaveStart", OnWaveStart);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnWaveComplete", OnWaveComplete);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnBossSpawn", OnBossSpawn);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnUpgradePurchased", OnUpgradePurchased);
    GAME_SAFE_RELEASE(m_upgradeManager);
    GAME_SAFE_RELEASE(m_waveManager);
    GAME_SAFE_RELEASE(m_screenCamera);
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    float const gameDeltaSeconds = static_cast<float>(m_gameClock->GetDeltaSeconds());

    if (m_gameState == eGameState::GAME)
    {
        // WaveManager handles all enemy spawning (timing, type selection, wave progression)
        if (m_waveManager)
        {
            m_waveManager->Update(gameDeltaSeconds);
        }
    }

    UpdateFromInput();
    AdjustForPauseAndTimeDistortion();
    HandleEntityCollision();
    for (size_t i = 0; i < m_entityList.size(); ++i)
    {
        Entity* entity = m_entityList[i];
        if (entity != nullptr)
        {
            if (!entity->IsDead())
            {
                entity->Update(gameDeltaSeconds);
                entity->UpdateFromInput(gameDeltaSeconds);
            }
            else
            {
                // Clean up dead entity
                delete entity;
                m_entityList.erase(m_entityList.begin() + i);
                --i;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    //-Start-of-Screen-Camera-------------------------------------------------------------------------
    g_renderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::ATTRACT)
    {
        RenderAttractMode();
    }
    else if (m_gameState == eGameState::GAME || m_gameState == eGameState::SHOP)
    {
        RenderGame();
    }

    g_renderer->EndCamera(*m_screenCamera);
    //-End-of-Screen-Camera---------------------------------------------------------------------------
    g_widgetSubsystem->Render();
    DebugRenderScreen(*m_screenCamera);
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnGameStateChanged(EventArgs& args)
{
    String const preGameState = args.GetValue("preGameState", "DEFAULT");
    String const curGameState = args.GetValue("curGameState", "DEFAULT");

    if (preGameState == "ATTRACT" && curGameState == "GAME")
    {
        // WaveManager auto-starts wave 1 on its first Update() call
        g_audio->StopSound(g_game->m_attractPlaybackID);
        SoundID const ingameBGM       = g_audio->CreateOrGetSound("Data/Audio/ingame.mp3", eAudioSystemSoundDimension::Sound2D);
        g_game->m_ingamePlaybackID = g_audio->StartSound(ingameBGM, true, 1.f, 0.f, 1.f);
    }
    else if (preGameState == "GAME" && curGameState == "ATTRACT")
    {
        g_game->DestroyEntity();
        if (g_game->GetWaveManager()) g_game->GetWaveManager()->Reset();

        if (g_game->GetPlayer() == nullptr) g_game->SpawnPlayer();
        g_audio->StopSound(g_game->m_ingamePlaybackID);
        SoundID const attractBGM       = g_audio->CreateOrGetSound("Data/Audio/attract.mp3", eAudioSystemSoundDimension::Sound2D);
        g_game->m_attractPlaybackID = g_audio->StartSound(attractBGM, true, 1.f, 0.f, 1.f);
    }
    else if (preGameState == "GAME" && curGameState == "SHOP")
    {
        g_game->ShowShop();
    }
    else if (preGameState == "SHOP" && curGameState == "GAME")
    {
        g_game->DestroyShop();
    }

    return false;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnEntityDestroyed(EventArgs& args)
{
    String   name     = args.GetValue("name", "DEFAULT");
    EntityID entityID = args.GetValue("entityID", -1);

    if (name == "Coin") return true;

    Entity* entity = g_game->GetEntityByEntityID(entityID);
    if (entity == nullptr) return true;

    // Spawn coins based on m_coinToDrop (minimum 1)
    int const coinCount = (entity->m_coinToDrop > 0) ? entity->m_coinToDrop : 1;
    for (int i = 0; i < coinCount; ++i)
    {
        // Scatter coins slightly around death position for visual variety
        float const scatterAngle = 360.f / static_cast<float>(coinCount) * static_cast<float>(i);
        Vec2 const  offset       = Vec2::MakeFromPolarDegrees(scatterAngle, 10.f * static_cast<float>(i > 0));
        Vec2 const  coinPos      = entity->m_position + offset;

        g_game->m_entityList.push_back(new Coin((int)g_game->m_entityList.size(), coinPos, 0.f, Rgba8::RED, true, false));
    }

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnWaveStart(EventArgs& args)
{
    int const waveNumber = atoi(args.GetValue("waveNumber", "0").c_str());
    bool const isBossWave = args.GetValue("isBossWave", "false") == "true";

    if (isBossWave)
    {
        DebuggerPrintf("Wave %d started (BOSS WAVE)!\n", waveNumber);
    }
    else
    {
        DebuggerPrintf("Wave %d started.\n", waveNumber);
    }

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnWaveComplete(EventArgs& args)
{
    int const waveNumber = atoi(args.GetValue("waveNumber", "0").c_str());
    DebuggerPrintf("Wave %d completed!\n", waveNumber);

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnBossSpawn(EventArgs& args)
{
    int const waveNumber = atoi(args.GetValue("waveNumber", "0").c_str());
    DebuggerPrintf("Boss spawned on wave %d!\n", waveNumber);

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::OnUpgradePurchased(EventArgs& args)
{
    String const upgradeType = args.GetValue("upgradeType", "Unknown");
    int const newLevel = atoi(args.GetValue("newLevel", "0").c_str());

    DebuggerPrintf("Upgrade purchased: %s -> Level %d\n", upgradeType.c_str(), newLevel);

    return true;
}

//----------------------------------------------------------------------------------------------------
eGameState Game::GetCurrentGameState() const
{
    return m_gameState;
}

//----------------------------------------------------------------------------------------------------
static char const* GameStateToString(eGameState state)
{
    switch (state)
    {
    case eGameState::ATTRACT: return "ATTRACT";
    case eGameState::GAME:    return "GAME";
    case eGameState::SHOP:    return "SHOP";
    default:                  return "DEFAULT";
    }
}

//----------------------------------------------------------------------------------------------------
void Game::ChangeGameState(eGameState const newGameState)
{
    if (newGameState == m_gameState) return;

    EventArgs args;
    args.SetValue("preGameState", GameStateToString(m_gameState));
    args.SetValue("curGameState", GameStateToString(newGameState));

    m_gameState = newGameState;

    g_eventSystem->FireEvent("OnGameStateChanged", args);
}

//----------------------------------------------------------------------------------------------------
Clock* Game::GetGameClock() const
{
    return m_gameClock;
}

//----------------------------------------------------------------------------------------------------
Player* Game::GetPlayer() const
{
    for (Entity* entity : m_entityList)
    {
        Player* player = dynamic_cast<Player*>(entity);
        if (player != nullptr) return player;
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------
Shop* Game::GetShop() const
{
    for (Entity* entity : m_entityList)
    {
        Shop* shop = dynamic_cast<Shop*>(entity);
        if (shop != nullptr) return shop;
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------
WaveManager* Game::GetWaveManager() const
{
    return m_waveManager;
}

//----------------------------------------------------------------------------------------------------
UpgradeManager* Game::GetUpgradeManager() const
{
    return m_upgradeManager;
}

//----------------------------------------------------------------------------------------------------
Entity* Game::GetEntityByEntityID(EntityID const& entityID) const
{
    for (Entity* entity : m_entityList)
    {
        if (entity && entity->m_entityID == entityID)
        {
            return entity;
        }
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromInput()
{
    SoundID const clickSound = g_audio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);

    if (m_gameState == eGameState::ATTRACT)
    {
        if (g_input->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_input->WasKeyJustPressed(KEYCODE_SPACE))
        {
            ChangeGameState(eGameState::GAME);
            g_audio->StartSound(clickSound, false, 1.f, 0.f, 0.5f);
        }
    }
    else if (m_gameState == eGameState::GAME)
    {
        if (g_input->WasKeyJustPressed(KEYCODE_ESC))
        {
            ChangeGameState(eGameState::ATTRACT);
            g_audio->StartSound(clickSound);
        }
        if (g_input->WasKeyJustPressed(KEYCODE_SPACE))
        {
            ChangeGameState(eGameState::SHOP);
            g_audio->StartSound(clickSound, false, 10.f, 0.f, 1.f);
        }
    }
    else if (m_gameState == eGameState::SHOP)
    {
        if (g_input->WasKeyJustPressed(KEYCODE_ESC))
        {
            ChangeGameState(eGameState::GAME);
            g_audio->StartSound(clickSound, false, 10.f, 0.f, 1.f);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::FireCollisionEvent(Entity* entityA, Entity* entityB)
{
    EventArgs args;
    args.SetValue("entityA", entityA->m_name);
    args.SetValue("entityAID", std::to_string(entityA->m_entityID));
    args.SetValue("entityB", entityB->m_name);
    args.SetValue("entityBID", std::to_string(entityB->m_entityID));
    g_eventSystem->FireEvent("OnCollisionEnter", args);
}

//----------------------------------------------------------------------------------------------------
// IsEnemy - Returns true if the entity is an enemy type (not player, bullet, coin, shop, debris)
//----------------------------------------------------------------------------------------------------
bool Game::IsEnemy(Entity const* entity)
{
    if (!entity) return false;

    String const& name = entity->m_name;
    return name != "You"
        && name != "Bullet"
        && name != "EnemyBullet"
        && name != "Coin"
        && name != "Shop"
        && name != "Debris"
        && name != "DEFAULT";
}

//----------------------------------------------------------------------------------------------------
void Game::HandleBulletEnemyCollision(Bullet* bullet, Entity* enemy)
{
    FireCollisionEvent(bullet, enemy);

    // Kill bullet immediately so it can't hit multiple enemies in the same frame
    bullet->MarkAsDead();

    enemy->DecreaseHealth(1);

    // Per-type knockback: tanky enemies resist, others get moderate pushback
    float knockbackDist = 15.f;
    if (enemy->m_name == "Square")
    {
        knockbackDist = 5.f;   // tanky — minimal knockback
    }

    // Apply knockback in direction opposite to enemy velocity, clamped to prevent teleporting
    if (enemy->m_velocity.GetLengthSquared() > 0.f)
    {
        Vec2 const knockbackDir = enemy->m_velocity.GetNormalized();
        enemy->m_position -= knockbackDir * knockbackDist;
    }

    SoundID const hitSound = g_audio->CreateOrGetSound("Data/Audio/hit.mp3", eAudioSystemSoundDimension::Sound2D);
    g_audio->StartSound(hitSound, false, 1.f, 0.f, 1.f);
}

//----------------------------------------------------------------------------------------------------
void Game::HandlePlayerCoinCollision(Player* player, Coin* coin)
{
    FireCollisionEvent(player, coin);

    coin->DecreaseHealth(1);

    SoundID const coinSound = g_audio->CreateOrGetSound("Data/Audio/coin.mp3", eAudioSystemSoundDimension::Sound2D);
    g_audio->StartSound(coinSound, false, 1.f, 0.f, 1.f);
}

//----------------------------------------------------------------------------------------------------
void Game::HandlePlayerEnemyCollision(Player* player, Entity* enemy)
{
    FireCollisionEvent(player, enemy);
}

//----------------------------------------------------------------------------------------------------
void Game::HandleEntityCollision()
{
    for (int i = 0; i < (int)m_entityList.size(); ++i)
    {
        Entity* entityA = m_entityList[i];
        if (entityA->IsDead()) continue;

        for (int j = i + 1; j < (int)m_entityList.size(); ++j)
        {
            Entity* entityB = m_entityList[j];
            if (entityB->IsDead()) continue;
            if (!DoDiscsOverlap2D(entityA->m_position, entityA->m_physicRadius, entityB->m_position, entityB->m_physicRadius))
                continue;

            Bullet* bulletA = dynamic_cast<Bullet*>(entityA);
            Bullet* bulletB = dynamic_cast<Bullet*>(entityB);
            Player* playerA = dynamic_cast<Player*>(entityA);
            Player* playerB = dynamic_cast<Player*>(entityB);
            Coin*   coinA   = dynamic_cast<Coin*>(entityA);
            Coin*   coinB   = dynamic_cast<Coin*>(entityB);

            // Player bullet vs Enemy (all enemy types; excludes EnemyBullets)
            if (bulletA && bulletA->m_name == "Bullet" && IsEnemy(entityB))
                HandleBulletEnemyCollision(bulletA, entityB);
            else if (bulletB && bulletB->m_name == "Bullet" && IsEnemy(entityA))
                HandleBulletEnemyCollision(bulletB, entityA);
            // Player vs Coin
            else if (playerA && coinB)
                HandlePlayerCoinCollision(playerA, coinB);
            else if (playerB && coinA)
                HandlePlayerCoinCollision(playerB, coinA);
            // Player vs Enemy (all enemy types)
            else if (playerA && IsEnemy(entityB))
                HandlePlayerEnemyCollision(playerA, entityB);
            else if (playerB && IsEnemy(entityA))
                HandlePlayerEnemyCollision(playerB, entityA);

            // If entityA died during collision handling (e.g., bullet hit enemy),
            // stop checking it against remaining entities
            if (entityA->IsDead()) break;
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::AdjustForPauseAndTimeDistortion() const
{
    if (g_input->WasKeyJustPressed(KEYCODE_P))
    {
        m_gameClock->TogglePause();
    }

    if (g_input->WasKeyJustPressed(KEYCODE_O))
    {
        m_gameClock->StepSingleFrame();
    }

    if (g_input->IsKeyDown(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(0.1f);
    }

    if (g_input->WasKeyJustReleased(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(1.f);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    VertexList_PCU verts1;
    AddVertsForAABB2D(verts1, AABB2(Vec2::ZERO, Window::s_mainWindow->GetScreenDimensions()));
    g_renderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 100));
    g_renderer->SetBlendMode(eBlendMode::ALPHA);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(g_resourceSubsystem->CreateOrGetTextureFromFile("Data/Images/serenity.png"));
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts1);

    HWND hwnd        = GetFocus();
    char wTitle[256] = "";
    GetWindowTextA(hwnd, wTitle, 256);

    DebugAddScreenText(Stringf("NormalizedMouseUV(%.2f, %.2f)", Window::s_mainWindow->GetNormalizedMouseUV().x, Window::s_mainWindow->GetNormalizedMouseUV().y), m_screenCamera->GetOrthographicBottomLeft(), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("CursorPositionOnScreen(%.1f, %.1f)", Window::s_mainWindow->GetCursorPositionOnScreen().x, Window::s_mainWindow->GetCursorPositionOnScreen().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 20), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Focus Window(%s)", wTitle), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 40), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Client Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetClientDimensions().x, Window::s_mainWindow->GetClientDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 60), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Viewport Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetViewportDimensions().x, Window::s_mainWindow->GetViewportDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 80), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Screen Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetScreenDimensions().x, Window::s_mainWindow->GetScreenDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 100), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);

    for (Entity* entity : m_entityList)
    {
        if (entity && !entity->IsDead() && entity->m_name == "You")
        {
            entity->Render();
        }
    }

    VertexList_PCU verts2;
    Vec2           offset = Vec2((1445 * 0.5f), (248 * 0.5f));
    Player*        player = GetPlayer();
    if (player != nullptr)
    {
        AddVertsForAABB2D(verts2, AABB2(Vec2(player->m_position - offset * 0.5f), Vec2(player->m_position + offset * 0.5f)));
        g_renderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 100));
        g_renderer->SetBlendMode(eBlendMode::ALPHA);
        g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
        g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
        g_renderer->SetDepthMode(eDepthMode::DISABLED);
        g_renderer->BindTexture(g_resourceSubsystem->CreateOrGetTextureFromFile("Data/Images/title.png"));
        g_renderer->BindShader(nullptr);
        g_renderer->DrawVertexArray(verts2);

        VertexList_PCU verts3;
        Vec2           offset2    = Vec2(0, -80);
        BitmapFont*    bitmapFont = g_resourceSubsystem->CreateOrGetBitmapFontFromFile("Data/Fonts/DaemonFont");
        bitmapFont->AddVertsForTextInBox2D(verts3, Stringf("Press Space to Start\nWASD to move, LMB to shoot"), AABB2(Vec2(player->m_position - offset * 0.5f) + offset2, Vec2(player->m_position + offset * 0.5f) + offset2), 20.f, Rgba8::WHITE, 1.f, Vec2(0.5, 0.5f), eTextBoxMode::OVERRUN);

        g_renderer->SetBlendMode(eBlendMode::ALPHA);
        g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
        g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
        g_renderer->SetDepthMode(eDepthMode::DISABLED);
        g_renderer->BindTexture(&bitmapFont->GetTexture());
        g_renderer->BindShader(nullptr);
        g_renderer->DrawVertexArray(verts3);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::RenderGame() const
{
    VertexList_PCU verts1;
    AddVertsForAABB2D(verts1, AABB2(Vec2::ZERO, Window::s_mainWindow->GetScreenDimensions()));
    g_renderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 100));
    g_renderer->SetBlendMode(eBlendMode::ALPHA);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(g_resourceSubsystem->CreateOrGetTextureFromFile("Data/Images/ripple.png"));
    g_renderer->BindShader(g_renderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts1);

    for (Entity* entity : m_entityList)
    {
        if (entity && !entity->IsDead() && entity->IsChildWindowVisible())
        {
            entity->Render();
        }
    }

    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicTopRight() - Vec2(200.f, 60.f), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicBottomLeft(), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
}

//----------------------------------------------------------------------------------------------------
Triangle* Game::SpawnTriangle()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Triangle* triangle = new Triangle(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8::BLUE,
        true,
        randomType
    );

    m_entityList.push_back(triangle);
    return triangle;
}

//----------------------------------------------------------------------------------------------------
Circle* Game::SpawnCircle()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Circle* circle = new Circle(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8::GREEN,
        true,
        randomType
    );

    m_entityList.push_back(circle);
    return circle;
}

//----------------------------------------------------------------------------------------------------
Octagon* Game::SpawnOctagon()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Octagon* octagon = new Octagon(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8::MAGENTA,
        true,
        randomType
    );

    m_entityList.push_back(octagon);
    return octagon;
}

//----------------------------------------------------------------------------------------------------
Square* Game::SpawnSquare()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Square* square = new Square(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8::ORANGE,
        true,
        randomType
    );

    m_entityList.push_back(square);
    return square;
}

//----------------------------------------------------------------------------------------------------
Pentagon* Game::SpawnPentagon()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Pentagon* pentagon = new Pentagon(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8::CYAN,
        true,
        randomType
    );

    m_entityList.push_back(pentagon);
    return pentagon;
}

//----------------------------------------------------------------------------------------------------
Hexagon* Game::SpawnHexagon()
{
    Vec2 const randomPos  = EnemyUtils::GetRandomSpawnPosition(Window::s_mainWindow->GetScreenDimensions());
    int const  randomType = g_rng->RollRandomIntInRange(0, 1);

    Hexagon* hexagon = new Hexagon(
        s_nextEntityID++,
        randomPos,
        0.f,
        Rgba8(220, 50, 50, 255),  // dark red - distinct from Player's yellow
        true,
        randomType,
        true    // large hexagon can split
    );

    m_entityList.push_back(hexagon);
    return hexagon;
}

//----------------------------------------------------------------------------------------------------
// SpawnEntity - Uses WaveManager to select a random enemy type based on spawn weights,
// then delegates to SpawnEnemyByType(). Falls back to spawning one of each type if
// WaveManager is unavailable (pre-wave-system behavior).
//----------------------------------------------------------------------------------------------------
void Game::SpawnEntity()
{
    if (m_waveManager)
    {
        eEnemyType const enemyType = m_waveManager->SelectRandomEnemyType();
        SpawnEnemyByType(enemyType);
    }
    else
    {
        // Fallback: spawn one of each (legacy behavior)
        SpawnTriangle();
        SpawnCircle();
        SpawnOctagon();
        SpawnSquare();
        SpawnPentagon();
        SpawnHexagon();
    }
}

//----------------------------------------------------------------------------------------------------
// SpawnEnemyByType - Factory method that spawns the correct enemy class based on eEnemyType
//----------------------------------------------------------------------------------------------------
Entity* Game::SpawnEnemyByType(eEnemyType enemyType)
{
    switch (enemyType)
    {
    case eEnemyType::TRIANGLE:  return SpawnTriangle();
    case eEnemyType::CIRCLE:    return SpawnCircle();
    case eEnemyType::OCTAGON:   return SpawnOctagon();
    case eEnemyType::SQUARE:    return SpawnSquare();
    case eEnemyType::PENTAGON:  return SpawnPentagon();
    case eEnemyType::HEXAGON:   return SpawnHexagon();
    default:
        DebuggerPrintf("SpawnEnemyByType: Unknown enemy type %d, falling back to Triangle.\n", static_cast<int>(enemyType));
        return SpawnTriangle();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::DestroyEntity()
{
    for (Entity* entity : m_entityList)
    {
        if (entity == nullptr) continue;
        if (entity->m_name == "You") continue;
        if (entity->m_name == "Shop") continue;
        entity->MarkAsDead();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::ShowShop()
{
    Shop* shop = GetShop();
    if (shop != nullptr)
    {
        shop->MarkAsChildWindowVisible();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::DestroyShop()
{
    Shop* shop = GetShop();
    if (shop != nullptr)
    {
        shop->MarkAsChildWindowInvisible();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayer()
{
    m_entityList.push_back(new Player(s_nextEntityID++, Window::s_mainWindow->GetScreenDimensions() * 0.5f, 0.f, Rgba8::YELLOW, true, true));
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnShop()
{
    m_entityList.push_back(new Shop(s_nextEntityID++, Window::s_mainWindow->GetScreenDimensions() * 0.5f, 0.f, Rgba8::BLACK, true, true));
}
