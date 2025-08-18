#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

constexpr int NUM_DEBRIS_TRIANGLES = 8;
constexpr int NUM_DEBRIS_VERTS = 3 * NUM_DEBRIS_TRIANGLES;

class Debris : public Entity
{
public:
	Debris(Game* game, Vec2 const& position, Vec2 const& velocity, Rgba8 const& color, float averageRadius);
	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;

private:
	void InitializeLocalVerts();

private:
	Vertex_PCU m_localVerts[NUM_DEBRIS_VERTS];
	float m_outerRadius = 0.f;
	float m_innerRadius = 0.f;
};

