//----------------------------------------------------------------------------------------------------
// Slime.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Slime.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/Residue.hpp"
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
Slime::Slime(EntityID const entityID,
             Vec2 const&    position,
             float const    orientationDegrees,
             Rgba8 const&   color,
             bool const     isVisible,
             bool const     hasChildWindow)
	: Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
	m_entityID       = entityID;
	m_name           = "Slime";
	m_physicRadius   = 15.f;
	m_thickness      = 4.f;
	m_cosmeticRadius = m_physicRadius + m_thickness;
	m_health         = 20;
	m_speed          = 100.f;
	m_coinToDrop     = 6;

	if (m_hasChildWindow)
	{
		g_windowSubsystem->CreateBossWindow(m_entityID, m_name, 150, 150);

		Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
		Vec2    windowClientPosition  = window->GetClientPosition();
		Vec2    windowClientDimension = window->GetClientDimensions();

		m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("SLIME HP=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
		g_widgetSubsystem->AddWidget(m_healthWidget, 200);
	}
}

//----------------------------------------------------------------------------------------------------
Slime::~Slime()
{
	if (m_hasChildWindow)
	{
		g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
		m_healthWidget->MarkForDestroy();
	}
}

//----------------------------------------------------------------------------------------------------
void Slime::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
	Entity::Update(deltaSeconds);

	SyncWindowWidgetToPosition();
	if (m_isDead) return;

	// Chase player
	Player* player = g_game->GetPlayer();
	if (player && !player->IsDead())
	{
		EnemyUtils::ChasePlayer(m_position, m_orientationDegrees, player->m_position, m_speed, deltaSeconds);
	}

	// Residue trail
	m_trailTimer += deltaSeconds;
	if (m_trailTimer >= m_trailCooldown)
	{
		m_trailTimer = 0.f;
		SpawnResidue();
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
void Slime::Render() const
{
	// Render as a blob/circle shape
	VertexList_PCU verts;
	constexpr int  NUM_SIDES = 12;

	for (int i = 0; i < NUM_SIDES; ++i)
	{
		float const angle0 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i);
		float const angle1 = 360.f / static_cast<float>(NUM_SIDES) * static_cast<float>(i + 1);

		Vec2 const vert0 = m_position + Vec2::MakeFromPolarDegrees(angle0, m_physicRadius);
		Vec2 const vert1 = m_position + Vec2::MakeFromPolarDegrees(angle1, m_physicRadius);

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
void Slime::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
void Slime::DecreaseHealth(int amount)
{
	Entity::DecreaseHealth(amount);

	// Widget text removed - no text rendering on enemy windows
}

//----------------------------------------------------------------------------------------------------
void Slime::LobResidueBlobs()
{
	Player* player = g_game->GetPlayer();
	if (!player || player->IsDead()) return;

	Vec2 const dirToPlayer = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
	if (dirToPlayer == Vec2::ZERO) return;

	for (int i = 0; i < m_blobCount; ++i)
	{
		// Add slight spread for multiple blobs
		float const spreadAngle = (m_blobCount > 1)
			? g_rng->RollRandomFloatInRange(-15.f, 15.f)
			: 0.f;

		float const baseAngle = dirToPlayer.GetOrientationDegrees() + spreadAngle;
		Vec2 const  blobDir   = Vec2::MakeFromPolarDegrees(baseAngle, 1.f);

		Bullet* blob = new Bullet(
			Game::AllocateEntityID(),
			m_position,
			baseAngle,
			Rgba8(100, 220, 50, 255),  // sickly green
			true,
			false
		);
		blob->m_velocity     = blobDir;
		blob->m_name         = "EnemyBullet";
		blob->m_speed        = 200.f;   // slower than normal bullets
		blob->m_physicRadius = 12.f;    // bigger than normal bullets

		g_game->m_entityList.push_back(blob);
	}
}

//----------------------------------------------------------------------------------------------------
void Slime::SpawnResidue()
{
	Residue* residue = new Residue(
		g_rng->RollRandomIntInRange(100, 10000),
		m_position,
		Rgba8(80, 180, 40, 180)  // light green puddle
	);

	g_game->m_entityList.push_back(residue);
}
