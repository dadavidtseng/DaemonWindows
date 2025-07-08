//----------------------------------------------------------------------------------------------------
// Bullet.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
    Bullet(Vec2 const& position, float orientationDegrees);
    void Update(float deltaSeconds) override;
    void Render() const override;
    void UpdateFromInput() override;
};
