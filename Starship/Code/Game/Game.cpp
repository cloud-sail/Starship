#include "Game/Game.hpp"
#include "Game/App.hpp"
#include "Game/Asteroid.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/Bullet.hpp"
#include "Game/Beetle.hpp"
#include "Game/Wasp.hpp"
#include "Game/Debris.hpp"
#include "Game/DebrisSystem.hpp"
#include "Game/CameraController.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Sound.hpp"


#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"

const Rgba8 COLOR_PINK(255, 105, 180, 90);
const Rgba8 COLOR_PURPLE(138, 43, 226, 90);
const Rgba8 COLOR_TURQUOISE(64, 224, 208, 90);

Game::Game()
{
	m_playerShip = new PlayerShip(this, Vec2(WORLD_CENTER_X, WORLD_CENTER_Y), 0.f);
	m_debrisSystem = new DebrisSystem(this);
	m_worldCamera = new CameraController(this, Vec2(WORLD_CENTER_X, WORLD_CENTER_Y), m_playerShip);
	m_musicPlaybackID = g_theAudio->StartSound(Sound::BACKGROUND_MUSIC, true);
	m_gameClock = new Clock();
	g_theEventSystem->SubscribeEventCallbackFunction("TimeSpeed", OnTimeScaleChanged);
}

Game::~Game()
{
	delete m_gameClock;
	DestroyEntityList(MAX_BULLETS, m_bullets);
	DestroyEntityList(MAX_ASTEROIDS, m_asteroids);
	DestroyEntityList(MAX_BEETLES, m_beetles);
	DestroyEntityList(MAX_WASPS, m_wasps);

	g_theAudio->StopSound(m_musicPlaybackID);
	delete m_worldCamera;
	delete m_debrisSystem;
	delete m_playerShip;
}

void Game::Update()
{
	if (m_isAttractiveMode)
	{
		UpdateAttractiveMode();
	}
	else
	{
		HandleInputKeys();
		UpdateEntities();
		CheckBulletsVsEnemies();
		CheckEnemiesVsShips();
		PushOffEnemies();
		RecycleGarbageEntities();
		GenerateEnemyWave();
		CheckGameStatus();
	}
	UpdateCameras();
}

void Game::Render() const
{
	if (m_isAttractiveMode)
	{
		g_theRenderer->BeginCamera(m_screenCamera);
		RenderAttractiveMode();
		g_theRenderer->EndCamera(m_screenCamera);
		return;
	}

	g_theRenderer->BeginCamera(m_worldCamera->m_camera);
	
	RenderBackgroundGrid();
	RenderEntities();
	RenderEnemyIndicator();

	if (g_isDebugDraw)
	{
		DebugRenderEntities();
	}


	g_theRenderer->EndCamera(m_worldCamera->m_camera);

	g_theRenderer->BeginCamera(m_screenCamera);
	RenderUI();
	g_theRenderer->EndCamera(m_screenCamera);

}

Vec2 const Game::GetRandomPositionJustOffCamera(float radius) const
{
	RandomNumberGenerator rng;
	int sideNum = rng.RollRandomIntLessThan(4);

	switch (sideNum)
	{
	case 0:
		return Vec2(m_worldCamera->m_position.x - m_worldCamera->m_halfDimensions.x - radius,
					m_worldCamera->m_position.y + rng.RollRandomFloatInRange(-m_worldCamera->m_halfDimensions.y, m_worldCamera->m_halfDimensions.y));
		break;
	case 1:
		return Vec2(m_worldCamera->m_position.x + rng.RollRandomFloatInRange(-m_worldCamera->m_halfDimensions.x, m_worldCamera->m_halfDimensions.x),
					m_worldCamera->m_position.y - m_worldCamera->m_halfDimensions.y - radius);
		break;
	case 2:
		return Vec2(m_worldCamera->m_position.x + m_worldCamera->m_halfDimensions.x + radius,
					m_worldCamera->m_position.y + rng.RollRandomFloatInRange(-m_worldCamera->m_halfDimensions.y, m_worldCamera->m_halfDimensions.y));
		break;
	case 3:
		return Vec2(m_worldCamera->m_position.x + rng.RollRandomFloatInRange(-m_worldCamera->m_halfDimensions.x, m_worldCamera->m_halfDimensions.x),
					m_worldCamera->m_position.y + m_worldCamera->m_halfDimensions.y + radius);
		break;
	default:
		return Vec2(-radius, -radius);
		break;
	}
}

void Game::GenerateBullet(Vec2 const& startPosition, float startOrientationDegrees)
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
	{
		if (m_bullets[bulletIndex] == nullptr)
		{
			m_bullets[bulletIndex] = new Bullet(this, startPosition, startOrientationDegrees);
			return;
		}
	}
	// ERROR
	ERROR_RECOVERABLE("Cannot spawn a new bullet; all slots are full!")
}

bool Game::IsAlive(Entity* entity) const
{
	if (entity == nullptr)
	{
		return false;
	}
	return entity->IsAlive();
}

bool Game::IsPlayerShipAlive() const
{
	return m_playerShip->IsAlive();
}

Vec2 const Game::GetPlayerShipPosition() const
{
	return m_playerShip->m_position;
}



void Game::GenerateAsteroid()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
	{
		if (m_asteroids[asteroidIndex] == nullptr)
		{
			m_asteroids[asteroidIndex] = new Asteroid(this);
			return;
		}
	}
	// ERROR
	ERROR_RECOVERABLE("Cannot spawn a new asteroid; all slots are full!")
}

void Game::GenerateBeetle()
{
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex)
	{
		if (m_beetles[beetleIndex] == nullptr)
		{
			m_beetles[beetleIndex] = new Beetle(this);
			return;
		}
	}
}

void Game::GenerateWasp()
{
	for (int waspIndex = 0; waspIndex < MAX_BEETLES; ++waspIndex)
	{
		if (m_wasps[waspIndex] == nullptr)
		{
			m_wasps[waspIndex] = new Wasp(this);
			return;
		}
	}
}

void Game::UpdateEntityList(int listMaxSize, Entity** entityList, float deltaSeconds)
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (entityList[i] != nullptr)
		{
			entityList[i]->Update(deltaSeconds);
		}
	}
}

void Game::RenderEntityList(int listMaxSize, Entity* const* entityList) const
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (entityList[i] != nullptr)
		{
			entityList[i]->Render();
		}
	}
}

void Game::DebugRenderEntityList(int listMaxSize, Entity* const* entityList) const
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (entityList[i] != nullptr)
		{
			DebugDrawLine(m_playerShip->m_position, entityList[i]->m_position, .2f, Rgba8(50, 50, 50));
			entityList[i]->DebugRender();
		}
	}
}

void Game::RecycleGarbageEntityList(int listMaxSize, Entity** entityList)
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (entityList[i] != nullptr)
		{
			if (entityList[i]->IsGarbage())
			{
				delete entityList[i];
				entityList[i] = nullptr;
			}
		}
	}
}

void Game::DestroyEntityList(int listMaxSize, Entity** entityList)
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (entityList[i] != nullptr)
		{
			delete entityList[i];
			entityList[i] = nullptr;
		}
	}
}

void Game::UpdateEntities()
{
	float deltaSeconds = static_cast<float>(m_gameClock->GetDeltaSeconds());
	UpdateEntityList(MAX_BULLETS, m_bullets, deltaSeconds);
	UpdateEntityList(MAX_ASTEROIDS, m_asteroids, deltaSeconds);
	UpdateEntityList(MAX_BEETLES, m_beetles, deltaSeconds);
	UpdateEntityList(MAX_WASPS, m_wasps, deltaSeconds);

	m_debrisSystem->Update(deltaSeconds);
	m_playerShip->Update(deltaSeconds);
}

void Game::RenderEntities() const
{
	RenderEntityList(MAX_BULLETS, m_bullets);
	RenderEntityList(MAX_ASTEROIDS, m_asteroids);
	RenderEntityList(MAX_BEETLES, m_beetles);
	RenderEntityList(MAX_WASPS, m_wasps);

	m_debrisSystem->Render();
	m_playerShip->Render();
}

void Game::RenderEnemyIndicator() const
{
	RenderEnemyIndicatorList(MAX_ASTEROIDS, m_asteroids);
	RenderEnemyIndicatorList(MAX_BEETLES, m_beetles);
	RenderEnemyIndicatorList(MAX_WASPS, m_wasps);
}

void Game::RenderEnemyIndicatorList(int listMaxSize, Entity* const* entityList) const
{
	std::vector<Vertex_PCU> verts;
	AABB2 cameraView = m_worldCamera->GetCameraView();
	AABB2 indicatorBox = cameraView;
	indicatorBox.SetDimensions(cameraView.GetDimensions() * 0.95f);
	for (int i = 0; i < listMaxSize; ++i)
	{
		Entity* entity = entityList[i];
		if (entity != nullptr)
		{
			if (cameraView.IsPointInside(entity->m_position))
			{
				continue;
			}
			Vec2 nearestPoint = indicatorBox.GetNearestPoint(entity->m_position);
			AddVertsForDisc2D(verts, nearestPoint, 1.f, entity->GetMainColor());

		}
	}
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(verts);
}

void Game::RenderBackgroundGrid() const
{
	float cellWidth = WORLD_SIZE_X / static_cast<float>(DEBUG_GRID_NUM_X);
	float cellHeight = WORLD_SIZE_Y / static_cast<float>(DEBUG_GRID_NUM_Y);

	float thickness = 0.5f;

	for (int i = 0; i <= DEBUG_GRID_NUM_X; ++i)
	{
		float x = i * cellWidth;
		Vec2 start(x, 0.0f);
		Vec2 end(x, WORLD_SIZE_Y);

		Rgba8 color;
		if (i % 3 == 0)
			color = COLOR_PINK;
		else if (i % 3 == 1)
			color = COLOR_PURPLE;
		else
			color = COLOR_TURQUOISE;

		DebugDrawLine(start, end, thickness, color);
	}

	for (int j = 0; j <= DEBUG_GRID_NUM_Y; ++j)
	{
		float y = j * cellHeight;
		Vec2 start(0.0f, y);
		Vec2 end(WORLD_SIZE_X, y);

		Rgba8 color;
		if (j % 3 == 0)
			color = COLOR_PINK;
		else if (j % 3 == 1)
			color = COLOR_PURPLE;
		else
			color = COLOR_TURQUOISE;

		DebugDrawLine(start, end, thickness, color);
	}
}

void Game::DebugRenderEntities() const
{
	DebugRenderEntityList(MAX_BULLETS, m_bullets);
	DebugRenderEntityList(MAX_ASTEROIDS, m_asteroids);
	DebugRenderEntityList(MAX_BEETLES, m_beetles);
	DebugRenderEntityList(MAX_WASPS, m_wasps);

	m_debrisSystem->DebugRender();
	m_playerShip->DebugRender();
}

void Game::RenderUI() const
{
	int extraLives = m_playerShip->GetExtraLives();
	for (int i = 0; i < extraLives; ++i)
	{
		PlayerShip::RenderShipIcon(Vec2(i * 32.f + 20.f, SCREEN_SIZE_Y - 20.f), 90.f, 6.4f);
	}
}

void Game::RecycleGarbageEntities()
{
	RecycleGarbageEntityList(MAX_BULLETS, m_bullets);
	RecycleGarbageEntityList(MAX_ASTEROIDS, m_asteroids);
	RecycleGarbageEntityList(MAX_BEETLES, m_beetles);
	RecycleGarbageEntityList(MAX_WASPS, m_wasps);

	m_debrisSystem->RecycleGarbage();
}

void Game::CheckBulletsVsEnemies()
{

	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
	{
		if (!IsAlive(m_bullets[bulletIndex])) continue;
		Bullet* bullet = dynamic_cast<Bullet*>(m_bullets[bulletIndex]);
		if (!bullet) continue;
		for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
		{
			if (!IsAlive(m_asteroids[asteroidIndex])) continue;
			HandleBulletVsEnemy(bullet, m_asteroids[asteroidIndex]);
		}
		
		for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex)
		{
			if (!IsAlive(m_beetles[beetleIndex])) continue;
			HandleBulletVsEnemy(bullet, m_beetles[beetleIndex]);
		}

		for (int waspIndex = 0; waspIndex < MAX_WASPS; ++waspIndex)
		{
			if (!IsAlive(m_wasps[waspIndex])) continue;
			HandleBulletVsEnemy(bullet, m_wasps[waspIndex]);
		}
	}

}


void Game::HandleBulletVsEnemy(Bullet* bullet, Entity* enemy)
{
	if (DoDiscsOverlap(enemy->m_position, enemy->GetPhysicsRadius(), bullet->m_position, bullet->GetPhysicsRadius()))
	{
		bullet->TakeDamage(99);
		enemy->TakeDamage(bullet->m_damage);
		HandleElasticCollision(enemy, bullet);
		g_theAudio->StartSound(Sound::ENEMY_HIT);
		m_worldCamera->AddTrauma(0.3f);
		m_debrisSystem->SpawnDebrisCluster(BULLET_DEBRIS_NUM, bullet->m_position, -bullet->m_velocity, bullet->GetMainColor(), bullet->GetCosmeticRadius() * DEBRIS_SCATTER_COEFFICIENT, bullet->GetPhysicsRadius() * DEBRIS_AVERAGE_RADIUS_COEFFICIENT);
		if (!IsAlive(enemy))
		{
			g_theAudio->StartSound(Sound::ENEMY_DIE);
			m_debrisSystem->SpawnDebrisCluster(ENEMY_DEBRIS_NUM, enemy->m_position, enemy->m_velocity, enemy->GetMainColor(), enemy->GetCosmeticRadius() * DEBRIS_SCATTER_COEFFICIENT, enemy->GetCosmeticRadius() * DEBRIS_AVERAGE_RADIUS_COEFFICIENT);
		}
	}
}

void Game::CheckEnemiesVsShips()
{
	if (!IsAlive(m_playerShip)) return;

	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex) {
		if (!IsAlive(m_asteroids[asteroidIndex])) continue;
		HandleShipVsEnemy(m_playerShip, m_asteroids[asteroidIndex]);
	}

	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex) {
		if (!IsAlive(m_beetles[beetleIndex])) continue;
		HandleShipVsEnemy(m_playerShip, m_beetles[beetleIndex]);
	}

	for (int waspIndex = 0; waspIndex < MAX_WASPS; ++waspIndex)
	{
		if (!IsAlive(m_wasps[waspIndex])) continue;
		HandleShipVsEnemy(m_playerShip, m_wasps[waspIndex]);
	}

}

void Game::HandleShipVsEnemy(PlayerShip* ship, Entity* enemy)
{
	if (DoDiscsOverlap(enemy->m_position, enemy->GetPhysicsRadius(), ship->m_position, ship->GetPhysicsRadius()))
	{
		ship->TakeDamage(enemy->m_damage);
		enemy->TakeDamage(ship->m_damage);
		HandleElasticCollision(enemy, ship);
		g_theAudio->StartSound(Sound::PLAYER_HIT);
		m_worldCamera->AddTrauma(0.7f);
		if (!IsAlive(ship))
		{
			g_theAudio->StartSound(Sound::PLAYER_DIE);
			m_worldCamera->AddTrauma(1.0f);
			m_debrisSystem->SpawnDebrisCluster(ENEMY_DEBRIS_NUM, ship->m_position, ship->m_velocity, ship->GetMainColor(), ship->GetCosmeticRadius() * DEBRIS_SCATTER_COEFFICIENT, ship->GetCosmeticRadius() * DEBRIS_AVERAGE_RADIUS_COEFFICIENT);
		}
		if (!IsAlive(enemy))
		{
			g_theAudio->StartSound(Sound::ENEMY_DIE);
			m_debrisSystem->SpawnDebrisCluster(ENEMY_DEBRIS_NUM, enemy->m_position, enemy->m_velocity, enemy->GetMainColor(), enemy->GetCosmeticRadius() * DEBRIS_SCATTER_COEFFICIENT, enemy->GetCosmeticRadius() * DEBRIS_AVERAGE_RADIUS_COEFFICIENT);
		}
	}
}

void Game::PushOffEnemies()
{
	PushOffSameTypeEnemy(MAX_ASTEROIDS, m_asteroids);
	PushOffSameTypeEnemy(MAX_BEETLES, m_beetles);
	PushOffSameTypeEnemy(MAX_WASPS, m_wasps);

	PushOffDifferentTypeEnemies(MAX_ASTEROIDS, m_asteroids, MAX_BEETLES, m_beetles);
	PushOffDifferentTypeEnemies(MAX_BEETLES, m_beetles, MAX_WASPS, m_wasps);
	PushOffDifferentTypeEnemies(MAX_WASPS, m_wasps, MAX_ASTEROIDS, m_asteroids);
}

void Game::PushOffSameTypeEnemy(int listMaxSize, Entity** entityList)
{
	for (int i = 0; i < listMaxSize; ++i)
	{
		if (!IsAlive(entityList[i])) continue;
		for (int j = i + 1; j < listMaxSize; ++j)
		{
			if (!IsAlive(entityList[j])) continue;
			HandleElasticCollision(entityList[i], entityList[j]);
		}
	}
}

void Game::PushOffDifferentTypeEnemies(int listMaxSizeA, Entity** entityListA, int listMaxSizeB, Entity** entityListB)
{
	for (int indexA = 0; indexA < listMaxSizeA; ++indexA)
	{
		if (!IsAlive(entityListA[indexA])) continue;
		for (int indexB = 0; indexB < listMaxSizeB; ++indexB)
		{
			if (!IsAlive(entityListB[indexB])) continue;
			HandleElasticCollision(entityListA[indexA], entityListB[indexB]);
		}
	}
}

void Game::HandleElasticCollision(Entity* a, Entity* b)
{
	bool pushed = PushDiscsOutOfEachOther2D(a->m_position, a->m_physicsRadius, b->m_position, b->m_physicsRadius);
	if (!pushed)
	{
		return;
	}
	float dotProduct = DotProduct2D(a->m_velocity - b->m_velocity, a->m_position - b->m_position);
	float invSquaredDistance = 1.f / (a->m_position - b->m_position).GetLengthSquared();
	Vec2 newVa = a->m_velocity - ((2.f * b->m_mass / (a->m_mass + b->m_mass)) * dotProduct * invSquaredDistance) * (a->m_position - b->m_position);
	Vec2 newVb = b->m_velocity - ((2.f * a->m_mass / (a->m_mass + b->m_mass)) * dotProduct * invSquaredDistance) * (b->m_position - a->m_position);

	a->m_velocity = newVa;
	b->m_velocity = newVb;
}

//-----------------------------------------------------------------------------------------------
void Game::GenerateEnemyWave()
{

	//  Check if all enemies are died
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex) {
		if (IsAlive(m_asteroids[asteroidIndex])) return;
	}

	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex) {
		if (IsAlive(m_beetles[beetleIndex])) return;
	}

	for (int waspIndex = 0; waspIndex < MAX_WASPS; ++waspIndex)
	{
		if (IsAlive(m_wasps[waspIndex])) return;
	}

	// All enemy waves have been generated
	if (m_currentWave >= NUM_WAVES) 
	{
		// TODO Victory sound
		g_theAudio->StartSound(Sound::GAME_WIN);
		m_isGameOver = true;
		return;
	}

	// Generate enemy waves
	for (int i = 0; i < m_waves[m_currentWave].m_numNewAsteroid; ++i)
	{
		GenerateAsteroid();
	}
	for (int i = 0; i < m_waves[m_currentWave].m_numNewBeetles; ++i)
	{
		GenerateBeetle();
	}
	for (int i = 0; i < m_waves[m_currentWave].m_numNewWasps; ++i)
	{
		GenerateWasp();
	}
	g_theAudio->StartSound(Sound::GAME_NEXT_LEVEL);
	m_currentWave++;
}

void Game::CheckGameStatus()
{
	float deltaSeconds = static_cast<float>(m_gameClock->GetDeltaSeconds());
	if (m_isGameOver)
	{
		m_timeAfterGameOver += deltaSeconds;
		if (m_timeAfterGameOver > 3.0f)
		{
			RestartGame();
		}
	}
	else if (m_playerShip->GetExtraLives() <= 0 && !IsAlive(m_playerShip))
	{
		g_theAudio->StartSound(Sound::GAME_LOSE);
		m_isGameOver = true;
	}
}

void Game::RestartGame()
{
	m_shouldGameResetNextFrame = true;
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateAttractiveMode()
{
	//  Attractive Mode Input
	XboxController const& controller = g_theInput->GetController(0);
	if (g_theInput->WasKeyJustPressed(KEYCODE_ESCAPE) || controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_BACK))
	{
		g_theApp->HandleQuitRequested();
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE) ||
		g_theInput->WasKeyJustPressed(KEYCODE_N) ||
		controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_START) ||
		controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_A))
	{
		m_isAttractiveMode = false;
		return;
	}

	// Update Attractive Mode Animation
	
	float time = static_cast<float>(m_gameClock->GetTotalSeconds());
	//float phaseOffsets[3][3] = {
	//	{0.0f, 120.0f, 240.0f},
	//	{60.0f, 180.0f, 300.0f},
	//	{120.0f, 240.0f, 360.0f}
	//};

	float phaseOffset[3] = { 0.f, 60.f, 120.f };
	float frequency = 0.05f;
	for (int i = 0; i < 3; ++i)
	{
		float t = SinDegrees(360.f * frequency * time + phaseOffset[i]);
		float wavelength = RangeMap(t, -1.f, 1.f, 400.f, 700.f);
		m_attractiveColor[i] = Rgba8::MakeFromWaveLength(wavelength);



		//float r = SinDegrees(360.f * frequency * time + phaseOffsets[i][0]);
		//float g = SinDegrees(360.f * frequency * time + phaseOffsets[i][1]);
		//float b = SinDegrees(360.f * frequency * time + phaseOffsets[i][2]);

		//m_attractiveColor[i].r = static_cast<unsigned char>(RangeMap(r, -1.f, 0.f, 0.f, 255.f));
		//m_attractiveColor[i].g = static_cast<unsigned char>(RangeMap(g, -1.f, 0.f, 0.f, 255.f));
		//m_attractiveColor[i].b = static_cast<unsigned char>(RangeMap(b, -1.f, 0.f, 0.f, 255.f));
	}
}

void Game::RenderAttractiveMode() const
{
	PlayerShip::RenderShipIcon(Vec2(SCREEN_SIZE_X * 0.5f - 400.f, SCREEN_SIZE_Y * 0.5f), 0.f, 80.f);
	PlayerShip::RenderShipIcon(Vec2(SCREEN_SIZE_X * 0.5f + 400.f, SCREEN_SIZE_Y * 0.5f), 180.f, 80.f);

	Vertex_PCU startButton[3];
	startButton[0].m_position = Vec3(-1.f, 1.f, 0.f);
	startButton[1].m_position = Vec3(-1.f, -1.f, 0.f);
	startButton[2].m_position = Vec3(1.f, 0.f, 0.f);
	startButton[0].m_color = m_attractiveColor[0];
	startButton[1].m_color = m_attractiveColor[1];
	startButton[2].m_color = m_attractiveColor[2];
	TransformVertexArrayXY3D(3, &startButton[0], 80.f, 0.f, Vec2(SCREEN_SIZE_X * 0.5f, SCREEN_SIZE_Y * 0.5f));
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray(3, &startButton[0]);
}

//-----------------------------------------------------------------------------------------------
void Game::HandleInputKeys()
{
	XboxController const& controller = g_theInput->GetController(0);
	// pause, run one frame, slow time mode 

	//m_gameClock->SetTimeScale((g_theInput->IsKeyDown(KEYCODE_T) || controller.GetLeftTrigger() > 0.f) ? 0.1 : 1.0);
	if (g_theInput->WasKeyJustPressed(KEYCODE_T))
	{
		m_isSlowDown = !m_isSlowDown;
		m_gameClock->SetTimeScale(m_isSlowDown ? 0.1 : 1.0);
	}


	if (g_theInput->WasKeyJustPressed(KEYCODE_P))
	{
		m_gameClock->TogglePause();
	}

	if (g_theInput->WasKeyJustPressed(KEYCODE_O))
	{
		m_gameClock->StepSingleFrame();
	}

	// Press I to add Asteroid
	if (g_theInput->WasKeyJustPressed(KEYCODE_I))
	{
		GenerateAsteroid();
	}

	// Debug Draw Mode
	if (g_theInput->WasKeyJustPressed(KEYCODE_F1))
	{
		g_isDebugDraw = !g_isDebugDraw;
	}

	// Back To Attractive Mode and Reset the Game
	if (g_theInput->WasKeyJustPressed(KEYCODE_ESCAPE) || controller.WasButtonJustPressed(XboxButtonId::XBOX_BUTTON_BACK))
	{
		RestartGame();
	}
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateCameras()
{
	// Update Screen Camera
	m_screenCamera.SetOrthoView(Vec2(0.f, 0.f), Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y));

	// Update World Camera
	m_worldCamera->Update();
}


//-----------------------------------------------------------------------------------------------
bool OnTimeScaleChanged(EventArgs& args)
{
	float timeScale = args.GetValue("scale", -1.f);
	if (timeScale <= 0.f)
	{
		g_theDevConsole->AddText(DevConsole::ERROR, "Wrong Syntax or scale is not positive! example: TimeSpeed scale=0.2");
	}
	else
	{
		g_theApp->m_theGame->m_gameClock->SetTimeScale(timeScale);
		g_theDevConsole->AddText(DevConsole::INFO_MINOR, Stringf("Time Scale has been set as %f", timeScale));
	}
	return true;
}
