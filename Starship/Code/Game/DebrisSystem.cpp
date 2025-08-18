#include "Game/DebrisSystem.hpp"
#include "Game/Debris.hpp"

DebrisSystem::DebrisSystem(Game* game)
	: m_game(game)
{
}

DebrisSystem::~DebrisSystem()
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			delete m_debris[debrisIndex];
			m_debris[debrisIndex] = nullptr;
		}
	}
}

void DebrisSystem::Update(float deltaSeconds)
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			m_debris[debrisIndex]->Update(deltaSeconds);
		}
	}
}

void DebrisSystem::Render() const
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			m_debris[debrisIndex]->Render();
		}
	}
}

void DebrisSystem::DebugRender() const
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			m_debris[debrisIndex]->DebugRender();

		}
	}
}

void DebrisSystem::RecycleGarbage()
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] != nullptr)
		{
			if (m_debris[debrisIndex]->IsGarbage())
			{
				delete m_debris[debrisIndex];
				m_debris[debrisIndex] = nullptr;
			}
		}
	}
}

void DebrisSystem::SpawnDebrisCluster(int numDebris, Vec2 const& position, Vec2 const& momentumVelocity, Rgba8 const& color, float maxScatterSpeed, float averageRadius)
{
	for (int debrisIndex = 0; debrisIndex < numDebris; ++debrisIndex)
	{
		Vec2 velocity = 0.3f * momentumVelocity;
		velocity += (Vec2::MakeFromPolarDegrees(m_rng.RollRandomFloatInRange(0.f, 360.f)) * m_rng.RollRandomFloatInRange(0.f, maxScatterSpeed));
		SpawnDebris(position, velocity, color, averageRadius);
	}
}

void DebrisSystem::SpawnDebris(Vec2 const& position, Vec2 const& velocity, Rgba8 const& color, float averageRadius)
{
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex)
	{
		if (m_debris[debrisIndex] == nullptr)
		{
			m_debris[debrisIndex] = new Debris(m_game, position, velocity, color, averageRadius);
			return;
		}
	}
}




