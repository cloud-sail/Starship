#pragma once
#include "Game/GameCommon.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

class Debris;

class DebrisSystem
{
public:
	DebrisSystem(Game* game);
	~DebrisSystem();

	void Update(float deltaSeconds);
	void Render() const;
	void DebugRender() const;

	void RecycleGarbage();

	void SpawnDebrisCluster(int numDebris, Vec2 const& position, Vec2 const& momentumVelocity, Rgba8 const& color, float maxScatterSpeed, float averageRadius);
	void SpawnDebris(Vec2 const& position, Vec2 const& velocity, Rgba8 const& color, float averageRadius);

private:
	RandomNumberGenerator m_rng;
	Game* m_game = nullptr;
	Debris* m_debris[MAX_DEBRIS] = {};

};

