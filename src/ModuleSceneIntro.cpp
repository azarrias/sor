#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"

// Reference at https://www.youtube.com/watch?v=MXjaCkPWpvU

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading game intro");

	background = App->textures->Load("graphics/intro.png");

	App->audio->PlayMusic("audio/intro.ogg", 1.0f);
	if (fx == 0)
		fx = App->audio->LoadFx("audio/starting.wav");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	return true;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading game intro");

	App->textures->Unload(background);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	SDL_Rect rect;
	rect.x = 310;
	rect.y = 4;
	rect.w = 320;
	rect.h = 240;


	App->renderer->Blit(background, 0, 0, &rect);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->scene_stage, this);
		App->audio->PlayFx(fx);
	}

	return UPDATE_CONTINUE;
}