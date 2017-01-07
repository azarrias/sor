#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"

struct SDL_Rect;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	update_status Update();

public:
	SDL_Rect coord;
	float camSpeed = 0.99f;
};

#endif // __ModuleCamera_H__