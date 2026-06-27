//----------------------------------------------------------------------------------------------------
// Wyrm.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Wyrm.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Bullet.hpp"
#include "Game/Gameplay/EnemyUtils.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Player.hpp"
#include "Game/Gameplay/WyrmSegment.hpp"
#include "Game/Subsystem/Widget/ButtonWidget.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Resource/ResourceSubsystem.hpp"
#include "Engine/Widget/WidgetSubsystem.hpp"


//----------------------------------------------------------------------------------------------------
Wyrm::Wyrm(EntityID const entityID,
           Vec2 const&    position,
           float const    orientationDegrees,
           Rgba8 const&   color,
           bool const     isVisible,
           bool const     hasChildWindow)
	: Entity(position, orientationDegrees, color, isVisible, hasChildWindow)
{
	m_entityID       = entityID;
	m_name           = "Wyrm";
	m_physicRadius   = 25.f;
	m_thickness      = 8.f;
	m_cosmeticRadius = m_physicRadius + m_thickness;
	m_health         = 10;
	m_speed          = m_headSpeed;
	m_coinToDrop     = 8;

	// Initialize movement angle toward a random direction
	m_moveAngle = g_rng->RollRandomFloatInRange(0.f, 360.f);

	// Boss window for head
	if (m_hasChildWindow)
	{
		g_windowSubsystem->CreateBossWindow(m_entityID, m_name);

		Window* window                = g_windowSubsystem->GetWindow(g_windowSubsystem->FindWindowIDByEntityID(m_entityID));
		Vec2    windowClientPosition  = window->GetClientPosition();
		Vec2    windowClientDimension = window->GetClientDimensions();

		m_healthWidget = g_widgetSubsystem->CreateWidget<ButtonWidget>(g_widgetSubsystem, Stringf("WYRM HEAD HP=%d", m_health), (int)windowClientPosition.x, (int)windowClientPosition.y, (int)windowClientDimension.x, (int)windowClientDimension.y, m_color);
		g_widgetSubsystem->AddWidget(m_healthWidget, 200);
	}

	// Center position in boss window coordinate space
	m_position = Vec2(150.f, 150.f);

	// Spawn segments behind the head
	SpawnSegments();
}

//----------------------------------------------------------------------------------------------------
Wyrm::~Wyrm()
{
	if (m_hasChildWindow)
	{
		g_windowSubsystem->RemoveEntityFromMappings(m_entityID);
		m_healthWidget->MarkForDestroy();
	}

	// Chain re-linking is handled by Wyrm::Update() while pointers are still valid.
	// Destructors must NOT access m_segments — they may already be freed
	// during the deferred deletion pass.
	m_segments.clear();
}

//----------------------------------------------------------------------------------------------------
void Wyrm::Update(float const deltaSeconds)
{
	if (g_game->GetCurrentGameState() == eGameState::SHOP || g_game->GetCurrentGameState() == eGameState::ATTRACT) return;
	Entity::Update(deltaSeconds);

	SyncWindowWidgetToPosition();
	if (m_isDead) return;

	Player* player = g_game->GetPlayer();
	if (!player || player->IsDead()) return;

	// Smooth turning toward the player
	Vec2 const dirToPlayer   = EnemyUtils::GetDirectionToPlayer(m_position, player->m_position);
	if (dirToPlayer != Vec2::ZERO)
	{
		float const targetAngle   = dirToPlayer.GetOrientationDegrees();
		float const angleDiff     = GetShortestAngularDispDegrees(m_moveAngle, targetAngle);
		float const maxTurnThisFrame = m_turnSpeed * deltaSeconds;

		if (fabsf(angleDiff) <= maxTurnThisFrame)
		{
			m_moveAngle = targetAngle;
		}
		else
		{
			m_moveAngle += (angleDiff > 0.f ? 1.f : -1.f) * maxTurnThisFrame;
		}
	}

	// Move head in current direction
	Vec2 const moveDir = Vec2::MakeFromPolarDegrees(m_moveAngle, 1.f);
	m_position += moveDir * m_headSpeed * deltaSeconds;
	m_orientationDegrees = m_moveAngle;

	// Sequential fire cooldown
	m_fireTimer += deltaSeconds;
	if (m_fireTimer >= m_fireCooldown)
	{
		m_fireTimer = 0.f;
		InitiateSequentialFire();
	}

	// Validate segment pointers are still in the entity list
	for (int i = static_cast<int>(m_segments.size()) - 1; i >= 0; --i)
	{
		bool found = false;
		for (Entity* e : g_game->m_entityList)
		{
			if (e == m_segments[i])
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			// Dangling pointer detected — remove from tracking
			m_segments.erase(m_segments.begin() + i);
		}
	}

	// Clean up dead segments from our tracking list
	for (int i = static_cast<int>(m_segments.size()) - 1; i >= 0; --i)
	{
		if (m_segments[i]->IsDead())
		{
			// Re-link the chain: connect predecessor to successor
			WyrmSegment* deadSeg  = m_segments[i];
			Entity*      leader   = deadSeg->GetLeader();
			WyrmSegment* follower = deadSeg->GetFollower();

			// Walk up the chain to find a living leader (skip dead leaders)
			while (leader && leader->IsDead())
			{
				// If leader is a WyrmSegment, follow its leader pointer
				WyrmSegment* leaderSeg = nullptr;
				for (WyrmSegment* seg : m_segments)
				{
					if (seg == leader)
					{
						leaderSeg = seg;
						break;
					}
				}
				if (leaderSeg)
					leader = leaderSeg->GetLeader();
				else
					break; // leader is the Wyrm head (or unknown), stop
			}

			// If the leader is also dead (Wyrm head died), set to nullptr
			if (leader && leader->IsDead())
				leader = nullptr;

			// If there's a follower, its new leader is the nearest living ancestor
			if (follower)
			{
				follower->SetLeader(leader);
			}

			// If the leader is a living segment, update its follower pointer
			if (leader)
			{
				for (WyrmSegment* seg : m_segments)
				{
					if (seg == leader)
					{
						seg->SetFollower(follower);
						break;
					}
				}
			}

			m_segments.erase(m_segments.begin() + i);
		}
	}
}

//----------------------------------------------------------------------------------------------------
void Wyrm::Render() const
{
	// Render head as a diamond/rhombus shape to distinguish from segments
	VertexList_PCU verts;

	Vec2 const forward = Vec2::MakeFromPolarDegrees(m_orientationDegrees, m_physicRadius * 1.3f);
	Vec2 const right   = Vec2::MakeFromPolarDegrees(m_orientationDegrees - 90.f, m_physicRadius * 0.7f);

	Vec2 const tip   = m_position + forward;
	Vec2 const left  = m_position + right;
	Vec2 const rght  = m_position - right;
	Vec2 const tail  = m_position - forward * 0.5f;

	AddVertsForTriangle2D(verts, tip, left, rght, m_color);
	AddVertsForTriangle2D(verts, left, tail, rght, Rgba8(m_color.r / 2, m_color.g / 2, m_color.b / 2, m_color.a));

	g_renderer->SetModelConstants();
	g_renderer->SetBlendMode(eBlendMode::OPAQUE);
	g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
	g_renderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
	g_renderer->SetDepthMode(eDepthMode::DISABLED);
	g_renderer->BindTexture(nullptr);
	g_renderer->BindShader(g_resourceSubsystem->CreateOrGetShaderFromFile("Data/Shaders/Default"));
	g_renderer->DrawVertexArray(verts);

	// Draw connection line to first segment
	if (!m_segments.empty() && !m_segments[0]->IsDead())
	{
		VertexList_PCU lineVerts;
		Rgba8 const lineColor(m_color.r, m_color.g, m_color.b, 128);
		AddVertsForLineSegment2D(lineVerts, m_position, m_segments[0]->m_position, 3.f, false, lineColor);

		g_renderer->SetBlendMode(eBlendMode::ALPHA);
		g_renderer->DrawVertexArray(lineVerts);
	}
}

//----------------------------------------------------------------------------------------------------
void Wyrm::UpdateFromInput(float deltaSeconds)
{
	UNUSED(deltaSeconds)
}

//----------------------------------------------------------------------------------------------------
void Wyrm::DecreaseHealth(int amount)
{
	Entity::DecreaseHealth(amount);

	// Widget text removed - no text rendering on enemy windows
}

//----------------------------------------------------------------------------------------------------
void Wyrm::SpawnSegments()
{
	// Spawn segments behind the head in a line
	Vec2 const backDir = Vec2::MakeFromPolarDegrees(m_moveAngle + 180.f, 1.f);

	Entity* previousEntity = this;

	for (int i = 0; i < NUM_SEGMENTS; ++i)
	{
		Vec2 const segmentPos = m_position + backDir * (65.f * static_cast<float>(i + 1));

		// Gradually shift color from bright to darker along the chain
		uint8_t const greenShift = static_cast<uint8_t>(255 - (i * 25));

		WyrmSegment* segment = new WyrmSegment(
			Game::AllocateEntityID(),
			segmentPos,
			m_orientationDegrees,
			Rgba8(0, greenShift, 100, 255),
			true,
			true,  // each segment has own window
			i
		);

		// Link chain
		segment->SetLeader(previousEntity);
		if (!m_segments.empty())
		{
			m_segments.back()->SetFollower(segment);
		}

		m_segments.push_back(segment);
		g_game->m_entityList.push_back(segment);

		previousEntity = segment;
	}
}

//----------------------------------------------------------------------------------------------------
void Wyrm::InitiateSequentialFire()
{
	// The head fires first, then triggers the chain
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

	// Trigger first segment to start the sequential fire chain
	if (!m_segments.empty() && !m_segments[0]->IsDead())
	{
		m_segments[0]->TriggerFire();
	}
}
