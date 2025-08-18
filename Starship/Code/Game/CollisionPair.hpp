#pragma once

//-----------------------------------------------------------------------------------------------
constexpr int MAX_COLLISION_PAIRS = 50;

//-----------------------------------------------------------------------------------------------
class Entity;


//-----------------------------------------------------------------------------------------------
struct CollisionPair
{
	Entity* a = nullptr;
	Entity* b = nullptr;

	CollisionPair();
	CollisionPair(Entity* entityA, Entity* entityB);

	bool IsEqual(CollisionPair const& other) const;
};