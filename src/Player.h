#ifndef __Player_H__
#define __Player_H__

#include "Module.h"
#include "SimpleTimer.h"
#include "Creature.h"

class Player : public Creature
{
public:
	Player();
	~Player();

	bool Start();
	update_status Update();
	bool CleanUp();
	void respawn();
	void jump();
	void attack();
	void setCurrentAnimation(Animation* anim);
	void updatePosition();
	void handleInput();
	void handleState();
public:
	//SDL_Texture* graphics = nullptr;
	SimpleTimer playerTimer, refreshTimer, movementTimer, jumpTimer, attackTimer;
	unsigned short int lives = 4;
};

#endif // __Player_H__