//----------------------------------------------------------------------------------------------------
// Shop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Shop.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
Shop::Shop(EntityID const entityID,
           Vec2 const&    position,
           float const    orientationDegrees,
           Rgba8 const&   color,
           bool const     isVisible,
           bool const     hasChildWindow)
    : Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
    m_entityID       = entityID;
    m_name           = "Shop";
    m_health         = 999;
    m_physicRadius   = 30.f;
    m_thickness      = 10.f;
    m_cosmeticRadius = m_physicRadius + m_thickness;

    g_eventSystem->SubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);

    if (m_hasChildWindow)
    {
        g_windowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 700, 500);

        Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
        Vec2    windowClientPosition  = window->GetClientPosition();
        Vec2    windowClientDimension = window->GetClientDimensions();

        for (int i = 0; i < 3; ++i)
        {
            m_slotWidgets[i] = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, "", (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
            g_widgetSubsystem->AddWidget(m_slotWidgets[i], 999);
            m_slotWidgets[i]->SetVisible(false);
        }

        m_restockWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, "", (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        g_widgetSubsystem->AddWidget(m_restockWidget, 999);
        m_restockWidget->SetVisible(false);
    }

    // Initialize with random upgrades
    RandomizeSlots();
}

//----------------------------------------------------------------------------------------------------
Shop::~Shop()
{
    if (m_hasChildWindow)
    {
        g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
        for (int i = 0; i < 3; ++i)
        {
            m_slotWidgets[i]->MarkForDestroy();
        }
        m_restockWidget->MarkForDestroy();
    }
    g_eventSystem->UnsubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
}

//----------------------------------------------------------------------------------------------------
void Shop::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);

    if (m_hasChildWindow)
    {
        WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }

    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
    Vec2        windowPos  = windowData->m_window->GetClientPosition();
    Vec2        windowDim  = windowData->m_window->GetClientDimensions();

    // Position 3 slot widgets evenly across the shop window
    // Window coords: Y increases downward in screen space
    // But widget system uses Y-up, so higher Y = higher on screen
    float slotWidth  = 180.f;
    float slotHeight = 180.f;
    float totalWidth = 3.f * slotWidth + 2.f * 30.f; // 30px gap between slots
    float startX     = windowPos.x + (windowDim.x - totalWidth) * 0.5f;
    float slotY      = windowPos.y + windowDim.y * 0.55f; // Upper-middle area

    for (int i = 0; i < 3; ++i)
    {
        float x = startX + static_cast<float>(i) * (slotWidth + 30.f);
        m_slotWidgets[i]->SetPosition(Vec2(x, slotY));
        m_slotWidgets[i]->SetDimensions(Vec2(slotWidth, slotHeight));
    }

    // Position restock button below the slots
    float restockWidth  = 140.f;
    float restockHeight = 45.f;
    m_restockWidget->SetPosition(Vec2(windowPos.x + (windowDim.x - restockWidth) * 0.5f, windowPos.y + windowDim.y * 0.2f));
    m_restockWidget->SetDimensions(Vec2(restockWidth, restockHeight));
}

//----------------------------------------------------------------------------------------------------
void Shop::Render() const
{
    VertexList_PCU verts;

    // Draw 3 slot backgrounds
    WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
    Vec2        windowPos  = windowData->m_window->GetClientPosition();
    Vec2        windowDim  = windowData->m_window->GetClientDimensions();

    float slotWidth  = 180.f;
    float slotHeight = 180.f;
    float totalWidth = 3.f * slotWidth + 2.f * 30.f;
    float startX     = windowPos.x + (windowDim.x - totalWidth) * 0.5f;
    float slotY      = windowPos.y + windowDim.y * 0.55f;

    for (int i = 0; i < 3; ++i)
    {
        float x = startX + static_cast<float>(i) * (slotWidth + 30.f);
        Vec2 mins(x, slotY);
        Vec2 maxs(x + slotWidth, slotY + slotHeight);
        AddVertsForAABB2D(verts, AABB2(mins, maxs));
    }

    // Draw restock button background
    float restockWidth  = 140.f;
    float restockHeight = 45.f;
    Vec2  restockMins(windowPos.x + (windowDim.x - restockWidth) * 0.5f, windowPos.y + windowDim.y * 0.2f);
    Vec2  restockMaxs(restockMins.x + restockWidth, restockMins.y + restockHeight);
    AddVertsForAABB2D(verts, AABB2(restockMins, restockMaxs));

    g_renderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 200));
    g_renderer->SetBlendMode(eBlendMode::ALPHA);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_renderer->SetDepthMode(eDepthMode::DISABLED);
    g_renderer->BindTexture(nullptr);
    g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_renderer->DrawVertexArray(verts);

    // Update widget text for each slot
    UpgradeManager* upgradeMgr = g_game->GetUpgradeManager();
    for (int i = 0; i < 3; ++i)
    {
        std::string displayName = UpgradeManager::UpgradeTypeToDisplayName(m_slots[i].m_type);
        int         level       = upgradeMgr->GetUpgradeLevel(m_slots[i].m_type);
        int         cost        = upgradeMgr->GetUpgradeCost(m_slots[i].m_type);
        m_slotWidgets[i]->SetText(Stringf("lv. %d\n%s\n* %d", level, displayName.c_str(), cost));
    }

    // Update restock widget text
    m_restockWidget->SetText(Stringf("restock\n* %d", m_restockCost));
}

//----------------------------------------------------------------------------------------------------
STATIC bool Shop::OnGameStateChanged(EventArgs& args)
{
    String const preGameState = args.GetValue("preGameState", "DEFAULT");
    String const curGameState = args.GetValue("curGameState", "DEFAULT");

    Shop* shop = g_game->GetShop();
    if (shop == nullptr) return false;

    if (curGameState == "SHOP")
    {
        // Play shop open sound
        SoundID const shopOpenSFX = g_audio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
        g_audio->StartSound(shopOpenSFX, false, 1.f, 0.f, 0.5f);

        // Entering shop: show widgets and randomize slots
        for (int i = 0; i < 3; ++i)
        {
            shop->m_slotWidgets[i]->SetVisible(true);
        }
        shop->m_restockWidget->SetVisible(true);
        shop->RandomizeSlots();
        shop->m_restockCount = 0;
        shop->m_restockCost  = 1;
    }
    else if (preGameState == "SHOP")
    {
        // Leaving shop: hide widgets
        for (int i = 0; i < 3; ++i)
        {
            shop->m_slotWidgets[i]->SetVisible(false);
        }
        shop->m_restockWidget->SetVisible(false);
    }

    return false;
}

//----------------------------------------------------------------------------------------------------
void Shop::UpdateFromInput(float const deltaSeconds)
{
    UNUSED(deltaSeconds)

    UpgradeManager* upgradeMgr = g_game->GetUpgradeManager();
    if (!upgradeMgr) return;

    // Mouse click on slot widgets or restock button
    if (g_input->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
    {
        Vec2 cursorPos = Window::s_mainWindow->GetCursorPositionOnScreen();

        // Check slot clicks
        WindowID    windowID   = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData = g_windowSubsystem->GetWindowData(windowID);
        Vec2        windowPos  = windowData->m_window->GetClientPosition();
        Vec2        windowDim  = windowData->m_window->GetClientDimensions();

        float slotWidth  = 180.f;
        float slotHeight = 180.f;
        float totalWidth = 3.f * slotWidth + 2.f * 30.f;
        float startX     = windowPos.x + (windowDim.x - totalWidth) * 0.5f;
        float slotY      = windowPos.y + windowDim.y * 0.55f;

        for (int i = 0; i < 3; ++i)
        {
            float x = startX + static_cast<float>(i) * (slotWidth + 30.f);
            if (cursorPos.x >= x && cursorPos.x <= x + slotWidth &&
                cursorPos.y >= slotY && cursorPos.y <= slotY + slotHeight)
            {
                if (upgradeMgr->PurchaseUpgrade(m_slots[i].m_type))
                {
                    SoundID const buySFX = g_audio->CreateOrGetSound("Data/Audio/coin.mp3", eAudioSystemSoundDimension::Sound2D);
                    g_audio->StartSound(buySFX, false, 1.f, 0.f, 1.f);
                }
                break;
            }
        }

        // Check restock button click
        float restockWidth  = 140.f;
        float restockHeight = 45.f;
        float restockX      = windowPos.x + (windowDim.x - restockWidth) * 0.5f;
        float restockY      = windowPos.y + windowDim.y * 0.2f;

        if (cursorPos.x >= restockX && cursorPos.x <= restockX + restockWidth &&
            cursorPos.y >= restockY && cursorPos.y <= restockY + restockHeight)
        {
            Restock();
            SoundID const restockSFX = g_audio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
            g_audio->StartSound(restockSFX, false, 1.f, 0.f, 0.8f);
        }
    }

    // Key R for restock (keep as convenience)
    if (g_input->WasKeyJustPressed('R'))
    {
        Restock();
        SoundID const restockSFX = g_audio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
        g_audio->StartSound(restockSFX, false, 1.f, 0.f, 0.8f);
    }
}

//----------------------------------------------------------------------------------------------------
void Shop::Restock()
{
    Player* player = g_game->GetPlayer();
    if (!player) return;

    if (player->m_coin < m_restockCost) return;

    // Deduct restock cost
    player->m_coin -= m_restockCost;
    player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));

    // Escalate restock cost: 1 + 10 per refresh
    m_restockCount++;
    m_restockCost = 1 + 10 * m_restockCount;

    // Randomize unlocked slots
    RandomizeSlots();
}

//----------------------------------------------------------------------------------------------------
void Shop::RandomizeSlots()
{
    int const numTypes = static_cast<int>(eUpgradeType::COUNT);

    for (int i = 0; i < 3; ++i)
    {
        if (m_slots[i].m_locked) continue;

        m_slots[i].m_type = static_cast<eUpgradeType>(g_rng->RollRandomIntInRange(0, numTypes - 1));
    }
}
