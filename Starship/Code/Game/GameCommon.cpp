#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"



void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	constexpr int NUM_SIDES = 32;
	constexpr int NUM_TRIS = 2 * NUM_SIDES;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;
	Vertex_PCU verts[NUM_VERTS];

	constexpr float DEGREES_PER_SIDE = 360.f / static_cast<float>(NUM_SIDES);

	float halfThickness = 0.5f * thickness;
	float innerRadius = radius - halfThickness;
	float outerRadius = radius + halfThickness;

	for (int sideIndex = 0; sideIndex < NUM_SIDES; ++sideIndex)
	{
		// Compute angle-related terms
		float startDegrees	= DEGREES_PER_SIDE * static_cast<float>(sideIndex);
		float endDegrees	= DEGREES_PER_SIDE * static_cast<float>(sideIndex + 1);

		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);

		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		// Compute inner & outer positions
		Vec3 innerStartPos	= Vec3(center.x + innerRadius * cosStart, center.y + innerRadius * sinStart, 0.f);
		Vec3 outerStartPos	= Vec3(center.x + outerRadius * cosStart, center.y + outerRadius * sinStart, 0.f);
		Vec3 innerEndPos	= Vec3(center.x + innerRadius * cosEnd	, center.y + innerRadius * sinEnd, 0.f);
		Vec3 outerEndPos	= Vec3(center.x + outerRadius * cosEnd	, center.y + outerRadius * sinEnd, 0.f);
		
		// Trapezoid is made of two triangles: ABC and DEF
		// A is inner end; B is inner start; C is outer start
		// D is inner end; E is outer start; F is outer end
		int offset = 6 * sideIndex;
		verts[offset].m_position	 = innerEndPos;	   	// A
		verts[offset + 1].m_position = innerStartPos;	// B
		verts[offset + 2].m_position = outerStartPos;	// C
		verts[offset].m_color = color;
		verts[offset + 1].m_color = color;
		verts[offset + 2].m_color = color;


		verts[offset + 3].m_position = innerEndPos;		// D
		verts[offset + 4].m_position = outerStartPos;	// E
		verts[offset + 5].m_position = outerEndPos;		// F
		verts[offset + 3].m_color = color;
		verts[offset + 4].m_color = color;
		verts[offset + 5].m_color = color;
	}
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);
}

void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	Vec2 forward = (end - start).GetNormalized();
	Vec2 left = forward.GetRotated90Degrees();
	float h = thickness * 0.5f;


	constexpr int NUM_VERTS = 6;
	Vertex_PCU verts[NUM_VERTS];

	Vec2 startLeft	= start - forward * h + left * h;
	Vec2 startRight = start - forward * h - left * h;
	Vec2 endLeft	= end + forward * h + left * h;
	Vec2 endRight	= end + forward * h - left * h;

	verts[0].m_position = Vec3(startLeft.x, startLeft.y, 0.f);
	verts[1].m_position = Vec3(startRight.x, startRight.y, 0.f);
	verts[2].m_position = Vec3(endRight.x, endRight.y, 0.f);
	verts[3].m_position = Vec3(startLeft.x, startLeft.y, 0.f);
	verts[4].m_position = Vec3(endRight.x, endRight.y, 0.f);
	verts[5].m_position = Vec3(endLeft.x, endLeft.y, 0.f);


	for (int vertIndex = 0; vertIndex < NUM_VERTS; ++vertIndex)
	{
		verts[vertIndex].m_color = color;
	}

	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);

}

Vec2 GetRelatedTranslation(Vec2 const& parentWorldPos, float parentWorldDegrees, Vec2 const& childWorldPos)
{
	Vec2 displacement = childWorldPos - parentWorldPos;
	Vec2 iBasis = Vec2::MakeFromPolarDegrees(parentWorldDegrees);
	Vec2 jBasis = iBasis.GetRotated90Degrees();
	return Vec2(DotProduct2D(iBasis, displacement), DotProduct2D(jBasis, displacement));
}

float GetRelatedOrientation(float parentWorldDegrees, float childWorldDegrees)
{
	return childWorldDegrees - parentWorldDegrees;
}

