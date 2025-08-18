#include "Game/Debris.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/MathUtils.hpp"

Debris::Debris(Game* game, Vec2 const& position, Vec2 const& velocity, Rgba8 const& color, float averageRadius)
	: Entity(game, position, 0.f)
{
	RandomNumberGenerator rng;
	m_velocity = velocity;
	m_orientationDegrees = rng.RollRandomFloatInRange(0.f, 360.f);
	m_angularVelocity = rng.RollRandomFloatInRange(-400.f, 400.f);
	m_outerRadius = 1.5f * averageRadius;
	m_innerRadius = 0.5f * averageRadius;
	m_physicsRadius = 0.f;
	m_cosmeticRadius = m_outerRadius;
	m_mainColor = color;

	InitializeLocalVerts();
}

void Debris::Update(float deltaSeconds)
{
	m_ageInSeconds += deltaSeconds;

	m_position += m_velocity * deltaSeconds;
	m_orientationDegrees += m_angularVelocity * deltaSeconds;

	if (m_ageInSeconds > 2.0f)
	{
		Die();
	}
}

void Debris::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_DEBRIS_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_DEBRIS_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
		tempWorldVerts[vertIndex].m_color.a = static_cast<unsigned char>(RangeMap(m_ageInSeconds, 0.f, 2.f, 127.f, 0.f));
	}
	TransformVertexArrayXY3D(NUM_DEBRIS_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_DEBRIS_VERTS, &tempWorldVerts[0]);
}

void Debris::InitializeLocalVerts()
{
	RandomNumberGenerator rng;
	float centralAngle = (360.f / NUM_DEBRIS_TRIANGLES);
	float vertexRadius[NUM_DEBRIS_TRIANGLES] = {};
	for (int i = 0; i < NUM_DEBRIS_TRIANGLES; ++i)
	{
		vertexRadius[i] = rng.RollRandomFloatInRange(m_innerRadius, m_outerRadius);
	}

	for (int triangleIndex = 0; triangleIndex < NUM_DEBRIS_TRIANGLES; ++triangleIndex)
	{
		m_localVerts[3 * triangleIndex].m_position = Vec3();
		m_localVerts[3 * triangleIndex + 1].m_position = Vec3(vertexRadius[triangleIndex] * CosDegrees(centralAngle * triangleIndex),
			vertexRadius[triangleIndex] * SinDegrees(centralAngle * triangleIndex), 0.f);
		m_localVerts[3 * triangleIndex + 2].m_position = Vec3(vertexRadius[(triangleIndex + 1) % NUM_DEBRIS_TRIANGLES] * CosDegrees(centralAngle * (triangleIndex + 1)),
			vertexRadius[(triangleIndex + 1) % NUM_DEBRIS_TRIANGLES] * SinDegrees(centralAngle * (triangleIndex + 1)), 0.f);
	}
	for (int vertIndex = 0; vertIndex < NUM_DEBRIS_VERTS; ++vertIndex)
	{
		m_localVerts[vertIndex].m_color = m_mainColor;
	}
}
