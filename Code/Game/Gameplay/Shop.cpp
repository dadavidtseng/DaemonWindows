//----------------------------------------------------------------------------------------------------
// Shop.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Shop.hpp"

#include "Game.hpp"
#include "Player.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
#include "Game/Subsystem/Widget/WidgetSubsystem.hpp"

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

    g_theEventSystem->SubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);

    if (m_hasChildWindow)
    {
        g_theWindowSubsystem->CreateChildWindow(m_entityID, m_name, static_cast<int>(m_position.x), static_cast<int>(m_position.y), 700, 500);

        Window* window                = g_theWindowSubsystem->GetWindow(g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID));
        Vec2    windowClientPosition  = window->GetClientPosition();
        Vec2    windowClientDimension = window->GetClientDimensions();

        m_itemWidgetA = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("A=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        m_itemWidgetB = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("B=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        m_itemWidgetC = g_theWidgetSubsystem->CreateWidget<ButtonWidget>(g_theWidgetSubsystem, Stringf("C=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
        g_theWidgetSubsystem->AddWidget(m_itemWidgetA, 999);
        g_theWidgetSubsystem->AddWidget(m_itemWidgetB, 999);
        g_theWidgetSubsystem->AddWidget(m_itemWidgetC, 999);
        m_itemWidgetA->SetVisible(false);
        m_itemWidgetB->SetVisible(false);
        m_itemWidgetC->SetVisible(false);
    }

    Item itemA;
    itemA.m_type = INCREASE_SPEED;
    Item itemB;
    itemB.m_type = INCREASE_HEALTH;
    Item itemC;
    itemC.m_type = INCREASE_MAX_HEALTH;
    m_itemList.push_back(itemA);
    m_itemList.push_back(itemB);
    m_itemList.push_back(itemC);
}

//----------------------------------------------------------------------------------------------------
Shop::~Shop()
{
    if (m_hasChildWindow)
    {
        g_theWindowSubsystem->RemoveEntityFromMappings(m_entityID);
        m_itemWidgetA->MarkForDestroy();
        m_itemWidgetB->MarkForDestroy();
        m_itemWidgetC->MarkForDestroy();
    }
    g_theEventSystem->UnsubscribeEventCallbackFunction("OnGameStateChanged", OnGameStateChanged);
}

//----------------------------------------------------------------------------------------------------
void Shop::Update(float const deltaSeconds)
{
    Entity::Update(deltaSeconds);

    if (m_hasChildWindow)
    {
        WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
        WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
        windowData->m_window->SetClientPosition(m_position - windowData->m_window->GetClientDimensions() * 0.5f);
    }
    WindowID    windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData* windowData = g_theWindowSubsystem->GetWindowData(windowID);
    m_itemWidgetA->SetPosition(windowData->m_window->GetClientPosition() - Vec2(500, -200));
    m_itemWidgetB->SetPosition(windowData->m_window->GetClientPosition() - Vec2(300, -200));
    m_itemWidgetC->SetPosition(windowData->m_window->GetClientPosition() - Vec2(100, -200));
    m_itemWidgetA->SetDimensions(windowData->m_window->GetClientDimensions());
    m_itemWidgetB->SetDimensions(windowData->m_window->GetClientDimensions());
    m_itemWidgetC->SetDimensions(windowData->m_window->GetClientDimensions());
}

//----------------------------------------------------------------------------------------------------
void Shop::Render() const
{
    // if (!m_isVisible) return;
    WindowID       windowID   = g_theWindowSubsystem->FindWindowIDByEntityID(m_entityID);
    WindowData*    windowData = g_theWindowSubsystem->GetWindowData(windowID);
    VertexList_PCU verts;
    AddVertsForAABB2D(verts, AABB2(m_position - Vec2(100, 200), m_position + Vec2(100, 200)));
    AddVertsForAABB2D(verts, AABB2(m_position - Vec2(315, 200), m_position + Vec2(-115, 200)));
    AddVertsForAABB2D(verts, AABB2(m_position - Vec2(-115, 200), m_position + Vec2(315, 200)));
    g_theRenderer->SetModelConstants(Mat44{}, Rgba8(255, 255, 255, 200));
    g_theRenderer->SetBlendMode(eBlendMode::ALPHA);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);


    m_itemWidgetA->SetText(Stringf("speed"));
    m_itemWidgetB->SetText(Stringf("health"));
    m_itemWidgetC->SetText(Stringf("max   \nhealth"));
}

STATIC bool Shop::OnGameStateChanged(EventArgs& args)
{
    String const preGameState = args.GetValue("preGameState", "DEFAULT");
    String const curGameState = args.GetValue("curGameState", "DEFAULT");

    Shop* shop = g_theGame->GetShop();
    if (shop == nullptr) return false;
    if (preGameState == "ATTRACT" && curGameState == "GAME")
    {
        shop->m_itemWidgetA->SetVisible(false);
        shop->m_itemWidgetB->SetVisible(false);
        shop->m_itemWidgetC->SetVisible(false);
    }
    else if (preGameState == "SHOP" && curGameState == "GAME")
    {
        shop->m_itemWidgetA->SetVisible(false);
        shop->m_itemWidgetB->SetVisible(false);
        shop->m_itemWidgetC->SetVisible(false);
    }
    else if (preGameState == "GAME" && curGameState == "SHOP")
    {
        shop->m_itemWidgetA->SetVisible(true);
        shop->m_itemWidgetB->SetVisible(true);
        shop->m_itemWidgetC->SetVisible(true);
    }

    return false;
}

//----------------------------------------------------------------------------------------------------
void Shop::UpdateFromInput()
{
    Player* player = g_theGame->GetPlayer();
    if (player->m_coin <= 0) return;
    if (g_theInput->WasKeyJustPressed(NUMCODE_1))
    {
        player->m_speed += 10;
    }
    else if (g_theInput->WasKeyJustPressed(NUMCODE_2))
    {
        player->m_health += 5;
        player->m_healthWidget->SetText(Stringf("Health=%d/%d", player->m_health, player->m_maxHealth));
        player->m_coin -= 5;
        player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));
    }
    else if (g_theInput->WasKeyJustPressed(NUMCODE_3))
    {
        player->m_maxHealth += 5;
        player->m_healthWidget->SetText(Stringf("Health=%d/%d", player->m_health, player->m_maxHealth));
        player->m_coin -= 10;
        player->m_coinWidget->SetText(Stringf("Coin=%d", player->m_coin));
    }
}
