#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEntityManager.h"
#include "Player.h"

ModuleCamera::ModuleCamera()
{
	coord.x = coord.y = 0;
	coord.w = SCREEN_WIDTH * SCREEN_SIZE;
	coord.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleCamera::~ModuleCamera()
{}

// Called every draw update
update_status ModuleCamera::Update()
{
	if (App->entities->player != nullptr &&
		App->entities->player->position.x > (int)(-coord.x + SCREEN_WIDTH * 0.6)) {
		coord.x -= (camSpeed * SCREEN_SIZE);
	}

	return UPDATE_CONTINUE;
}
