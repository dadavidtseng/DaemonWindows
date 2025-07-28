//----------------------------------------------------------------------------------------------------
// Bullet.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/Core/EventSystem.hpp"
#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
    Bullet(EntityID const& entityID, Vec2 const& position, float orientationDegrees, Rgba8 const& color, bool isVisible, bool hasChildWindow);
    ~Bullet() override;
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput(float deltaSeconds) override;

private:
    static bool OnCollisionEnter(EventArgs& args);
};
