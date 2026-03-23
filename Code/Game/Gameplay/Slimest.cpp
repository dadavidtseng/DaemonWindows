//----------------------------------------------------------------------------------------------------
// Slimest.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Slimest.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Gameplay/Slimer.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
Slimest::Slimest(EntityID const entityID,
                 Vec2 const&    position,
                 float const    orientationDegrees,
                 Rgba8 const&   color,
                 bool const     isVisible,
                 bool const     hasChildWindow)
	: Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
	m_entityID       = entityID;
	m_name           = "Slimest";
	m_physicRadius   = 35.f;
	m_thickness      = 10.f;
	m_cosmeticRadius = m_physicRadius + m_thickness;
	m_health         = 49;
	m_speed          = 50.f;
	m_coinToDrop     = 0;  // coins come from Slime children at the end

	// Boss window
	if (m_hasChildWindow)
	{
		g_windowSubsystem->CreateBossWindow(m_entityID, m_name);

		Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
		Vec2    windowClientPosition  = window->GetClientPosition();
		Vec2    windowClientDimension = window->GetClientDimensions();

		m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("SLIMEST HP=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
		g_widgetSubsystem->AddWidget(m_healthWidget, 200);
	}

	// Center position in boss window coordinate space
	m_position = Vec2(150.f, 150.f);
}

//----------------------------------------------------------------------------------------------------
Slimest::~Slimest()
{
	if (m_hasChildWindow)
	{
		g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
		m_healthWidget->MarkForDestroy();
	}
}

//----------------------------------------------------------------------------------------------------
void Slimest::MarkAsDead()
{
	if (!m_isDead)
	{
		SpawnSlimers();
	}

	Entity::MarkAsDead();
}

//----------------------------------------------------------------------------------------------------
void Slimest::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
	Entity::Update(deltaSeconds);

	SyncWindowWidgetToPosition();
	if (m_isDead) return;

	// Slow chase toward player
	Player* player = g_game->GetPlayer();
	if (player && !player->IsDead())
	{
		EnemyUtils::ChasePlayer(m_position, m_orientationDegrees, player->m_position, m_speed, deltaSeconds);
	}

	// Lob residue blobs
	m_lobTimer += deltaSeconds;
	if (m_lobTimer >= m_lobCooldown)
	{
		m_lobTimer = 0.f;
		LobResidueBlobs();
	}
}

//----------------------------------------------------------------------------------------------------
void Slimest::Render() const
{
	// Render as a large blob/circle shape
	VertexList_PCU verts;
	constexpr int  NUM_SIDES = 16;

	for (int i = 0; i < NUM_SIDES; ++i)
	{
		float const angle0 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i);
		float const angle1 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i + 1);

		// Slight wobble to look blob-like
		float const wobble0 = 1.f + 0.1f * sinf(angle0 * 0.08f);
		float const wobble1 = 1.f + 0.1f * sinf(angle1 * 0.08f);

		Vec2 const vert0 = m_position + Vec2::MakeFromPolarDegrees(angle0, m_physicRadius * wobble0);
		Vec2 const vert1 = m_position + Vec2::MakeFromPolarDegrees(angle1, m_physicRadius * wobble1);

		AddVertsForTriangle2D(verts, m_position, vert0, vert1, m_color);
	}

	g_renderer->SetModelConstants();
	g_renderer->SetBlendMode(eBlendMode::OPAQUE);
	g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
	g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
	g_renderer->SetDepthMode(eDepthMode::DISABLED);
	g_renderer->BindTexture(nullptr);
	g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
	g_renderer->DrawVertexArray(verts);
}

//----------------------------------------------------------------------------------------------------
void Slimest::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
void Slimest::DecreaseHealth(int amount)
{
	Entity::DecreaseHealth(amount);

	if (m_healthWidget)
	{
		m_healthWidget->SetText(Stringf("SLIMEST HP=%d", m_health));
	}
}

//----------------------------------------------------------------------------------------------------
void Slimest::LobResidueBlobs()
{
	Player* player = g_game->GetPlayer();
	if (!player || player->IsDead()) return;

	Vec2 const dirToPlayer = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
	if (dirToPlayer == Vec2::ZERO) return;

	for (int i = 0; i < m_blobCount; ++i)
	{
		float const spreadAngle = (m_blobCount > 1)
			? (-15.f + 30.f / static_cast<float>(m_blobCount - 1) * static_cast<float>(i))
			: 0.f;

		float const baseAngle = dirToPlayer.GetOrientationDegrees() + spreadAngle;
		Vec2 const  blobDir   = Vec2::MakeFromPolarDegrees(baseAngle, 1.f);

		Bullet* blob = new Bullet(
			g_rng->RollRandomIntInRange(100, 10000),
			m_position,
			baseAngle,
			Rgba8(60, 180, 30, 255),  // dark green
			true,
			false
		);
		blob->m_velocity     = blobDir;
		blob->m_name         = "EnemyBullet";
		blob->m_speed        = 160.f;   // slowest blobs
		blob->m_physicRadius = 16.f;    // biggest blobs

		g_game->m_entityList.push_back(blob);
	}
}

//----------------------------------------------------------------------------------------------------
void Slimest::SpawnSlimers()
{
	for (int i = 0; i < m_splitCount; ++i)
	{
		float const scatterAngle = 360.f / static_cast<float>(m_splitCount) * static_cast<float>(i);
		Vec2 const  offset       = Vec2::MakeFromPolarDegrees(scatterAngle, m_physicRadius * 2.f);
		Vec2 const  spawnPos     = m_position + offset;

		Slimer* slimer = new Slimer(
			g_rng->RollRandomIntInRange(100, 10000),
			spawnPos,
			0.f,
			Rgba8(80, 220, 50, 255),  // medium green
			true,
			true
		);

		g_game->m_entityList.push_back(slimer);
	}

	DebuggerPrintf("Slimest (ID=%d): Split into %d Slimers.\n", m_entityID, m_splitCount);
}
