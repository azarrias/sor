#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(bool active) : Module(active), playerTimer(800)
{
	// Coordinates for Blaze

	// idle animation
	idle.frames.push_back({ 0, 956, 56, 61 });

	// waiting animation
	waiting.frames.push_back({ 0, 956, 56, 61 });
	waiting.frames.push_back({ 56, 956, 56, 61 });
	waiting.frames.push_back({ 119, 956, 56, 61 });
	waiting.speed = 0.05f;

	// respawning animation
	respawning.frames.push_back({ 48, 1104, 43, 65 });
	respawning.frames.push_back({ 6, 1104, 42, 65 });
	respawning.loop = false;
	respawning.speed = 0.0f;

	// move upwards
	up.frames.push_back({ 100, 1, 32, 14 });
	up.frames.push_back({ 132, 0, 32, 14 });
	up.loop = false;
	up.speed = 0.1f;

	// Move down
	down.frames.push_back({ 33, 1, 32, 14 });
	down.frames.push_back({ 0, 1, 32, 14 });
	down.loop = false;
	down.speed = 0.1f;
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
	position.x = 16;
	position.y = -80;
	isRespawning = true;
	playerTimer.reset();
	--lives;
	destroyed = false;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;

	if (isRespawning)
	{
		current_animation = &respawning;
		if (position.y >= 132) {
			playerTimer.update();
			position.y = 132;
			respawning.speed = 1.0f;
			if (playerTimer.check()) {
				isRespawning = false;
			}
		}
		else {
			position.y += 5;
		}
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += speed;
			if (current_animation != &down)
			{
				down.Reset();
				current_animation = &down;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			position.y -= speed;
			if (current_animation != &up)
			{
				up.Reset();
				current_animation = &up;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			// TODO 6: Shoot a laser using the particle system
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			current_animation = &idle;
	}

	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
