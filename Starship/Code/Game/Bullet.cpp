#include "Game/Bullet.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

Bullet::Bullet(Game* game, Vec2 const& startPosition, float startOrientationDegrees)
	: Entity(game, startPosition, startOrientationDegrees)
{
	m_physicsRadius = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = BULLET_COSMETIC_RADIUS;

	m_velocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees) * BULLET_SPEED;
	m_mainColor = Rgba8(255, 255, 0);

	m_mass = BULLET_MASS;
	InitializeLocalVerts();
}

void Bullet::Update(float deltaSeconds)
{
	m_ageInSeconds += deltaSeconds;

	m_position += m_velocity * deltaSeconds;
	if (m_ageInSeconds > 2.0f)
	{
		Die();
	}
}

void Bullet::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_BULLET_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_BULLET_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_BULLET_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_BULLET_VERTS, &tempWorldVerts[0]);
}

void Bullet::InitializeLocalVerts()
{
	// Nose
	m_localVerts[0].m_position = Vec3(.5f, 0.f, 0.f);
	m_localVerts[1].m_position = Vec3(0.f, .5f, 0.f);
	m_localVerts[2].m_position = Vec3(0.f, -.5f, 0.f);

	m_localVerts[0].m_color = Rgba8(255, 255, 0);
	m_localVerts[1].m_color = Rgba8(255, 255, 0);
	m_localVerts[2].m_color = Rgba8(255, 255, 0);

	// Tail
	m_localVerts[3].m_position = Vec3(0.f, .5f, 0.f);
	m_localVerts[4].m_position = Vec3(-2.f, 0.f, 0.f);
	m_localVerts[5].m_position = Vec3(0.f, -.5f, 0.f);

	m_localVerts[3].m_color = Rgba8(255, 0, 0);
	m_localVerts[4].m_color = Rgba8(255, 0, 0, 0);
	m_localVerts[5].m_color = Rgba8(255, 0, 0);
}
