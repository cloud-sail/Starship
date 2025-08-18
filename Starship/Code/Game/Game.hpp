#pragma once

#include "Game/GameCommon.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"


// ----------------------------------------------------------------------------------------------
class App;
class PlayerShip;
class Asteroid;
class Bullet;
class Entity;
class Beetle;
class Wasp;
class Debris;
class DebrisSystem;
class CameraController;
class Clock;

struct Vec2;

struct EnemyWave
{
	int m_numNewAsteroid = 1;
	int m_numNewBeetles = 0;
	int m_numNewWasps = 0;
};

constexpr int NUM_WAVES = 5;

// ----------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();
	void Update();
	void Render() const;
	bool IsAlive(Entity* entity) const;
	bool ShouldGameReset() const { return m_shouldGameResetNextFrame; }

public:
	bool IsPlayerShipAlive() const;
	Vec2 const GetPlayerShipPosition() const;
	Vec2 const GetRandomPositionJustOffCamera(float radius) const;

public:
	DebrisSystem* m_debrisSystem = nullptr;
	CameraController* m_worldCamera = nullptr;
	Clock* m_gameClock = nullptr;

private:
	void UpdateEntities();
	void RenderEntities() const;
	void RenderEnemyIndicator() const;
	void RenderEnemyIndicatorList(int listMaxSize, Entity* const* entityList) const;
	void RenderBackgroundGrid() const;
	void DebugRenderEntities() const;
	void RenderUI() const;
	void RecycleGarbageEntities();

	void CheckBulletsVsEnemies();
	void HandleBulletVsEnemy(Bullet* bullet, Entity* enemy);
	void CheckEnemiesVsShips();
	void HandleShipVsEnemy(PlayerShip* ship, Entity* enemy);
	void PushOffEnemies();
	void PushOffSameTypeEnemy(int listMaxSize, Entity** entityList);
	void PushOffDifferentTypeEnemies(int listMaxSizeA, Entity** entityListA, int listMaxSizeB, Entity** entityListB);


	void HandleElasticCollision(Entity* a, Entity* b);


private:
	void UpdateCameras();

private:
	// Camera
	Camera m_screenCamera;

	// Loop Music
	SoundPlaybackID m_musicPlaybackID = MISSING_SOUND_ID;

//-----------------------------------------------------------------------------------------------
// Entity Management
public:
	void GenerateBullet(Vec2 const& startPosition, float startOrientationDegrees);

public:
	PlayerShip* m_playerShip = nullptr; // One player now

private:
	// The “= {};” syntax initializes the array to zeros.
	Entity* m_bullets[MAX_BULLETS] = {};
	Entity* m_asteroids[MAX_ASTEROIDS] = {};
	Entity* m_beetles[MAX_BEETLES] = {};
	Entity* m_wasps[MAX_WASPS] = {};

private:
	void GenerateAsteroid();
	void GenerateBeetle();
	void GenerateWasp();

	void UpdateEntityList(int listMaxSize, Entity** entityList, float deltaSeconds);
	void RenderEntityList(int listMaxSize, Entity* const* entityList) const;
	void DebugRenderEntityList(int listMaxSize, Entity* const* entityList) const;
	void RecycleGarbageEntityList(int listMaxSize, Entity** entityList);
	void DestroyEntityList(int listMaxSize, Entity** entityList);

//-----------------------------------------------------------------------------------------------
// Game States
private:
	void GenerateEnemyWave();
	void CheckGameStatus();
	void RestartGame();

private:
	EnemyWave m_waves[NUM_WAVES] = {
		{1, 1, 1},
		{3, 1, 1},
		{1, 2, 2},
		{1, 3, 1},
		{1, 3, 3}
	};
	int m_currentWave = 0;

	bool m_isGameOver = false;
	float m_timeAfterGameOver = 0.f;
	bool m_shouldGameResetNextFrame = false;

//-----------------------------------------------------------------------------------------------
// Attractive Mode
private:
	void UpdateAttractiveMode();
	void RenderAttractiveMode() const;

private:
	bool m_isAttractiveMode = true;
	Rgba8 m_attractiveColor[3];

//-----------------------------------------------------------------------------------------------
// Game Input
private:
	void HandleInputKeys();

private:
	bool m_isSlowDown = false;
};

bool OnTimeScaleChanged(EventArgs& args);
