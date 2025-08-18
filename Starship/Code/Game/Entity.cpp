#include "Game/Entity.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"


Entity::Entity(Game* game, Vec2 const& startPosition, float startOrientationDegrees)
	: m_game(game)
	, m_position(startPosition)
	, m_orientationDegrees(startOrientationDegrees)
{
}

void Entity::DebugRender() const
{
    // Forward Vector
    DebugDrawLine(m_position, m_position + Vec2::MakeFromPolarDegrees(m_orientationDegrees) * m_cosmeticRadius, 0.2f, Rgba8(255, 0, 0));
    // Relative-left Vector
    DebugDrawLine(m_position, m_position + Vec2::MakeFromPolarDegrees(m_orientationDegrees).GetRotated90Degrees() * m_cosmeticRadius, 0.2f, Rgba8(0, 255, 0));
	// Cosmetic (Outer) Radius
	DebugDrawRing(m_position, m_cosmeticRadius, .2f, Rgba8(255, 0, 255));
    // Physics (Inner) Radius
	DebugDrawRing(m_position, m_physicsRadius, .2f, Rgba8(0, 255, 255));
    // Velocity Vector
	DebugDrawLine(m_position, m_position + m_velocity, 0.2f, Rgba8(255, 255, 0));
}

void Entity::Die()
{
    m_isDead = true;
    m_isGarbage = true;
}

void Entity::OnCollision(Entity* other)
{
    UNUSED(other);
}


float Entity::GetCollisionDamage() const
{
    return 0.0f;
}

void Entity::TakeDamage(int damage)
{
    m_health -= damage;
    if (m_health <= 0)
    {
        Die();
        m_health = 0;
    }
}

void Entity::AddVelocity(float speedLimit, Vec2 const& deltaVelocity)
{
    float currentSpeed = m_velocity.GetLength();
    if (currentSpeed < speedLimit)
    {
        m_velocity += deltaVelocity;
    }
    else
    {
        Vec2 iBasis = m_velocity.GetNormalized();
        Vec2 jBasis = iBasis.GetRotated90Degrees();
        float deltaIBasis = DotProduct2D(iBasis, deltaVelocity);
        float deltaJBasis = DotProduct2D(jBasis, deltaVelocity);
        if (deltaIBasis > 0.f)
        {
            deltaIBasis = 0.f;
        }
        m_velocity += (iBasis * deltaIBasis + jBasis * deltaJBasis);
    }
}

void Entity::RenderHealthBar() const
{
	constexpr float thickness = 0.5f;
	constexpr float verticalOffset = 2.5f;
	constexpr float barLength = 4.f;

	Vec2 barStartPos = m_position + Vec2(barLength * -0.5f, verticalOffset);
	Vec2 barBackgroundEndPos = barStartPos + Vec2(barLength, 0.f);
	Vec2 barFrontEndPos = barStartPos + Vec2(barLength * static_cast<float>(m_health) / static_cast<float>(m_maxHealth), 0.f);

	std::vector<Vertex_PCU> verts;
	AddVertsForLineSegment2D(verts, barStartPos, barBackgroundEndPos, thickness, Rgba8::RED);
	AddVertsForLineSegment2D(verts, barStartPos, barFrontEndPos, thickness, Rgba8::GREEN);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(verts);
}

bool Entity::IsAlive() const
{
	return !m_isDead;
}

bool Entity::IsGarbage() const
{
    return m_isGarbage;
}

float Entity::GetPhysicsRadius() const
{
	return m_physicsRadius;
}

float Entity::GetCosmeticRadius() const
{
	return m_cosmeticRadius;
}

Rgba8 const Entity::GetMainColor() const
{
	return m_mainColor;
}

//Vec2 const Entity::GetWorldPosForLocalPos(Vec2 const& localPos) const
//{
//    // No scale in Starship
//    Vec2 result = localPos.GetRotatedDegrees(m_orientationDegrees);
//    result += m_position;
//    if (m_parent == nullptr)
//    {
//        return result;
//    }
//    else
//    {
//        return m_parent->GetWorldPosForLocalPos(result);
//    }
//
//}
