#ifndef __Player_H__
#define __Player_H__

#include "Module.h"
#include "Creature.h"

class Player : public Creature
{
public:
	Player();
	~Player();

	bool LoadConfigFromJSON(const char* fileName);
	bool Start();
	update_status Update();
	bool CleanUp();
	void spawn();
	void handleInput();
	void hit(Creature* c2);
	void die();
public:
	unsigned short int lives = 4;
private:
	uint soundFxHit = 0;
	uint soundFxAttack = 0;
	uint soundFxDie = 0;
};

#endif // __Player_H__