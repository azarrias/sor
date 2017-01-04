#ifndef __Player_H__
#define __Player_H__

#include "Module.h"
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
	//void updatePosition();
	void handleInput();
public:
	unsigned short int lives = 4;
};

#endif // __Player_H__