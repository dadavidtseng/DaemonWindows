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
    explicit Entity(Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    virtual  ~Entity() = default;
    EntityID  m_actorID            = 0;
    WindowID m_windowID           = 0;
    String   m_name               = "DEFAULT";
    Vec2     m_position           = Vec2::ZERO;
    Vec2     m_velocity           = Vec2::ZERO;
    Rgba8    m_color              = Rgba8::WHITE;
    int      m_health             = 3;
    float    m_orientationDegrees = 0.f;
    float    m_physicRadius       = 0.f;
    float    m_cosmeticRadius     = 0.f;
    float    m_thickness          = 0.f;

    virtual void Update(float deltaSeconds);
    virtual void Render() const = 0;
    virtual void UpdateFromInput() = 0;

    virtual void MarkAsDead();
    virtual void MarkAsGarbage();

    virtual bool IsDead() const;
    virtual bool IsGarbage() const;

    void DecreaseHealth(int damage);

protected:
    float m_speed     = 100.f;
    bool  m_isDead    = false;
    bool  m_isGarbage = false;
};
