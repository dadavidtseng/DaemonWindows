//----------------------------------------------------------------------------------------------------
// Shop.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Gameplay/UpgradeManager.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class ButtonWidget;

//----------------------------------------------------------------------------------------------------
// Shop slot: holds a randomly selected upgrade type for display
//----------------------------------------------------------------------------------------------------
struct ShopSlot
{
	eUpgradeType m_type   = eUpgradeType::SPEED;
	bool         m_locked = false; // Right-click to lock prevents restock from changing this slot
};

//----------------------------------------------------------------------------------------------------
class Shop : public Entity
{
public:
	explicit Shop(EntityID entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
	~Shop() override;

	void Update(float deltaSeconds) override;
	void Render() const override;

	// Restock: randomize unlocked slots, costs coins
	void Restock();
	void RandomizeSlots();

private:
	static bool OnGameStateChanged(EventArgs& args);
	void        UpdateFromInput(float deltaSeconds) override;
	void        UpdateSlotWidgetText(int slotIndex) const;

	// 3 upgrade slots
	ShopSlot m_slots[3];

	// Widgets for the 3 slots + restock button
	std::shared_ptr<ButtonWidget> m_slotWidgets[3];
	std::shared_ptr<ButtonWidget> m_restockWidget;

	// Restock cost escalation: 1 + 10 per refresh
	int m_restockCost      = 1;
	int m_restockCount     = 0;
};
