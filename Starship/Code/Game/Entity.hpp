#pragma once
#include "Game/GameCommon.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"

//------------------------------------------------------------------------------------------------
class Entity
{
public:
	Entity(Game* game, Vec2 const& startPosition, float startOrientationDegrees);
	virtual ~Entity() = default;
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;
	virtual void DebugRender() const;
	virtual void Die();
	virtual void OnCollision(Entity* other);
	virtual float GetCollisionDamage() const;

	bool IsAlive() const;
	bool IsGarbage() const;

	float GetPhysicsRadius() const;
	float GetCosmeticRadius() const;
	Rgba8 const GetMainColor() const;

	//Vec2 const GetWorldPosForLocalPos(Vec2 const& localPos) const;

	void TakeDamage(int damage);

	void AddVelocity(float speedLimit, Vec2 const& deltaVelocity);

	void RenderHealthBar() const;

public:

	Vec2	m_position;
	float	m_orientationDegrees = 0.f; // counter-clockwise from +x/east

	float	m_mass = 1.f;
	Vec2	m_velocity;
	float	m_angularVelocity = 0.f; // (signed) spin rate, in degrees per second, + is counter-clockwise

	Rgba8	m_mainColor; // for Debris Color
	float	m_physicsRadius = 5.f;
	float	m_cosmeticRadius = 10.f;

	int	m_health = 1;
	int m_maxHealth = 1;

	bool m_isFriend = false;
	int m_damage = 1; // collision damage

protected:
	Game*	m_game = nullptr;
	bool	m_isDead = false; // game play idea
	bool	m_isGarbage = false; // code idea

	float   m_ageInSeconds = 0.f;

	// override Die Playership only dead not garbage
	// Default Die dead and garbage
	// in Die SpawnNewCluster

};