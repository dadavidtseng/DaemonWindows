//----------------------------------------------------------------------------------------------------
// Shop.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

enum eItemType : int8_t
{
    INCREASE_SPEED,
    INCREASE_HEALTH,
    INCREASE_MAX_HEALTH,
};

struct Item
{
    eItemType m_type;
};

//----------------------------------------------------------------------------------------------------
class Shop : public Entity
{
public:
    explicit Shop(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Shop() override;

    void Update(float deltaSeconds) override;
    void Render() const override;

private:
    static bool OnGameStateChanged(EventArgs& args);
    void        UpdateFromInput(float deltaSeconds) override;

    std::shared_ptr<ButtonWidget> m_itemWidgetA;
    std::shared_ptr<ButtonWidget> m_itemWidgetB;
    std::shared_ptr<ButtonWidget> m_itemWidgetC;
    std::vector<Item>             m_itemList;
};
