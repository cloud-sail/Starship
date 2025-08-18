#include "Game/CameraController.hpp"
#include "Game/Entity.hpp"
#include "Game/Game.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Clock.hpp"

CameraController::CameraController(Game* game, Vec2 const& startPos, Entity* target)
	: m_position(startPos)
	, m_halfDimensions(CAMERA_SIZE_X * 0.5f, CAMERA_SIZE_Y * 0.5f)
	, m_target(target)
	, m_bounds(CAMERA_SIZE_X * 0.2f, CAMERA_SIZE_Y * 0.2f, WORLD_SIZE_X - CAMERA_SIZE_X * 0.2f, WORLD_SIZE_Y - CAMERA_SIZE_Y * 0.2f)
	, m_game(game)
{
	m_camera.SetOrthoView(m_position-m_halfDimensions, m_position + m_halfDimensions);
}

void CameraController::Update()
{
	float deltaSeconds = static_cast<float>(m_game->m_gameClock->GetDeltaSeconds());
	m_time = static_cast<float>(m_game->m_gameClock->GetTotalSeconds());
	// camera follow
	if (m_target != nullptr)
	{
		Vec2 target = m_target->m_position;
		m_position += 0.1f * (target - m_position) * deltaSeconds * 30.f;
	}

	AABB2 cameraView(m_position - m_halfDimensions, m_position + m_halfDimensions);

	// Camera Shake
	if (m_trauma > 0.f)
	{
		float shake = m_trauma * m_trauma;
		float offsetX = MAX_CAM_SHAKE_OFFSET * shake * GetTimeRelatedShakeX(m_time);
		float offsetY = MAX_CAM_SHAKE_OFFSET * shake * GetTimeRelatedShakeY(m_time);
		cameraView.Translate(Vec2(offsetX, offsetY));
		m_trauma -= deltaSeconds * TRAUMA_REDUCED_RATE;
		m_trauma = GetClampedZeroToOne(m_trauma);
	}

	// Camera Clamp
	if (cameraView.m_mins.x < 0.f)
	{
		cameraView.Translate(Vec2(-cameraView.m_mins.x, 0.f));
	}
	if (cameraView.m_mins.y < 0.f)
	{
		cameraView.Translate(Vec2(0.f, -cameraView.m_mins.y));
	}
	if (cameraView.m_maxs.x > WORLD_SIZE_X)
	{
		cameraView.Translate(Vec2(WORLD_SIZE_X - cameraView.m_maxs.x, 0.f));
	}
	if (cameraView.m_maxs.y > WORLD_SIZE_Y)
	{
		cameraView.Translate(Vec2(0.f, WORLD_SIZE_Y - cameraView.m_maxs.y));
	}

	m_camera.SetOrthoView(cameraView.m_mins, cameraView.m_maxs);
}


Vec2 const CameraController::GetCameraPosition() const
{
	return m_position;
}

void CameraController::BindTarget(Entity* target)
{
	m_target = target;
}

void CameraController::SetCameraSize(float ratio)
{
	m_halfDimensions = Vec2(CAMERA_SIZE_X, CAMERA_SIZE_Y) * 0.5f * ratio;
}

void CameraController::AddTrauma(float stress)
{
	m_trauma += stress;
	m_trauma = GetClampedZeroToOne(m_trauma);
}

AABB2 CameraController::GetCameraView() const
{
	return AABB2(m_camera.GetOrthoBottomLeft(), m_camera.GetOrthoTopRight());
}

float CameraController::GetTimeRelatedShakeX(float seconds)
{
	float x = 20.f * seconds;
	return (SinRadians(2.f * x) + SinRadians(3.1415926535897932384626433832795f * x)) * 0.5f;
}

float CameraController::GetTimeRelatedShakeY(float seconds)
{
	float x = 20.f * seconds;
	return (SinRadians(-2.2f * x) + SinRadians(1.2f * 3.1415926535897932384626433832795f * x)) * 0.5f;
}


