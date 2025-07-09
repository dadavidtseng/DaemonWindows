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
    Entity(Vec2 const& position, float orientationDegrees);
    virtual  ~Entity() = default;
    ActorID  m_actorID            = 0;
    WindowID m_windowID           = 0;
    String   m_name               = "DEFAULT";
    Vec2     m_position           = Vec2::ZERO;
    Vec2     m_velocity           = Vec2::ZERO;
    int      m_health             = 100;
    float    m_orientationDegrees = 0.f;
    float    m_physicRadius       = 0.f;
    float    m_cosmeticRadius     = 0.f;
    float    m_thickness          = 0.f;

    virtual void Update(float deltaSeconds) = 0;
    virtual void Render() const = 0;
    virtual void UpdateFromInput() = 0;

    // 共同邏輯放在基類
    void TakeDamage(int damage);
    void Move(Vec2 direction, float speed);
    bool IsAlive() const { return m_health > 0; }

protected:
    float m_speed = 100.0f;
};
