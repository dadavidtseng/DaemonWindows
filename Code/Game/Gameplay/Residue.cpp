//----------------------------------------------------------------------------------------------------
// Residue.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Residue.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
Residue::Residue(EntityID const entityID,
                 Vec2 const&    position,
                 Rgba8 const&   color)
	: Entity(position, 0.f, color, true, false)
{
	m_entityID       = entityID;
	m_name           = "Residue";
	m_physicRadius   = 12.f;
	m_cosmeticRadius = m_physicRadius;
	m_health         = 999;   // cannot be killed by bullets
	m_speed          = 0.f;   // stationary
}

//----------------------------------------------------------------------------------------------------
Residue::~Residue() = default;

//----------------------------------------------------------------------------------------------------
void Residue::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;

	m_age += deltaSeconds;
	if (m_age >= m_lifetime)
	{
		MarkAsDead();
		return;
	}

	// Tick damage cooldown
	m_damageTimer += deltaSeconds;
}

//----------------------------------------------------------------------------------------------------
void Residue::Render() const
{
	// Fade alpha based on remaining lifetime
	float const fadeRatio = 1.f - (m_age / m_lifetime);
	uint8_t const alpha   = static_cast<uint8_t>(200.f * fadeRatio);
	Rgba8 const drawColor(m_color.r, m_color.g, m_color.b, alpha);

	VertexList_PCU verts;
	constexpr int  NUM_SIDES = 10;

	for (int i = 0; i < NUM_SIDES; ++i)
	{
		float const angle0 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i);
		float const angle1 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i + 1);

		Vec2 const vert0 = m_position + Vec2::MakeFromPolarDegrees(angle0, m_physicRadius);
		Vec2 const vert1 = m_position + Vec2::MakeFromPolarDegrees(angle1, m_physicRadius);

		AddVertsForTriangle2D(verts, m_position, vert0, vert1, drawColor);
	}

	g_renderer->SetModelConstants();
	g_renderer->SetBlendMode(eBlendMode::ALPHA);
	g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
	g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
	g_renderer->SetDepthMode(eDepthMode::DISABLED);
	g_renderer->BindTexture(nullptr);
	g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
	g_renderer->DrawVertexArray(verts);
}

//----------------------------------------------------------------------------------------------------
void Residue::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
bool Residue::CanDamage() const
{
	return m_damageTimer >= m_damageCooldown;
}

//----------------------------------------------------------------------------------------------------
void Residue::ResetDamageCooldown()
{
	m_damageTimer = 0.f;
}
