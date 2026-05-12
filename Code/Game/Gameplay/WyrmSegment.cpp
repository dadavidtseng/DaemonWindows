//----------------------------------------------------------------------------------------------------
// WyrmSegment.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/WyrmSegment.hpp"
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
WyrmSegment::WyrmSegment(EntityID const entityID,
                         Vec2 const&    position,
                         float const    orientationDegrees,
                         Rgba8 const&   color,
                         bool const     isVisible,
                         bool const     hasChildWindow,
                         int const      segmentIndex)
	: Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
	, m_segmentIndex(segmentIndex)
{
	m_entityID       = entityID;
	m_name           = "WyrmSegment";
	m_physicRadius   = 20.f;
	m_thickness      = 6.f;
	m_cosmeticRadius = m_physicRadius + m_thickness;
	m_health         = 10;
	m_speed          = 200.f;
	m_coinToDrop     = 8;

	// Boss window for each segment
	if (m_hasChildWindow)
	{
		g_windowSubsystem->CreateBossWindow(m_entityID, Stringf("Wyrm[%d]", m_segmentIndex), 200, 200);

		Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
		Vec2    windowClientPosition  = window->GetClientPosition();
		Vec2    windowClientDimension = window->GetClientDimensions();

		m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("WYRM[%d] HP=%d", m_segmentIndex, m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
		g_widgetSubsystem->AddWidget(m_healthWidget, 200);
	}
}

//----------------------------------------------------------------------------------------------------
WyrmSegment::~WyrmSegment()
{
	if (m_hasChildWindow)
	{
		g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
		m_healthWidget->MarkForDestroy();
	}

	// Chain re-linking is handled by Wyrm::Update() while pointers are still valid.
	// Destructors must NOT access m_leader or m_follower — they may already be freed
	// during the deferred deletion pass.
	m_leader   = nullptr;
	m_follower = nullptr;
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
	Entity::Update(deltaSeconds);

	SyncWindowWidgetToPosition();
	if (m_isDead) return;

	// Validate leader pointer is still in the entity list (debug crash detection)
	if (m_leader)
	{
		bool leaderFound = false;
		for (Entity* e : g_game->m_entityList)
		{
			if (e == m_leader)
			{
				leaderFound = true;
				break;
			}
		}
		if (!leaderFound)
		{
			// Leader pointer is dangling! Null it out to prevent crash
			m_leader = nullptr;
		}
	}

	// Validate follower pointer similarly
	if (m_follower)
	{
		bool followerFound = false;
		for (Entity* e : g_game->m_entityList)
		{
			if (e == m_follower)
			{
				followerFound = true;
				break;
			}
		}
		if (!followerFound)
		{
			m_follower = nullptr;
		}
	}

	// Follow the leader (head or previous segment) maintaining chain distance
	if (m_leader && !m_leader->IsDead())
	{
		Vec2 const toLeader     = m_leader->m_position - m_position;
		float const distToLeader = toLeader.GetLength();

		if (distToLeader > m_followDistance)
		{
			Vec2 const direction = toLeader.GetNormalized();
			m_position += direction * m_speed * deltaSeconds;
			m_orientationDegrees = direction.GetOrientationDegrees();
		}

		if (deltaSeconds > 0.f)
		{
			Vec2 const previousPosition = m_position;
			// velocity is approximation for collision purposes
			m_velocity = (m_position - previousPosition) / deltaSeconds;
		}
	}
	else
	{
		// Leader is dead — this segment becomes an independent chaser
		Player* player = g_game->GetPlayer();
		if (player && !player->IsDead())
		{
			EnemyUtils::ChasePlayer(m_position, m_orientationDegrees, player->m_position, m_speed * 0.6f, deltaSeconds);
		}
	}

	// Sequential fire delay processing
	if (m_fireTriggered)
	{
		m_fireDelayTimer += deltaSeconds;
		if (m_fireDelayTimer >= m_fireDelay)
		{
			FireBulletAtPlayer();
			m_fireTriggered  = false;
			m_fireDelayTimer = 0.f;

			// Propagate fire to follower
			if (m_follower && !m_follower->IsDead())
			{
				m_follower->TriggerFire();
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::Render() const
{
	// Render as a circle (body segment)
	VertexList_PCU verts;
	constexpr int  NUM_SIDES = 16;

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

	// Draw connection line to leader
	if (m_leader && !m_leader->IsDead())
	{
		VertexList_PCU lineVerts;
		Rgba8 const lineColor(m_color.r, m_color.g, m_color.b, 128);
		AddVertsForLineSegment2D(lineVerts, m_position, m_leader->m_position, 3.f, false, lineColor);

		g_renderer->SetBlendMode(eBlendMode::ALPHA);
		g_renderer->DrawVertexArray(lineVerts);
	}
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::DecreaseHealth(int amount)
{
	Entity::DecreaseHealth(amount);

	// Widget text removed - no text rendering on enemy windows
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::TriggerFire()
{
	m_fireTriggered  = true;
	m_fireDelayTimer = 0.f;
	m_fireDelay      = 0.3f;  // 0.3s delay between each segment firing
}

//----------------------------------------------------------------------------------------------------
void WyrmSegment::FireBulletAtPlayer()
{
	Player* player = g_game->GetPlayer();
	if (!player || player->IsDead()) return;

	Vec2 const dirToPlayer = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
	if (dirToPlayer == Vec2::ZERO) return;

	float const angle = dirToPlayer.GetOrientationDegrees();

	Bullet* bullet = new Bullet(
		Game::AllocateEntityID(),
		m_position,
		angle,
		Rgba8(0, 255, 100, 255),  // green bullets
		true,
		false
	);
	bullet->m_velocity = dirToPlayer;
	bullet->m_name     = "EnemyBullet";

	g_game->m_entityList.push_back(bullet);
}
