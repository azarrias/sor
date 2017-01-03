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
	jumping.frames.push_back({ 8, 1108, 42, 61 });
	jumping.frames.push_back({ 48, 1104, 43, 65 });
	jumping.loop = false;
	jumping.speed = 0.0f;

	// jump kick
	jumpKick.frames.push_back({ 88, 1114, 42, 55 });
	jumpKick.frames.push_back({ 134, 1104, 43, 65 });
	jumpKick.frames.push_back({ 178, 1114, 70, 55 });
	jumpKick.loop = false;
	jumpKick.speed = 0.5f;

	// chop
	chop.frames.push_back({ 87, 1028, 47, 61 });
	chop.frames.push_back({ 7, 1028, 70, 61 });
	chop.frames.push_back({ 87, 1028, 47, 61 });
	chop.loop = false;
	chop.speed = 0.2f;
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
	height = 162;
	status = RESPAWNING;
	playerTimer.reset();
	--lives;
}

// Jump
void ModulePlayer::jump()
{
	LOG("Player jumps");
	status = JUMPING;
	setCurrentAnimation(&jumping);
	jumpTimer.reset();
}

// Attack
void ModulePlayer::attack()
{
	LOG("Player attacks");
	status = ATTACKING;
	if (isOnTheAir()) {
		setCurrentAnimation(&jumpKick);
		status = JUMPING;
	}
	else {
		//velocity.x = 0;
		velocity.y = 0.0f;
		setCurrentAnimation(&chop);
	}
	attackTimer.reset();
}

// Update: draw background
update_status ModulePlayer::Update()
{
	KeyEvent* keyEvent = nullptr;

	if (App->input->keyEventQueue.empty() == false) {
		keyEvent = App->input->keyEventQueue.front();
	}

	if (keyEvent != nullptr) {
		switch (keyEvent->key)
		{
		case KEY_UP:
			if(keyEvent->status == IS_DOWN)
				velocity.y -= 1.0f;
			else if(keyEvent->status == IS_UP)
				velocity.y += 1.0f;
			break;
		case KEY_DOWN:
			if (keyEvent->status == IS_DOWN)
				velocity.y += 1.0f;
			else if (keyEvent->status == IS_UP)
				velocity.y -= 1.0f;
			break;
		case KEY_LEFT:
			if (keyEvent->status == IS_DOWN)
				velocity.x -= 2.0f;
			else if (keyEvent->status == IS_UP)
				velocity.x += 2.0f;
			break;
		case KEY_RIGHT:
			if (keyEvent->status == IS_DOWN)
				velocity.x += 2.0f;
			else if (keyEvent->status == IS_UP)
				velocity.x -= 2.0f;
			break;
		case KEY_D:
			if (status == IDLE || status == WALK) {
				jumpTimer.reset();
				status = JUMP_INI;
				setCurrentAnimation(&jumping);
			}
			break;
		}
	}

	switch (status) {
	case RESPAWNING:
		setCurrentAnimation(&respawning);
		if (height > 0)
			verticalForce += 0.3f;
		else {
//			velocity.y = 0.0f;
			respawning.speed = 1.0f;
			if (playerTimer.getDelta() >= 100) {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case IDLE:

		if (velocity.x != 0.0f || velocity.y != 0.0f) {
			status = WALK;
			setCurrentAnimation(&walk);
		}
		break;

/*		if (keyEvent != nullptr && keyEvent->status == IS_DOWN) {
			switch (keyEvent->key) {
				case KEY_UP:	
					status = WALK;
					setCurrentAnimation(&walk);
					break;
				case KEY_DOWN:	
					status = WALK;
					setCurrentAnimation(&walk);
					break;
				case KEY_LEFT:	
					status = WALK;
					setCurrentAnimation(&walk);
					break;
				case KEY_RIGHT:	
					status = WALK;
					setCurrentAnimation(&walk);
					break;
				case KEY_D:
					jumpTimer.reset();
					status = JUMP_INI;
					setCurrentAnimation(&jumping);
					break;
				}
			}
		break;*/

	case WALK:
			
		if (velocity.x == 0.0f && velocity.y == 0.0f) {
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		break;

	case JUMP_INI:

		if (jumpTimer.getDelta() >= 100) {
			verticalForce = -8.5f;
			jumping.speed = 1.0f;
			height = 0;
			status = JUMPING;
		}
		break;

	case JUMPING:
		if (height > 0) {
			verticalForce += 0.5f;
		}
		else {
			jumping.speed = 0.0f;
			jumping.Reset();
			//verticalForce = 0.0f;
			prevVelocity = velocity;
			velocity.x = 0.0f;
			status = JUMP_END;
			jumpTimer.reset();
		}
		break;

	case ATTACK_JMP:
/*		if (keyEvent != nullptr && keyEvent->status == IS_DOWN) {
			switch (keyEvent->key) {
			case KEY_LEFT:
				velocity.x -= 2.0f;
				break;
			case KEY_RIGHT:
				velocity.x += 2.0f;
				break;
			}
		}
		else if (keyEvent != nullptr && keyEvent->status == IS_UP) {
			switch (keyEvent->key) {
			case KEY_LEFT:
				if (velocity.x < 0.0f) velocity.x += 2.0f;
				break;
			case KEY_RIGHT:
				if (velocity.x > 0.0f) velocity.x -= 2.0f;
				break;
			}
		}*/
		if (height > 0)
			verticalForce += 0.5f;
		else {
			jumping.speed = 0.0f;
			jumping.Reset();
			prevVelocity = velocity;
			velocity.x = 0.0f;
			setCurrentAnimation(&jumping);
			status = JUMP_END;
			jumpTimer.reset();
		}
		break;

	case JUMP_END:
		if (jumpTimer.getDelta() >= 1000) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walk);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}

		break;
	}

	if (App->input->keyEventQueue.empty() == false) {
		App->input->keyEventQueue.pop_front();
		RELEASE(keyEvent);
	}

	if (status != DEAD && status != JUMP_INI && status != JUMP_END)
		updatePosition();

	if (status != DEAD)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	// debug player
	char integer_string[32];
	LOG("******");
	LOG("Status: (0-DEFAULT, 1-RESPAWNING, 2-JUMPING, 3-DEAD)");
	sprintf_s(integer_string, "%d", status);
	LOG(integer_string);
	LOG("Depth:");
	sprintf_s(integer_string, "%d", depth);
	LOG(integer_string);
	LOG("VelX:");
	sprintf_s(integer_string, "%f", velocity.x);
	LOG(integer_string);
	LOG("PosX:");
	sprintf_s(integer_string, "%d", position.x);
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
	if (status == IDLE || status == WALK || status == JUMPING || status == ATTACK_JMP)
		position.x += (int)velocity.x;
	if (position.x < 0) 
		position.x = 0;
	if (!isOnTheAir() && status != JUMPING && status != ATTACKING)
		depth -= (int)velocity.y;
	if (status == WALK)
		position.y += (int)velocity.y;
	if (status == RESPAWNING || status == JUMPING || status == ATTACK_JMP) {
		height -= (int)verticalForce;
		if (height < 0) {
			height = 0;
			verticalForce = 0.0f;
		}
		position.y += (int)verticalForce;
	}
	if (depth > 53) {
		position.y = 102;
		depth = 53;
	}
	if (depth < 0 || position.y > 155) {
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
