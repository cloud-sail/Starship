#include "Game/App.hpp"
#include "Game/Game.hpp"
#include "Game/Sound.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/CameraController.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"


PlayerShip::PlayerShip(Game* game, Vec2 const& startPosition, float startOrientationDegrees)
	: Entity(game, startPosition, startOrientationDegrees)
{
	m_physicsRadius = PLAYER_SHIP_PHYSICS_RADIUS;
	m_cosmeticRadius = PLAYER_SHIP_COSMETIC_RADIUS;
	m_mainColor = Rgba8(102, 153, 204);

	m_maxHealth = PLAYER_MAX_HEALTH;
	m_health	= PLAYER_MAX_HEALTH;
	InitializeLocalVerts();
}

void PlayerShip::Update(float deltaSeconds) // disable when dead
{
	ShipKeyBoardInput keyBoardInput = GetKeyBoardInput();
	ShipControllerInput controllerInput = GetControllerInput(0);
	if (m_isDead)
	{
		if ((!keyBoardInput.isRespawnKeyDown && !controllerInput.isRespawnKeyDown) || m_extraLives <= 0)
		{
			return;
		}
		Respawn();
	}
	if (m_isDead) return;
	if (keyBoardInput.isFiring || controllerInput.isFiring)
	{
		FireBullet();
	}
	UpdateMovement(deltaSeconds, keyBoardInput, controllerInput);
}

void PlayerShip::Render() const
{
	if (m_isDead) return;
	Vertex_PCU tempWorldVerts[NUM_PLAYER_SHIP_VERTS + 3];
	// Tail
	tempWorldVerts[0].m_position = Vec3(-2.f, -1.f, 0.f);
	tempWorldVerts[1].m_position = Vec3(-2.f, 1.f, 0.f);
	tempWorldVerts[2].m_position = Vec3(-2.f - m_thrustFraction * 4.f, 0.f, 0.f);

	tempWorldVerts[0].m_color = Rgba8::RED;
	tempWorldVerts[1].m_color = Rgba8::RED;
	tempWorldVerts[2].m_color = Rgba8::YELLOW;

	for (int vertIndex = 0; vertIndex < NUM_PLAYER_SHIP_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex+3] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_PLAYER_SHIP_VERTS + 3, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_PLAYER_SHIP_VERTS + 3, &tempWorldVerts[0]);

	RenderHealthBar();
}

void PlayerShip::Die()
{
	m_isDead = true;
}

int PlayerShip::GetExtraLives() const
{
	return m_extraLives;
}

void PlayerShip::RenderShipIcon(Vec2 const& position, float orientationDegrees, float scale)
{
	Vertex_PCU tempWorldVerts[NUM_PLAYER_SHIP_VERTS];
	// Left wing
	tempWorldVerts[0].m_position = Vec3(-2.f, 1.f, 0.f);
	tempWorldVerts[1].m_position = Vec3(2.f, 1.f, 0.f);
	tempWorldVerts[2].m_position = Vec3(0.f, 2.f, 0.f);

	// Rear body
	tempWorldVerts[3].m_position = Vec3(-2.f, 1.f, 0.f);
	tempWorldVerts[4].m_position = Vec3(-2.f, -1.f, 0.f);
	tempWorldVerts[5].m_position = Vec3(0.f, 1.f, 0.f);

	// Mid body
	tempWorldVerts[6].m_position = Vec3(-2.f, -1.f, 0.f);
	tempWorldVerts[7].m_position = Vec3(0.f, -1.f, 0.f);
	tempWorldVerts[8].m_position = Vec3(0.f, 1.f, 0.f);

	// Nose
	tempWorldVerts[9].m_position = Vec3(0.f, -1.f, 0.f);
	tempWorldVerts[10].m_position = Vec3(1.f, 0.f, 0.f);
	tempWorldVerts[11].m_position = Vec3(0.f, 1.f, 0.f);

	// Right wing
	tempWorldVerts[12].m_position = Vec3(-2.f, -1.f, 0.f);
	tempWorldVerts[13].m_position = Vec3(0.f, -2.f, 0.f);
	tempWorldVerts[14].m_position = Vec3(2.f, -1.f, 0.f);

	for (int vertIndex = 0; vertIndex < NUM_PLAYER_SHIP_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex].m_color = Rgba8(102, 153, 204);
	}
	TransformVertexArrayXY3D(NUM_PLAYER_SHIP_VERTS, &tempWorldVerts[0], scale, orientationDegrees, position);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(NUM_PLAYER_SHIP_VERTS, &tempWorldVerts[0]);
}

void PlayerShip::InitializeLocalVerts()
{
	// Left wing
	m_localVerts[0].m_position	= Vec3(-2.f, 1.f, 0.f);
	m_localVerts[1].m_position	= Vec3(2.f, 1.f, 0.f);
	m_localVerts[2].m_position	= Vec3(0.f, 2.f, 0.f);

	// Rear body
	m_localVerts[3].m_position = Vec3(-2.f, 1.f, 0.f);
	m_localVerts[4].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[5].m_position = Vec3(0.f, 1.f, 0.f);

	// Mid body
	m_localVerts[6].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[7].m_position = Vec3(0.f, -1.f, 0.f);
	m_localVerts[8].m_position = Vec3(0.f, 1.f, 0.f);

	// Nose
	m_localVerts[9].m_position  = Vec3(0.f, -1.f, 0.f);
	m_localVerts[10].m_position = Vec3(1.f, 0.f, 0.f);
	m_localVerts[11].m_position = Vec3(0.f, 1.f, 0.f);

	// Right wing
	m_localVerts[12].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[13].m_position = Vec3(0.f, -2.f, 0.f);
	m_localVerts[14].m_position = Vec3(2.f, -1.f, 0.f);

	for (int vertIndex = 0; vertIndex < NUM_PLAYER_SHIP_VERTS; ++vertIndex)
	{
		m_localVerts[vertIndex].m_color = m_mainColor;
	}
}

void PlayerShip::UpdateMovement(float deltaSeconds, ShipKeyBoardInput const& keyBoardInput, ShipControllerInput const& controllerInput)
{
	float thrustFraction = 0.f;
	if (controllerInput.isConnected)
	{
		
		if (controllerInput.leftStickMagnitude > 0.f)
		{
			thrustFraction = keyBoardInput.isThrusting ? 1.f : controllerInput.leftStickMagnitude;
			m_orientationDegrees = controllerInput.leftStickOrientationDegrees;
		}
		else
		{
			if (keyBoardInput.isTurningLeft)
			{
				m_orientationDegrees += PLAYER_SHIP_TURN_SPEED * deltaSeconds;
			}
			if (keyBoardInput.isTurningRight)
			{
				m_orientationDegrees -= PLAYER_SHIP_TURN_SPEED * deltaSeconds;
			}
			thrustFraction = keyBoardInput.isThrusting ? 1.f : 0.f;
		}
	}
	else {
		if (keyBoardInput.isTurningLeft)
		{
			m_orientationDegrees += PLAYER_SHIP_TURN_SPEED * deltaSeconds;
		}
		if (keyBoardInput.isTurningRight)
		{
			m_orientationDegrees -= PLAYER_SHIP_TURN_SPEED * deltaSeconds;
		}
		thrustFraction = keyBoardInput.isThrusting ? 1.f : 0.f;
		
	}
	Vec2 deltaVelocity = Vec2::MakeFromPolarDegrees(m_orientationDegrees) * (deltaSeconds * PLAYER_SHIP_ACCELERATION * thrustFraction);
	m_thrustFraction = thrustFraction;

	AddVelocity(PLAYER_MAX_SPEED, deltaVelocity);

	m_position += deltaSeconds * m_velocity;

	// Bounce off Walls
	if (m_position.x < m_physicsRadius)
	{
		m_position.x = m_physicsRadius;
		m_velocity.x *= -1;
	}
	if (m_position.x > (WORLD_SIZE_X - m_physicsRadius)) 
	{
		m_position.x = WORLD_SIZE_X - m_physicsRadius;
		m_velocity.x *= -1;
	}
	if (m_position.y < m_physicsRadius)
	{
		m_position.y = m_physicsRadius;
		m_velocity.y *= -1;
	}
	if (m_position.y > (WORLD_SIZE_Y - m_physicsRadius))
	{
		m_position.y = WORLD_SIZE_Y - m_physicsRadius;
		m_velocity.y *= -1;
	}
}

Vec2 const PlayerShip::GetFirePosition() const
{
	return m_position + Vec2::MakeFromPolarDegrees(m_orientationDegrees);
}

void PlayerShip::FireBullet()
{
	Vec2 firePosition = GetFirePosition();
	g_theAudio->StartSound(Sound::PLAYER_SHOOT);
	m_game->m_worldCamera->AddTrauma(0.05f);
	m_game->GenerateBullet(firePosition, m_orientationDegrees);
}

ShipKeyBoardInput PlayerShip::GetKeyBoardInput()
{
	ShipKeyBoardInput input = {};
	input.isThrusting = g_theInput->IsKeyDown(KEYCODE_E);
	input.isTurningLeft = g_theInput->IsKeyDown(KEYCODE_S);
	input.isTurningRight = g_theInput->IsKeyDown(KEYCODE_F);
	input.isFiring = g_theInput->WasKeyJustPressed(KEYCODE_SPACE);
	input.isRespawnKeyDown = g_theInput->WasKeyJustPressed(KEYCODE_N);

	return input;
}

ShipControllerInput PlayerShip::GetControllerInput(int controllerID)
{
	ShipControllerInput input = {};
	XboxController const& controller = g_theInput->GetController(controllerID);

	input.isConnected = controller.IsConnected();
	input.isFiring = controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_A);
	input.isRespawnKeyDown = controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_START);

	float leftStickMagnitude = controller.GetLeftStick().GetMagnitude();
	if (leftStickMagnitude > 0.f)
	{
		input.leftStickMagnitude = leftStickMagnitude;
		input.leftStickOrientationDegrees = controller.GetLeftStick().GetOrientationDegrees();
	}
	return input;
}

void PlayerShip::Respawn()
{
	m_isDead = false;
	m_orientationDegrees = 0.f; 
	m_angularVelocity = 0.f; 
	m_health = PLAYER_MAX_HEALTH;
	m_isDead = false;
	m_isGarbage = false;

	m_velocity = Vec2();
	m_position = Vec2(WORLD_CENTER_X, WORLD_CENTER_Y);
	m_extraLives -= 1;

}
