#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModulePlayer.h"
#include <stdio.h>

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
	if (App->player->position.x > (int)(-coord.x + SCREEN_WIDTH * 0.6))
	{
		coord.x -= camSpeed;
	}

	char integer_string[32];

	// debug camera
	LOG("******");
	LOG("Player posx:");
	sprintf_s(integer_string, "%d", App->player->position.x);
	LOG(integer_string);
	LOG("Camera posx:");
	sprintf_s(integer_string, "%d", coord.x);
	LOG(integer_string);

	return UPDATE_CONTINUE;
}
