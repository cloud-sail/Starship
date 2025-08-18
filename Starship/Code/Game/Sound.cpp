#include "Game/Sound.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Math/Vec3.hpp"


SoundID Sound::BACKGROUND_MUSIC		= MISSING_SOUND_ID;
SoundID Sound::BOUTTON_TAP			= MISSING_SOUND_ID;
SoundID Sound::GAME_WIN				= MISSING_SOUND_ID;
SoundID Sound::GAME_LOSE				= MISSING_SOUND_ID;
SoundID Sound::GAME_NEXT_LEVEL		= MISSING_SOUND_ID;
SoundID Sound::PLAYER_SHOOT			= MISSING_SOUND_ID;
SoundID Sound::PLAYER_HIT				= MISSING_SOUND_ID;
SoundID Sound::PLAYER_DIE				= MISSING_SOUND_ID;
SoundID Sound::PLAYER_LOW_HEALTH		= MISSING_SOUND_ID;
SoundID Sound::PLAYER_ENERGY_RECOVER	= MISSING_SOUND_ID;
SoundID Sound::ENEMY_SHOOT			= MISSING_SOUND_ID;
SoundID Sound::ENEMY_HIT				= MISSING_SOUND_ID;
SoundID Sound::ENEMY_DIE				= MISSING_SOUND_ID;

void Sound::Init()
{
	BACKGROUND_MUSIC		= g_theAudio->CreateOrGetSound("Data/Audio/BGM.wav");
	BOUTTON_TAP				= g_theAudio->CreateOrGetSound("Data/Audio/ButtonTap.wav");
	GAME_WIN				= g_theAudio->CreateOrGetSound("Data/Audio/GameWin.wav");
	GAME_LOSE				= g_theAudio->CreateOrGetSound("Data/Audio/GameLose.wav");
	GAME_NEXT_LEVEL			= g_theAudio->CreateOrGetSound("Data/Audio/GameNextLevel.wav");
	PLAYER_SHOOT			= g_theAudio->CreateOrGetSound("Data/Audio/PlayerLaserShoot.wav");
	PLAYER_HIT				= g_theAudio->CreateOrGetSound("Data/Audio/PlayerHit.wav");
	PLAYER_DIE				= g_theAudio->CreateOrGetSound("Data/Audio/PlayerDie.wav");
	PLAYER_LOW_HEALTH		= g_theAudio->CreateOrGetSound("Data/Audio/LowHealth.wav");
	PLAYER_ENERGY_RECOVER	= g_theAudio->CreateOrGetSound("Data/Audio/EnergyRecover.wav");
	ENEMY_SHOOT				= g_theAudio->CreateOrGetSound("Data/Audio/EnemyShoot.wav");
	ENEMY_HIT				= g_theAudio->CreateOrGetSound("Data/Audio/EnemyHit.wav");
	ENEMY_DIE				= g_theAudio->CreateOrGetSound("Data/Audio/EnemyDie.wav");

	// Consider use balance not 3D sound
	// g_theAudio->SetListener(Vec3(100.f, 50.f, 10.f), Vec3(0.f, 0.f, -1.f), Vec3(0.f, 1.f, 0.f));
}

