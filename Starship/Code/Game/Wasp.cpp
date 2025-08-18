#include "Game/Wasp.hpp"
#include "Game/Game.hpp"

#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

Wasp::Wasp(Game* game)
	:Entity(game, Vec2(), 0.f)
{
	m_position = m_game->GetRandomPositionJustOffCamera(WASP_COSMETIC_RADIUS);
	m_physicsRadius = WASP_PHYSICS_RADIUS;
	m_cosmeticRadius = WASP_COSMETIC_RADIUS;
	m_mainColor = Rgba8(180, 180, 0);

	m_maxHealth = WASP_MAX_HEALTH;
	m_health	= WASP_MAX_HEALTH;
	m_mass		= WASP_MASS;
	InitializeLocalVerts();
}

void Wasp::Update(float deltaSeconds)
{
	if (m_game->IsPlayerShipAlive()) {
		float moveDirection = (m_game->GetPlayerShipPosition() - m_position).GetOrientationDegrees();
		m_orientationDegrees = moveDirection;
	}
	else
	{
		// IDLE
		Vec2 targetPosition = (m_position - Vec2(WORLD_CENTER_X, WORLD_CENTER_Y)).GetNormalized().GetRotatedDegrees(-30) * BEETLE_IDLE_RADIUS + Vec2(WORLD_CENTER_X, WORLD_CENTER_Y);
		float moveDirection = (targetPosition - m_position).GetOrientationDegrees();
		m_orientationDegrees = moveDirection;
	}
	Vec2 deltaVelocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees) * WASP_ACCELERATION * deltaSeconds;
	AddVelocity(WASP_MAX_SPEED, deltaVelocity);

	m_position += m_velocity * deltaSeconds;
}

void Wasp::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_WASP_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_WASP_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_WASP_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_WASP_VERTS, &tempWorldVerts[0]);
	RenderHealthBar();
}

void Wasp::InitializeLocalVerts()
{
	m_localVerts[0].m_position = Vec3(0.f, -2.f, 0.f);
	m_localVerts[1].m_position = Vec3(2.f, 0.f, 0.f);
	m_localVerts[2].m_position = Vec3(0.f, 2.f, 0.f);

	m_localVerts[3].m_position = Vec3(0.f, -1.f, 0.f);
	m_localVerts[4].m_position = Vec3(0.f, 1.f, 0.f);
	m_localVerts[5].m_position = Vec3(-2.f, -0.f, 0.f);

	for (int vertIndex = 0; vertIndex < NUM_WASP_VERTS; ++vertIndex)
	{
		m_localVerts[vertIndex].m_color = m_mainColor;
	}
}
