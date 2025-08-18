#include "Game/Asteroid.hpp"
#include "Game/Game.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

Asteroid::Asteroid(Game* game)
	: Entity(game, Vec2(), 0.f)
{
	m_physicsRadius = ASTEROID_PHYSICS_RADIUS;
	m_cosmeticRadius = ASTEROID_COSMETIC_RADIUS;
	m_mainColor = Rgba8(100, 100, 100);

	RandomNumberGenerator rng;

	m_orientationDegrees = rng.RollRandomFloatInRange(0.f, 360.f);
	m_angularVelocity = rng.RollRandomFloatInRange(-200.f, 200.f);
	m_velocity = Vec2::MakeFromPolarDegrees(rng.RollRandomFloatInRange(0.f, 360.f)) * ASTEROID_SPEED;
	m_position = m_game->GetRandomPositionJustOffCamera(ASTEROID_COSMETIC_RADIUS);

	m_maxHealth = ASTEROID_MAX_HEALTH;
	m_health = ASTEROID_MAX_HEALTH;
	InitializeLocalVerts();
}

void Asteroid::Update(float deltaSeconds)
{
	m_position += m_velocity * deltaSeconds;
	m_orientationDegrees += m_angularVelocity * deltaSeconds;

	WrapAround();
}

void Asteroid::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_ASTEROID_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_ASTEROID_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_ASTEROID_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_ASTEROID_VERTS, &tempWorldVerts[0]);

	RenderHealthBar();
}

void Asteroid::InitializeLocalVerts()
{
	RandomNumberGenerator rng;
	float centralAngle = (360.f / NUM_ASTEROID_TRIANGLES);
	float vertexRadius[NUM_ASTEROID_TRIANGLES] = {};
	for (int i = 0; i < NUM_ASTEROID_TRIANGLES; ++i)
	{
		vertexRadius[i] = rng.RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);
	}

	for (int triangleIndex = 0; triangleIndex < NUM_ASTEROID_TRIANGLES; ++triangleIndex)
	{
		m_localVerts[3 * triangleIndex].m_position = Vec3();
		m_localVerts[3 * triangleIndex + 1].m_position = Vec3(vertexRadius[triangleIndex] * CosDegrees(centralAngle * triangleIndex),
															  vertexRadius[triangleIndex] * SinDegrees(centralAngle * triangleIndex), 0.f);
		m_localVerts[3 * triangleIndex + 2].m_position = Vec3(vertexRadius[(triangleIndex + 1) % NUM_ASTEROID_TRIANGLES] * CosDegrees(centralAngle * (triangleIndex + 1)),
															  vertexRadius[(triangleIndex + 1) % NUM_ASTEROID_TRIANGLES] * SinDegrees(centralAngle * (triangleIndex + 1)), 0.f);
	}
	for (int vertIndex = 0; vertIndex < NUM_ASTEROID_VERTS; ++vertIndex)
	{
		m_localVerts[vertIndex].m_color = m_mainColor;
	}
}

void Asteroid::WrapAround()
{
	if (m_position.x < -m_cosmeticRadius)
	{
		m_position.x = WORLD_SIZE_X + m_cosmeticRadius;
	}
	if (m_position.x > (WORLD_SIZE_X + m_cosmeticRadius))
	{
		m_position.x = -m_cosmeticRadius;
	}
	if (m_position.y < -m_cosmeticRadius)
	{
		m_position.y = WORLD_SIZE_Y + m_cosmeticRadius;
	}
	if (m_position.y > (WORLD_SIZE_Y + m_cosmeticRadius))
	{
		m_position.y = -m_cosmeticRadius;
	}

}
