#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SimpleTimer.h"

struct SDL_Texture;

enum PlayerState
{
	DEFAULT,
	RESPAWNING,
	JUMPING,
	DEAD,
	UNAVAILABLE
};

enum JumpState
{
	INITIAL,
	GOING_UP,
	TOP_POS,
	GOING_DOWN,
	LAND,
	NOT_JUMPING
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void respawn();
	void setCurrentAnimation(Animation* anim);
	void updatePosition();
	bool isOnTheAir() const;
public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation waiting;
	Animation walk;
	Animation jump;
	Animation respawning;
	SimpleTimer playerTimer, refreshTimer, movementTimer;
	iPoint position = { 23, -30 };
	fPoint velocity = { 0.0f, 0.0f };
	short int depth = 23;
	unsigned short int lives = 4;
	unsigned short int manualFrameIndex = 0;
	PlayerState status = DEAD;
};

#endif // __ModulePlayer_H__