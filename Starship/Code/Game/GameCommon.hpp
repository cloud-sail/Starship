#pragma once

// Forward declaration
class App;
class AudioSystem;
class InputSystem;
class Renderer;
class Window;
class Game;

struct Vec2;
struct Rgba8;

//-----------------------------------------------------------------------------------------------
// Global Pointers
extern App* g_theApp;
extern AudioSystem* g_theAudio;
extern InputSystem* g_theInput;
extern Renderer* g_theRenderer;
extern Window* g_theWindow;
extern bool g_isDebugDraw;

//-----------------------------------------------------------------------------------------------
// Constants
constexpr float CAMERA_SIZE_X = 200.f;
constexpr float CAMERA_SIZE_Y = 100.f;
constexpr int	DEBUG_GRID_NUM_X = 50;
constexpr int	DEBUG_GRID_NUM_Y = 50;
constexpr float WORLD_SIZE_X = 500.f;
constexpr float WORLD_SIZE_Y = 500.f;
constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;
constexpr float SCREEN_SIZE_X = 1600.f;
constexpr float SCREEN_SIZE_Y = 800.f;


//-----------------------------------------------------------------------------------------------
constexpr int NUM_STARTING_ASTEROIDS = 6;
constexpr int MAX_ASTEROIDS = 12;
constexpr int MAX_BULLETS = 20;
constexpr int MAX_BEETLES = 12;
constexpr int MAX_WASPS = 12;
constexpr int MAX_DEBRIS = 200;

constexpr int   ASTEROID_MAX_HEALTH = 3;
constexpr float ASTEROID_SPEED = 10.f;
constexpr float ASTEROID_PHYSICS_RADIUS = 1.6f;
constexpr float ASTEROID_COSMETIC_RADIUS = 2.0f;

constexpr float BULLET_LIFETIME_SECONDS = 2.0f;
constexpr float BULLET_SPEED = 50.f;
constexpr float BULLET_PHYSICS_RADIUS = 0.5f;
constexpr float BULLET_COSMETIC_RADIUS = 2.0f;
constexpr float BULLET_MASS = 0.1f;

constexpr int	PLAYER_MAX_HEALTH = 5;
constexpr float PLAYER_MAX_SPEED = 40.f;
constexpr float PLAYER_SHIP_ACCELERATION = 30.f;
constexpr float PLAYER_SHIP_TURN_SPEED = 300.f;
constexpr float PLAYER_SHIP_PHYSICS_RADIUS = 1.75f;
constexpr float PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;

constexpr int	BEETLE_MAX_HEALTH = 5;
constexpr float BEETLE_ACCELERATION = 5.f;
//constexpr float BEETLE_SPEED = 10.f;
constexpr float BEETLE_MAX_SPEED = 20.f;
constexpr float BEETLE_PHYSICS_RADIUS = 1.414f;
constexpr float BEETLE_COSMETIC_RADIUS = 2.0f;
constexpr float BEETLE_IDLE_RADIUS = 35.0f;
constexpr float BEETLE_MASS = 10.f;

constexpr int	WASP_MAX_HEALTH = 2;
constexpr float WASP_ACCELERATION = 15.f;
constexpr float WASP_MAX_SPEED = 30.f;
constexpr float WASP_PHYSICS_RADIUS = 1.f;
constexpr float WASP_COSMETIC_RADIUS = 2.0f;
constexpr float WASP_IDLE_RADIUS = 10.0f;
constexpr float WASP_MASS = 0.5f;


constexpr int BULLET_DEBRIS_NUM = 3;
constexpr int ENEMY_DEBRIS_NUM = 9;
constexpr int SHIP_DEBRIS_NUM = 9;

constexpr float DEBRIS_SCATTER_COEFFICIENT = 10.f;
constexpr float DEBRIS_AVERAGE_RADIUS_COEFFICIENT = 0.5f;

//-----------------------------------------------------------------------------------------------

void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);

void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);

// Simple transformation calculation
Vec2 GetRelatedTranslation(Vec2 const& parentWorldPos, float parentWorldDegrees, Vec2 const& childWorldPos);
float GetRelatedOrientation(float parentWorldDegrees, float childWorldDegrees);