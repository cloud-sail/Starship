#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

// ----------------------------------------------------------------------------------------------
constexpr int NUM_BEETLE_TRIANGLES = 6;
constexpr int NUM_BEETLE_VERTS = 3 * NUM_BEETLE_TRIANGLES;



class Beetle: public Entity
{
public:
	Beetle(Game* game);
	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;

private:
	void InitializeLocalVerts();

private:
	Vertex_PCU m_localVerts[NUM_BEETLE_VERTS];
};

