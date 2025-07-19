//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"

#include "Bullet.hpp"
#include "Triangle.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Triangle2.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Framework/App.hpp"
#include "Game/Framework/GameCommon.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    m_screenCamera = new Camera();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Window::s_mainWindow->GetScreenDimensions();

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    m_screenCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);

    m_gameClock = new Clock(Clock::GetSystemClock());

    m_entities.push_back(new Player(0, Window::s_mainWindow->GetScreenDimensions() * 0.5f, 0.f, Rgba8::YELLOW));
    m_entities.push_back(new Triangle(1, Vec2(g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().x), g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().y)), 0.f, Rgba8::BLUE));
    m_entities.push_back(new Triangle(2, Vec2(g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().x), g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().y)), 0.f, Rgba8::BLUE));
    m_entities.push_back(new Triangle(3, Vec2(g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().x), g_theRNG->RollRandomFloatInRange(0, Window::s_mainWindow->GetScreenDimensions().y)), 0.f, Rgba8::BLUE));

    // auto test1 = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, "test1", 0, 0, 300, 300, Rgba8::BLUE);
    // g_theWidgetSubsystem->AddWidget(test1, 100);
    // auto test2 = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, "test2", 300, 200, 500, 500, Rgba8::WHITE);
    // g_theWidgetSubsystem->AddWidget(test2, 10);

    // g_theWindowSubsystem->CreateChildWindow(-1, "Background Window");
}

Game::~Game()
{
    GAME_SAFE_RELEASE(m_screenCamera);
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    float gameDeltaSeconds = (float)m_gameClock->GetDeltaSeconds();

    UpdateFromInput();
    AdjustForPauseAndTimeDistortion();
    HandleEntityCollision();
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        Entity* entity = m_entities[i];
        if (entity != nullptr)  // 先檢查指標
        {
            if (!entity->IsDead())  // 再檢查是否存活
            {
                entity->Update(gameDeltaSeconds);
            }
            else
            {
                // 清理死亡的實體
                delete entity;
                m_entities.erase(m_entities.begin() + i);
                --i;  // 調整索引
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    //-Start-of-Screen-Camera-------------------------------------------------------------------------
    g_theRenderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::ATTRACT)
    {
        RenderAttractMode();
    }
    else if (m_gameState == eGameState::GAME)
    {
        RenderGame();
    }

    g_theRenderer->EndCamera(*m_screenCamera);
    //-End-of-Screen-Camera---------------------------------------------------------------------------

    DebugRenderScreen(*m_screenCamera);
    // if (m_gameState == eGameState::GAME)
    // {
    //     DebugRenderScreen(*m_screenCamera);
    // }
}

bool Game::OnGameStateChanged(EventArgs& args)
{
    String const newGameState = args.GetValue("OnGameStateChanged", "DEFAULT");

    if (newGameState == "ATTRACT")
    {
        App::RequestQuit();
    }

    return true;
}

eGameState Game::GetCurrentGameState() const
{
    return m_gameState;
}

void Game::ChangeGameState(eGameState const newGameState)
{
    if (newGameState == m_gameState) return;

    EventArgs args;

    if (newGameState == eGameState::ATTRACT) args.SetValue("OnGameStateChanged", "ATTRACT");
    else if (newGameState == eGameState::GAME) args.SetValue("OnGameStateChanged", "GAME");

    m_gameState = newGameState;

    g_theEventSystem->FireEvent("OnGameStateChanged", args);
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromInput()
{
    if (m_gameState == eGameState::ATTRACT)
    {
        if (g_theInput->IsKeyDown(KEYCODE_I))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(0), Vec2(0, 10));
        }
        if (g_theInput->IsKeyDown(KEYCODE_K))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(0), Vec2(0, -10));
        }
        if (g_theInput->IsKeyDown(KEYCODE_J))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(0), Vec2(-10, 0));
        }
        if (g_theInput->IsKeyDown(KEYCODE_L))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(0), Vec2(10, 0));
        }
        if (g_theInput->IsKeyDown(KEYCODE_UPARROW))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(1), Vec2(0, 10));
        }
        if (g_theInput->IsKeyDown(KEYCODE_DOWNARROW))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(1), Vec2(0, -10));
        }
        if (g_theInput->IsKeyDown(KEYCODE_LEFTARROW))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(1), Vec2(-10, 0));
        }
        if (g_theInput->IsKeyDown(KEYCODE_RIGHTARROW))
        {
            g_theWindowSubsystem->UpdateWindowPosition(g_theWindowSubsystem->FindWindowIDByEntityID(1), Vec2(10, 0));
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
        {
            ChangeGameState(eGameState::GAME);
            SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
            g_theAudio->StartSound(clickSound, false, 1.f, 0.f, 0.5f);
            // g_theWindowSubsystem->CreateChildWindow(EntityID(1), "HELLO");
        }
    }
    else if (m_gameState == eGameState::GAME)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            ChangeGameState(eGameState::ATTRACT);
            SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
            g_theAudio->StartSound(clickSound);
        }
    }
}

void Game::HandleEntityCollision()
{
    for (int i = 0; i < m_entities.size(); ++i)
    {
        Entity* entityA = m_entities[i];
        if (entityA->IsDead()) continue;

        for (int j = 0; j < m_entities.size(); ++j)
        {
            if (i == j) continue;

            Entity* entityB = m_entities[j];
            if (entityB->IsDead()) continue;
            // 檢查兩個實體是否發生碰撞
            if (DoDiscsOverlap2D(entityA->m_position, entityA->m_physicRadius, entityB->m_position, entityB->m_physicRadius))
            {
                // 處理碰撞反應
                // entityA->m_color = entityB->m_color;

                // 方法1: 使用 dynamic_cast (如果有繼承關係和虛擬函式)
                Bullet*   bulletA  = dynamic_cast<Bullet*>(entityA);
                Triangle* triangle = dynamic_cast<Triangle*>(entityB);
                if (bulletA != nullptr && triangle != nullptr)
                {
                    // entityA 是 Bullet 類別
                    bulletA->DecreaseHealth(1);
                    triangle->DecreaseHealth(1);
                }
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------
void Game::AdjustForPauseAndTimeDistortion() const
{
    if (g_theInput->WasKeyJustPressed(KEYCODE_P))
    {
        m_gameClock->TogglePause();
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_O))
    {
        m_gameClock->StepSingleFrame();
    }

    if (g_theInput->IsKeyDown(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(0.1f);
    }

    if (g_theInput->WasKeyJustReleased(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(1.f);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    VertexList_PCU verts1;
    AddVertsForAABB2D(verts1, AABB2(Vec2::ZERO, Window::s_mainWindow->GetScreenDimensions()));
    g_theRenderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 100));
    g_theRenderer->SetBlendMode(eBlendMode::ALPHA);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(g_theRenderer->CreateOrGetTextureFromFile("Data/Images/goop.png"));
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts1);

    HWND    hwnd        = GetFocus();
    wchar_t wTitle[256] = L"";
    GetWindowTextW(hwnd, wTitle, 256);
    std::wstring ws(wTitle);
    std::string  title(ws.begin(), ws.end());
    DebugAddScreenText(Stringf("NormalizedMouseUV(%.2f, %.2f)", Window::s_mainWindow->GetNormalizedMouseUV().x, Window::s_mainWindow->GetNormalizedMouseUV().y), m_screenCamera->GetOrthographicBottomLeft(), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("CursorPositionOnScreen(%.1f, %.1f)", Window::s_mainWindow->GetCursorPositionOnScreen().x, Window::s_mainWindow->GetCursorPositionOnScreen().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 20), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Focus Window(%s)", title.c_str()), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 40), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Client Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetClientDimensions().x, Window::s_mainWindow->GetClientDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 60), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Viewport Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetViewportDimensions().x, Window::s_mainWindow->GetViewportDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 80), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Screen Dimensions(%.1f, %.1f)", Window::s_mainWindow->GetScreenDimensions().x, Window::s_mainWindow->GetScreenDimensions().y), m_screenCamera->GetOrthographicBottomLeft() + Vec2(0, 100), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);

    for (Entity* entity : m_entities)
    {
        if (entity && !entity->IsDead())
        {
            entity->Render();
        }
    }

    VertexList_PCU verts2;
    // AddVertsForAABB2D(verts2, AABB2(Vec2(108,69),Vec2(108+384,69+261)));
    // AddVertsForAABB2D(verts2, AABB2(Vec2(100,100),Vec2(100+400,100+300)));
    g_theRenderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 100));
    g_theRenderer->SetBlendMode(eBlendMode::ALPHA);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(nullptr);
    g_theRenderer->DrawVertexArray(verts2);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderGame() const
{
    VertexList_PCU verts1;
    AddVertsForAABB2D(verts1, AABB2(Vec2::ZERO, Window::s_mainWindow->GetScreenDimensions()));
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(g_theRenderer->CreateOrGetTextureFromFile("Data/Images/serenity.png"));
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts1);

    VertexList_PCU verts2;
    Vec2 const     screenBottomLeft  = m_screenCamera->GetOrthographicBottomLeft();
    Vec2 const     screenTopRight    = m_screenCamera->GetOrthographicTopRight();
    Vec2 const     screenBottomRight = Vec2(screenBottomLeft.x + screenTopRight.x, screenBottomLeft.y);
    Vec2 const     screenTopLeft     = Vec2(screenBottomLeft.x + screenBottomLeft.y, screenTopRight.y);
    AddVertsForLineSegment2D(verts2, screenBottomLeft + Vec2(100, 100), screenTopRight - Vec2(100, 100), 10.f, false, Rgba8::GREEN);
    AddVertsForLineSegment2D(verts2, screenTopLeft + Vec2(100, -100), screenBottomRight + Vec2(-100, 100), 10.f, false, Rgba8::GREEN);
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::OPAQUE);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts2);

    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicTopRight() - Vec2(200.f, 60.f), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicBottomLeft(), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
}
