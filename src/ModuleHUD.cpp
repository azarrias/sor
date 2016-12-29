#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleHUD.h"

ModuleHUD::ModuleHUD(bool active) 
	: Module(active)
{}

ModuleHUD::~ModuleHUD()
{}

// Load assets
bool ModuleHUD::Start()
{
	LOG("Loading HUD");
	graphics = App->textures->Load("graphics/hud.png");
	return true;
}

// Unload assets
bool ModuleHUD::CleanUp()
{
	LOG("Unloading HUD");
	App->textures->Unload(graphics);
	return true;
}

// Update: draw background
update_status ModuleHUD::Update()
{
	App->renderer->Blit(graphics, position.x - App->camera->coord.x, position.y, &hudRect);
	return UPDATE_CONTINUE;
}
