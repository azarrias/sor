#ifndef __ModuleHUD_H__
#define __ModuleHUD_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;

class ModuleHUD : public Module
{
public:
	ModuleHUD(bool active = true);
	~ModuleHUD();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	iPoint position;
	SDL_Rect hudRect = { 0, 0, 320, 32 };
	unsigned short int time = 40;
};

#endif // __ModuleHUD_H__