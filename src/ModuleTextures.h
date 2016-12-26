#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include <list>
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};

#endif // __ModuleTextures_H__