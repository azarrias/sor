#ifndef __ModuleStageTwo_H__
#define __ModuleStageTwo_H__

#include "Module.h"

struct SDL_Texture;

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
	SDL_Texture* stars = nullptr;
};

#endif // __ModuleStageTwo_H__