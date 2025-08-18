#include "Game/CollisionPair.hpp"

CollisionPair::CollisionPair()
	: a(nullptr)
	, b(nullptr)
{
}

CollisionPair::CollisionPair(Entity* entityA, Entity* entityB)
{
	if (entityA < entityB)
	{
		a = entityA;
		b = entityB;
	}
	else
	{
		a = entityB;
		b = entityA;
	}
}

bool CollisionPair::IsEqual(CollisionPair const& other) const
{
	return (a == other.a) && (b == other.b);
}
