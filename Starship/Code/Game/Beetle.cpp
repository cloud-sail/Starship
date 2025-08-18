#include "Game/Beetle.hpp"
#include "Game/Game.hpp"

#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"


Beetle::Beetle(Game* game)
	: Entity(game, Vec2(), 0.f)
{
	m_position = m_game->GetRandomPositionJustOffCamera(BEETLE_COSMETIC_RADIUS);
	m_physicsRadius = BEETLE_PHYSICS_RADIUS;
	m_cosmeticRadius = BEETLE_COSMETIC_RADIUS;
	m_mainColor = Rgba8(0, 150, 0);

	m_maxHealth = BEETLE_MAX_HEALTH;
	m_health	= BEETLE_MAX_HEALTH;
	m_mass		= BEETLE_MASS;
	InitializeLocalVerts();
}

void Beetle::Update(float deltaSeconds)
{
	if (m_game->IsPlayerShipAlive()) {
		float moveDirection = (m_game->GetPlayerShipPosition() - m_position).GetOrientationDegrees();
		m_orientationDegrees = moveDirection;
	}
	else
	{
		// IDLE
		Vec2 targetPosition = (m_position - Vec2(WORLD_CENTER_X, WORLD_CENTER_Y)).GetNormalized().GetRotatedDegrees(30) * BEETLE_IDLE_RADIUS + Vec2(WORLD_CENTER_X, WORLD_CENTER_Y);
		float moveDirection = (targetPosition - m_position).GetOrientationDegrees();
		m_orientationDegrees = moveDirection;
	}
	Vec2 deltaVelocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees) * BEETLE_ACCELERATION * deltaSeconds;
	AddVelocity(BEETLE_MAX_SPEED, deltaVelocity);

	//m_velocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees) * BEETLE_SPEED;
	m_position += m_velocity * deltaSeconds;
}

void Beetle::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_BEETLE_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_BEETLE_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_BEETLE_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_BEETLE_VERTS, &tempWorldVerts[0]);

	RenderHealthBar();
}

void Beetle::InitializeLocalVerts()
{
	float cos30 = 0.866f;
	float sin30 = 0.5f;

	Vec3 vertexs[NUM_BEETLE_TRIANGLES] = {};
	vertexs[0] = Vec3(cos30, sin30, 0.f) * m_cosmeticRadius;
	vertexs[1] = Vec3(sin30, cos30, 0.f) * m_cosmeticRadius;
	vertexs[2] = Vec3(-cos30, sin30, 0.f) * m_cosmeticRadius;
	vertexs[3] = Vec3(-cos30, -sin30, 0.f) * m_cosmeticRadius;
	vertexs[4] = Vec3(sin30, -cos30, 0.f) * m_cosmeticRadius;
	vertexs[5] = Vec3(cos30, -sin30, 0.f) * m_cosmeticRadius;

	for (int triangleIndex = 0; triangleIndex < NUM_BEETLE_TRIANGLES; ++triangleIndex)
	{
		m_localVerts[3 * triangleIndex].m_position = Vec3();
		m_localVerts[3 * triangleIndex + 1].m_position = vertexs[triangleIndex];
		m_localVerts[3 * triangleIndex + 2].m_position = vertexs[(triangleIndex + 1) % NUM_BEETLE_TRIANGLES];
	}

	for (int vertIndex = 0; vertIndex < NUM_BEETLE_VERTS; ++vertIndex)
	{
		m_localVerts[vertIndex].m_color = m_mainColor;
	}
}
