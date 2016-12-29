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
		setCurrentAnimation(&respawning);
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
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (position.x - speed >= 0)
				position.x -= speed;
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			position.x += speed;
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (depth - speed >= 0)
			{
				depth -= speed;
				position.y += speed;
			}
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (depth + speed <= 53)
			{
				depth += speed;
				position.y -= speed;
			}
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			// TODO: Special attack
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			// TODO: Normal attack
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			// TODO: Jump
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			// TODO: Pause the game
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			setCurrentAnimation(&idle);
	}

	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}

void ModulePlayer::setCurrentAnimation(Animation* anim) {
	if (current_animation != anim)
	{
		anim->Reset();
		current_animation = anim;
	}
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
