#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include <stdio.h>
#include "MemLeaks.h"

ModulePlayer::ModulePlayer(bool active) 
	: Module(active), refreshTimer(), playerTimer()
{
	// Coordinates for Blaze

	// idle animation
	for (size_t i=0; i < 8; ++i)
		idle.frames.push_back({ 7, 956, 49, 61 });
	idle.frames.push_back({ 63, 956, 49, 61 });
	idle.frames.push_back({ 126, 956, 49, 61 });
	idle.speed = 0.05f;

	// respawning animation
	respawning.frames.push_back({ 48, 1104, 43, 65 });
	respawning.frames.push_back({ 6, 1104, 42, 65 });
	respawning.loop = false;
	respawning.speed = 0.0f;

	// walk animation
	walk.frames.push_back({ 306, 956, 36, 61 });
	walk.frames.push_back({ 344, 956, 41, 61 });
	walk.frames.push_back({ 386, 956, 36, 61 });
	walk.frames.push_back({ 424, 956, 41, 61 });
	walk.speed = 0.1f;

	// jump animation
	jump.frames.push_back({ 6, 1104, 42, 65 });
	jump.frames.push_back({ 48, 1104, 43, 65 });
	jump.loop = false;
	jump.speed = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("graphics/pcs.png");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Respawn player
void ModulePlayer::respawn()
{
	LOG("Respawning player");
	position.x = 23;
	position.y = -30;
	status = RESPAWNING;
	playerTimer.reset();
	--lives;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (refreshTimer.getDelta() >= 5) {

		refreshTimer.reset();

		switch (status) {
		case RESPAWNING:
			setCurrentAnimation(&respawning);
			if (isOnTheAir())
				velocity.y += 0.3f;
			else {
				velocity.y = 0.0f;
				respawning.speed = 1.0f;
				if (playerTimer.getDelta() >= 100)
					status = DEFAULT;
			}
			break;

		case DEFAULT:
			if (movementTimer.getDelta() >= 60) {
				movementTimer.reset();
				while (App->input->keyEventQueue.empty() == false)
				{
					KeyEvent* keyEvent = App->input->keyEventQueue.front();

					switch (keyEvent->status)
					{
					case IS_DOWN:
						switch (keyEvent->key)
						{
						case KEY_UP: velocity.y -= 1.0f; break;
						case KEY_DOWN: velocity.y += 1.0f; break;
						case KEY_LEFT: velocity.x -= 1.0f; break;
						case KEY_RIGHT: velocity.x += 1.0f; break;
						}
						break;
					case IS_UP:
						switch (keyEvent->key)
						{
						case KEY_UP: velocity.y += 1.0f; break;
						case KEY_DOWN: velocity.y -= 1.0f; break;
						case KEY_LEFT: velocity.x += 1.0f; break;
						case KEY_RIGHT: velocity.x -= 1.0f; break;
						}
						break;
					}

					App->input->keyEventQueue.pop_front();
					RELEASE(keyEvent);
				}
			}
			if (velocity.y == 0 && velocity.x == 0)
				setCurrentAnimation(&idle);
			else setCurrentAnimation(&walk);

		}

		updatePosition();
		if (status != DEAD)
			App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	}

	// debug player
	char integer_string[32];
	LOG("******");
	LOG("Status: (0-DEFAULT, 1-RESPAWNING, 2-JUMPING, 3-DEAD)");
	sprintf_s(integer_string, "%d", status);
	LOG(integer_string);
	LOG("Manual frame index:");
	sprintf_s(integer_string, "%d", manualFrameIndex);
	LOG(integer_string);
	LOG("Depth:");
	sprintf_s(integer_string, "%d", depth);
	LOG(integer_string);
	LOG("PosY:");
	sprintf_s(integer_string, "%d", position.y);
	LOG(integer_string);

	return UPDATE_CONTINUE;
}

void ModulePlayer::setCurrentAnimation(Animation* anim) {
	if (current_animation != anim)
	{
		anim->Reset();
		current_animation = anim;
	}
}

void ModulePlayer::updatePosition() {
	position.x += (int)velocity.x;
	if (position.x < 0) 
		position.x = 0;
	if (!isOnTheAir())
		depth -= (int)velocity.y;
	position.y += (int)velocity.y;
	if (depth > 53) {
		position.y = 102;
		depth = 53;
	}
	if (depth < 0) {
		position.y = 155;
		depth = 0;
	} 
}

bool ModulePlayer::isOnTheAir() const {
	return (position.y < 155 - depth);
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
