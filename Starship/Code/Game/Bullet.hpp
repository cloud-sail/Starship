#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

// ----------------------------------------------------------------------------------------------
constexpr int NUM_BULLET_TRIANGLES = 2;
constexpr int NUM_BULLET_VERTS = 3 * NUM_BULLET_TRIANGLES;


//------------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
	Bullet(Game* game, Vec2 const& startPosition, float startOrientationDegrees);

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;

private:
	void InitializeLocalVerts();

private:
	Vertex_PCU m_localVerts[NUM_BULLET_VERTS];
	// lifeTime for bullet
};