#ifndef __ModuleStageTwo_H__
#define __ModuleStageTwo_H__

#include "Module.h"
#include "SimpleTimer.h"
#include <vector>
#include "Point.h"

struct SDL_Texture;

enum StageState
{
	INTRO,
	LEVEL,
	BOSS
};

class ModuleStageTwo : public Module
{
public:
	ModuleStageTwo(bool active = true);
	~ModuleStageTwo();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* background = nullptr;
	SDL_Texture* intro = nullptr;
	SDL_Texture* stars = nullptr;
	StageState stageState = INTRO;
	SimpleTimer stageTimer;
};

#endif // __ModuleStageTwo_H__