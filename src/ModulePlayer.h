#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SimpleTimer.h"

struct SDL_Texture;

enum PlayerState
{
	RESPAWNING,
	IDLE,
	WALK,
	JUMP_INI,
	JUMPING,
	JUMP_END,
	ATTACK_JMP,
	ATTACKING,
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
	void jump();
	void attack();
	void setCurrentAnimation(Animation* anim);
	void updatePosition();
	bool isOnTheAir() const;
public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation waiting;
	Animation walk;
	Animation jumping;
	Animation jumpKick;
	Animation chop;
	Animation respawning;
	SimpleTimer playerTimer, refreshTimer, movementTimer, jumpTimer, attackTimer;
	iPoint position = { 23, -30 };
	fPoint velocity = { 0.0f, 0.0f };
	fPoint prevVelocity = { 0.0f, 0.0f };
	short int height = 0;
	float verticalForce = 0.0f;
	short int depth = 23;
	unsigned short int lives = 4;
	PlayerState status = DEAD;
};

#endif // __ModulePlayer_H__