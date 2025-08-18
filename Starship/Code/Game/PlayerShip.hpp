#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

// ----------------------------------------------------------------------------------------------
constexpr int NUM_PLAYER_SHIP_TRIANGLES = 5;
constexpr int NUM_PLAYER_SHIP_VERTS = 3 * NUM_PLAYER_SHIP_TRIANGLES;

struct  ShipKeyBoardInput
{
	bool isThrusting = false;
	bool isTurningLeft = false;
	bool isTurningRight = false;
	bool isRespawnKeyDown = false;
	bool isFiring = false;
};

struct  ShipControllerInput
{
	bool isRespawnKeyDown = false;
	bool isConnected = false;
	bool isFiring = false;

	float leftStickOrientationDegrees = 0.f;
	float leftStickMagnitude = 0.f;

};

// ----------------------------------------------------------------------------------------------
class PlayerShip : public Entity
{
public:
	PlayerShip(Game* game, Vec2 const& startPosition, float startOrientationDegrees);

	virtual void Update(float deltaSeconds) override;
	virtual void Render() const override;
	virtual void Die() override;

	int GetExtraLives() const;

	static void RenderShipIcon(Vec2 const& position, float orientationDegrees, float scale);

private:
	void InitializeLocalVerts(); // make it static 
	void UpdateMovement(float deltaSeconds, ShipKeyBoardInput const& keyBoardInput, ShipControllerInput const& controllerInput);
	
	Vec2 const GetFirePosition() const;
	void FireBullet();
	ShipKeyBoardInput GetKeyBoardInput();
	ShipControllerInput GetControllerInput(int controllerID);

	void Respawn();

private:
	Vertex_PCU m_localVerts[NUM_PLAYER_SHIP_VERTS];

	bool m_isThrusting		= false;
	bool m_isTurningLeft	= false;
	bool m_isTurningRight	= false;

	float m_thrustFraction = 0.f;
	int m_extraLives = 3;

};