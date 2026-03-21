//----------------------------------------------------------------------------------------------------
// Spiker.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Spiker.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"
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
Spiker::Spiker(EntityID const entityID,
               Vec2 const&    position,
               float const    orientationDegrees,
               Rgba8 const&   color,
               bool const     isVisible,
               bool const     hasChildWindow)
	: Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
	m_entityID       = entityID;
	m_name           = "Spiker";
	m_physicRadius   = 35.f;
	m_thickness      = 10.f;
	m_cosmeticRadius = m_physicRadius + m_thickness;
	m_health         = m_maxHealth;
	m_speed          = 0.f;  // immobile
	m_coinToDrop     = 32;

	// Boss window
	if (m_hasChildWindow)
	{
		g_windowSubsystem->CreateBossWindow(m_entityID, m_name);

		Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
		Vec2    windowClientPosition  = window->GetClientPosition();
		Vec2    windowClientDimension = window->GetClientDimensions();

		m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("SPIKER HP=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
		g_widgetSubsystem->AddWidget(m_healthWidget, 200);
	}

	// Center position in boss window coordinate space
	m_position = Vec2(150.f, 150.f);
}

//----------------------------------------------------------------------------------------------------
Spiker::~Spiker()
{
	if (m_hasChildWindow)
	{
		g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
		m_healthWidget->MarkForDestroy();
	}
}

//----------------------------------------------------------------------------------------------------
void Spiker::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
	Entity::Update(deltaSeconds);

	SyncWindowWidgetToPosition();
	if (m_isDead) return;

	Player* player = g_game->GetPlayer();
	if (!player || player->IsDead()) return;

	// Radial bullet wave attack (every 2s)
	m_radialAttackTimer += deltaSeconds;
	if (m_radialAttackTimer >= m_radialAttackCooldown)
	{
		m_radialAttackTimer = 0.0f;
		FireRadialBulletWave();
	}

	// Laser attack (every 8s with 1.5s telegraph)
	m_laserTimer += deltaSeconds;
	if (!m_isLaserCharging && m_laserTimer >= m_laserCooldown)
	{
		// Lock aim direction at the moment charging begins
		Vec2 const dirToPlayer = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
		if (dirToPlayer != Vec2::ZERO)
		{
			m_laserAimDegrees = dirToPlayer.GetOrientationDegrees();
		}
		m_isLaserCharging = true;
		m_laserChargeTimer = 0.0f;
	}

	if (m_isLaserCharging)
	{
		m_laserChargeTimer += deltaSeconds;
		if (m_laserChargeTimer >= m_laserTelegraphTime)
		{
			FireLaser();
			m_isLaserCharging = false;
			m_laserTimer = 0.0f;
		}
	}
}

//----------------------------------------------------------------------------------------------------
void Spiker::Render() const
{
	// Render Spiker as an 8-pointed star shape
	VertexList_PCU verts;
	constexpr int  NUM_POINTS    = 8;
	float const    outerRadius   = m_physicRadius;
	float const    innerRadius   = m_physicRadius * 0.5f;

	for (int i = 0; i < NUM_POINTS * 2; ++i)
	{
		float const angle0  = 360.f / static_cast<float>(NUM_POINTS * 2) * static_cast<float>(i);
		float const angle1  = 360.f / static_cast<float>(NUM_POINTS * 2) * static_cast<float>(i + 1);
		float const radius0 = (i % 2 == 0) ? outerRadius : innerRadius;
		float const radius1 = ((i + 1) % 2 == 0) ? outerRadius : innerRadius;

		Vec2 const vert0 = m_position + Vec2::MakeFromPolarDegrees(angle0, radius0);
		Vec2 const vert1 = m_position + Vec2::MakeFromPolarDegrees(angle1, radius1);

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

	// Render laser telegraph if charging
	if (m_isLaserCharging)
	{
		RenderLaserTelegraph();
	}
}

//----------------------------------------------------------------------------------------------------
void Spiker::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
void Spiker::DecreaseHealth(int amount)
{
	Entity::DecreaseHealth(amount);

	if (m_healthWidget)
	{
		m_healthWidget->SetText(Stringf("SPIKER HP=%d", m_health));
	}

	CheckTeleportThreshold();
}

//----------------------------------------------------------------------------------------------------
// FireRadialBulletWave - Fires 8 bullets evenly spaced in a circle
//----------------------------------------------------------------------------------------------------
void Spiker::FireRadialBulletWave()
{
	constexpr int   NUM_BULLETS   = 8;
	constexpr float ANGLE_STEP    = 360.f / static_cast<float>(NUM_BULLETS);

	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		float const angle     = ANGLE_STEP * static_cast<float>(i);
		Vec2 const  direction = Vec2::MakeFromPolarDegrees(angle, 1.f);

		Bullet* bullet = new Bullet(
			g_rng->RollRandomIntInRange(100, 1000),
			m_position,
			angle,
			Rgba8(255, 50, 50, 255),
			true,
			false
		);
		bullet->m_velocity = direction;
		bullet->m_name     = "EnemyBullet";

		g_game->m_entityList.push_back(bullet);
	}
}

//----------------------------------------------------------------------------------------------------
// FireLaser - Fires a line of fast bullets along the aimed direction
//----------------------------------------------------------------------------------------------------
void Spiker::FireLaser()
{
	Vec2 const laserDir = Vec2::MakeFromPolarDegrees(m_laserAimDegrees, 1.f);

	// Fire a burst of 5 fast bullets along the laser line
	constexpr int NUM_LASER_BULLETS = 5;
	for (int i = 0; i < NUM_LASER_BULLETS; ++i)
	{
		float const offset = static_cast<float>(i) * 15.f;
		Vec2 const  spawnPos = m_position + laserDir * offset;

		Bullet* bullet = new Bullet(
			g_rng->RollRandomIntInRange(100, 1000),
			spawnPos,
			m_laserAimDegrees,
			Rgba8(255, 255, 0, 255),  // yellow laser bullets
			true,
			false
		);
		bullet->m_velocity = laserDir;
		bullet->m_name     = "EnemyBullet";

		g_game->m_entityList.push_back(bullet);
	}
}

//----------------------------------------------------------------------------------------------------
// RenderLaserTelegraph - Draws a dotted line showing where the laser will fire
//----------------------------------------------------------------------------------------------------
void Spiker::RenderLaserTelegraph() const
{
	Vec2 const laserDir = Vec2::MakeFromPolarDegrees(m_laserAimDegrees, 1.f);

	// Draw dotted line extending well beyond the screen
	constexpr float DASH_LENGTH   = 20.f;
	constexpr float GAP_LENGTH    = 15.f;
	constexpr float SEGMENT_TOTAL = DASH_LENGTH + GAP_LENGTH;
	constexpr float MAX_REACH     = 3000.f;  // covers any reasonable screen size
	int const       NUM_DASHES    = static_cast<int>(MAX_REACH / SEGMENT_TOTAL) + 1;

	// Pulsing alpha based on charge progress
	float const chargeRatio = m_laserChargeTimer / m_laserTelegraphTime;
	uint8_t const alpha     = static_cast<uint8_t>(128.f + 127.f * chargeRatio);
	Rgba8 const telegraphColor(255, 255, 0, alpha);

	VertexList_PCU verts;
	for (int i = 0; i < NUM_DASHES; ++i)
	{
		float const startDist = static_cast<float>(i) * SEGMENT_TOTAL;
		float const endDist   = startDist + DASH_LENGTH;

		Vec2 const dashStart = m_position + laserDir * startDist;
		Vec2 const dashEnd   = m_position + laserDir * endDist;

		AddVertsForLineSegment2D(verts, dashStart, dashEnd, 2.f, false, telegraphColor);
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
// CheckTeleportThreshold - Teleports the boss window when HP crosses 25% thresholds
//----------------------------------------------------------------------------------------------------
void Spiker::CheckTeleportThreshold()
{
	// Calculate which 25% threshold we're at (3 = 75%, 2 = 50%, 1 = 25%)
	int const currentThreshold = 4 - (m_health * 4 / m_maxHealth);

	if (currentThreshold > m_lastTeleportThreshold && currentThreshold < 4)
	{
		m_lastTeleportThreshold = currentThreshold;

		// Teleport window to a random position on screen
		int const screenWidth  = GetSystemMetrics(SM_CXSCREEN);
		int const screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int const windowSize   = 300;  // boss window default size
		int const margin       = 50;

		int const newX = g_rng->RollRandomIntInRange(margin, screenWidth - windowSize - margin);
		int const newY = g_rng->RollRandomIntInRange(margin, screenHeight - windowSize - margin);

		WindowID const windowID = g_windowSubsystem->FindWindowIDByEntityID(m_entityID);
		if (windowID != INVALID_WINDOW_ID)
		{
			g_windowSubsystem->AnimateWindowPosition(windowID, Vec2(static_cast<float>(newX), static_cast<float>(newY)), 0.15f);
			DebuggerPrintf("Spiker: Teleported window to (%d, %d) at %d%% HP threshold.\n", newX, newY, (4 - currentThreshold) * 25);
		}
	}
}
