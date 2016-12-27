#ifndef __ModuleSceneIntro_H__
#define __ModuleSceneIntro_H__

#include "Module.h"
#include "Animation.h"
#include "SimpleTimer.h"

struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool active = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* background = nullptr;
	uint fx = 0;
	Animation title;
	Animation subtitle;
	bool animateTitle = false;
	SimpleTimer introTimer;
	unsigned short int introState = 0;
	short int subtitlePosx = -50;
};

#endif // __ModuleSceneIntro_H__