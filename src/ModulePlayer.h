#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SimpleTimer.h"

struct SDL_Texture;

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
public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation waiting;
	Animation walk;
	Animation respawning;
	SimpleTimer playerTimer;
	iPoint position = { 0, 0 };
	unsigned short int depth = 20;
	unsigned short int lives = 4;
	bool destroyed = true;
	bool isRespawning = false;
};

#endif // __ModulePlayer_H__