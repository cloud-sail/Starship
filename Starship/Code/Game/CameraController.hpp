#pragma once
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"


constexpr float TRAUMA_REDUCED_RATE = 1.0f;
constexpr float MAX_CAM_SHAKE_OFFSET = 3.f;

class Entity;

class CameraController
{

public:
	CameraController(Game* game, Vec2 const& startPos,Entity* target);
	~CameraController() = default;
	
	void Update();

	Vec2 const GetCameraPosition() const;
	void BindTarget(Entity* target);
	void SetCameraSize(float ratio);

	void AddTrauma(float stress);
	AABB2 GetCameraView() const;
	

public:
	Vec2 m_position;
	Vec2 m_halfDimensions;

	Entity* m_target = nullptr; // check nullptr before use
	AABB2 m_bounds;

	Camera m_camera; // just for render, after effect, do not use the info in game logic

private:
	float GetTimeRelatedShakeX(float seconds);
	float GetTimeRelatedShakeY(float seconds);

private:
	Game* m_game = nullptr;
	float m_trauma = 0.f;
	float m_time = 0.f;
};

