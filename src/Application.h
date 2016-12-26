#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
//class ModuleParticles;

// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
class ModuleStageTwo;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
//	ModuleParticles* particles;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleStageTwo* scene_stage;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __Application_H__