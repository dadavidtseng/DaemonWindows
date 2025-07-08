//----------------------------------------------------------------------------------------------------
// Entity.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

#include "Game/Framework/GameCommon.hpp"
#include "Game/Subsystem/Window/WindowSubsystem.hpp"

//----------------------------------------------------------------------------------------------------
class Entity
{
public:
    virtual  ~Entity() = default;
    ActorID  m_actorID  = 0;
    WindowID m_windowID = 0;
    String   m_name     = "DEFAULT";
    Vec2     m_position = Vec2::ZERO;
    Vec2     m_velocity = Vec2::ZERO;
    int      m_health   = 100;

    virtual void Update(float deltaTime) = 0;
    virtual void Render() const;

    // 共同邏輯放在基類
    void TakeDamage(int damage);
    void Move(Vec2 direction, float speed);
    bool IsAlive() const { return m_health > 0; }

protected:
    float m_speed = 100.0f;
};
