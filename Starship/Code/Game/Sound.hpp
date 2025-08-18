#pragma once

#include "Engine/Audio/AudioSystem.hpp"

class Sound
{
public:
	static SoundID BACKGROUND_MUSIC;
	static SoundID BOUTTON_TAP;
	static SoundID GAME_WIN;
	static SoundID GAME_LOSE;
	static SoundID GAME_NEXT_LEVEL;
	static SoundID PLAYER_SHOOT;
	static SoundID PLAYER_HIT;
	static SoundID PLAYER_DIE;
	static SoundID PLAYER_LOW_HEALTH;
	static SoundID PLAYER_ENERGY_RECOVER;
	static SoundID ENEMY_SHOOT;
	static SoundID ENEMY_HIT;
	static SoundID ENEMY_DIE;

public:
	static void Init();
};

