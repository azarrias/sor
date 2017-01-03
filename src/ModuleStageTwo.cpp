#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
//#include "ModuleParticles.h"
#include "ModuleStageTwo.h"
#include "ModuleHUD.h"
#include "ModuleEntityManager.h"
#include "Player.h"

// Reference at https://www.youtube.com/watch?v=J9gWS0ZQaxE

ModuleStageTwo::ModuleStageTwo(bool active) 
	: Module(active), stageTimer()
{}

ModuleStageTwo::~ModuleStageTwo()
{}

// Load assets
bool ModuleStageTwo::Start()
{
	LOG("Loading stage");

	background = App->textures->Load("graphics/stage2_bg.png");
	intro = App->textures->Load("graphics/hud.png");

	App->hud->Enable();
	App->entities->player = (Player*)(App->entities->createEntity(Entity::Types::PLAYER));
	App->entities->Enable();
//	App->particles->Enable();
	App->collision->Enable();

	App->audio->PlayMusic("audio/stage2.ogg", 1.0f);

	// TODO 15: create some colliders for the walls
	// solution wall coords: {0, 224, 3930, 16} {1375, 0, 111, 96} {1375, 145, 111, 96}

	return true;
}

// UnLoad assets
bool ModuleStageTwo::CleanUp()
{
	LOG("Unloading stage");

	App->textures->Unload(background);
	App->entities->Disable();
	App->hud->Disable();
	App->collision->Disable();
//	App->particles->Disable();

	return true;
}

update_status ModuleStageTwo::Update()
{
	// Move camera forward -----------------------------
	//int scroll_speed = 1;

	//App->player->position.x += 1;
	//App->renderer->camera.x -= 3;

	// Draw everything --------------------------------------
	SDL_Rect round;
	round.x = 0;
	round.y = 50;
	round.w = 76;
	round.h = 14;

	//stageTimer.update();

	switch (stageState)
	{
	case INTRO:
		App->renderer->Blit(intro, 70, 100, &round);
		if (stageTimer.getDelta() >= 6000) {
			stageState = LEVEL;
			stageTimer.reset();
		}
		break;
	case LEVEL:
		App->renderer->Blit(background, 0, 0, NULL);
		if (App->entities->player->status == DEAD) {
			if (App->entities->player->lives > 0) {
				App->entities->player->respawn();
			}
			else {
				// TODO : You lose
			}
		}
	}

	return UPDATE_CONTINUE;
}