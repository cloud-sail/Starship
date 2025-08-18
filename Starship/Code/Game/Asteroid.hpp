#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

// ----------------------------------------------------------------------------------------------
constexpr int NUM_ASTEROID_TRIANGLES = 16;
constexpr int NUM_ASTEROID_VERTS = 3 * NUM_ASTEROID_TRIANGLES;


// ----------------------------------------------------------------------------------------------
class Asteroid : public Entity
{
public:
	Asteroid(Game* game);

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;

private:
	void InitializeLocalVerts();
	void WrapAround();

private:
	Vertex_PCU m_localVerts[NUM_ASTEROID_VERTS];
};