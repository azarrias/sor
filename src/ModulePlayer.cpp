#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(bool active) : Module(active), playerTimer(500)
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
	velocity.y = 0.0f;
	playerTimer.reset();
	--lives;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//int speed = 1;
	unsigned short int frameIndex = 0;

	switch (status) {
	case RESPAWNING:
		setCurrentAnimation(&respawning);
		if (position.y >= 132) {
			position.y = 132;
			velocity.y = 0.0f;
			playerTimer.update();
			respawning.speed = 1.0f;
			if (playerTimer.check())
				status = DEFAULT;
		}
		else velocity.y += 0.3f;
		position.y += (int)velocity.y;
		break;
	case JUMPING:
		if (manualFrameIndex == 1) {	// player is on the air
			velocity.y += 0.5f;
			position.y += (int)velocity.y;
		}
		if (position.y == 155 - depth) { // player is on the ground
			playerTimer.update();
			if (playerTimer.check()) {
				if (velocity.y > 0) {	// is landing
					velocity.y = 0;
					manualFrameIndex = 0;
					playerTimer.reset();
				}
				else if (velocity.y == 0) // just landed
					status = DEFAULT;
				else manualFrameIndex = 1; // is about to jump
			}
		}
		App->renderer->Blit(graphics, position.x, position.y, &(jump.frames[manualFrameIndex]));
		break;
	case DEFAULT:
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			velocity.x = -1.0f;
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			velocity.x = 1.0f;
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			velocity.y = 1.0f;
			updateDepth();
			setCurrentAnimation(&walk);
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			velocity.y = -1.0f;
			updateDepth();
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
			status = JUMPING;
			playerTimer.interval = 120;
			velocity.y = -8.5f;
			position.y += (int)8.5f;
			playerTimer.reset();
			setCurrentAnimation(&jump);
			manualFrameIndex = 0;
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
			&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			velocity.x = 0.0f;
			velocity.y = 0.0f;
			setCurrentAnimation(&idle);
		}

		updatePosition();
	}

	// Draw everything --------------------------------------
	if (status != DEAD && status != JUMPING)
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

void ModulePlayer::updatePosition() {
	if (position.x + (int)velocity.x > 0)
		position.x += (int)velocity.x;
	if (position.y + (int)velocity.y >= 102 && position.y + (int)velocity.y <= 155)
		position.y += (int)velocity.y;
}

void ModulePlayer::updateDepth() {
	if (depth - (int)velocity.y >= 0 && depth - (int)velocity.y <= 53)
		depth -= (int)velocity.y;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
