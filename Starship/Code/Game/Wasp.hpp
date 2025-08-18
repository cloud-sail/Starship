#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

// ----------------------------------------------------------------------------------------------
constexpr int NUM_WASP_TRIANGLES = 2;
constexpr int NUM_WASP_VERTS = 3 * NUM_WASP_TRIANGLES;


class Wasp: public Entity
{
public:
	Wasp(Game* game);
	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
private:
	void InitializeLocalVerts();

private:
	Vertex_PCU m_localVerts[NUM_WASP_VERTS];
};


