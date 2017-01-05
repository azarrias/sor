#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include <stdio.h>
#include "MemLeaks.h"
#include "ModuleCamera.h"

Player::Player()
	: Creature(Entity::Types::PLAYER)
{
	facing = RIGHT;
	// Coordinates for Blaze

	// idle animation
	for (size_t i = 0; i < 8; ++i)
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
	walking.frames.push_back({ 306, 956, 36, 61 });
	walking.frames.push_back({ 344, 956, 41, 61 });
	walking.frames.push_back({ 386, 956, 36, 61 });
	walking.frames.push_back({ 424, 956, 41, 61 });
	walking.speed = 0.1f;

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

Player::~Player()
{}

// Load assets
bool Player::Start()
{
	LOG("Loading player");
	graphics = App->textures->Load("graphics/pcs.png");
	return Creature::Start();
}

// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");
	return Entity::CleanUp();
}

// Respawn player
void Player::spawn()
{
	LOG("Respawning player");
	position.x = 23;
	position.y = -30;
	height = 162;
	status = RESPAWNING;
	setCurrentAnimation(&respawning);
	creatureTimer.reset();
	--lives;
}

// Jump
void Player::jump()
{
	LOG("Player jumps");
	status = JUMPING;
	setCurrentAnimation(&jumping);
	jumpTimer.reset();
}

// Attack
void Player::attack()
{
	LOG("Player attacks");
	status = ATTACKING;
	if (height > 0) {
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

void Player::handleInput()
{
	KeyEvent* keyEvent = nullptr;

	if (App->input->keyEventQueue.empty() == false) {
		keyEvent = App->input->keyEventQueue.front();
	}

	if (keyEvent != nullptr) {
		switch (keyEvent->key)
		{
		case KEY_UP:
			if (keyEvent->status == IS_DOWN)
				velocity.y -= 1.0f;
			else if (keyEvent->status == IS_UP)
				velocity.y += 1.0f;
			break;
		case KEY_DOWN:
			if (keyEvent->status == IS_DOWN)
				velocity.y += 1.0f;
			else if (keyEvent->status == IS_UP)
				velocity.y -= 1.0f;
			break;
		case KEY_LEFT:
			if (keyEvent->status == IS_DOWN) {
				velocity.x -= 2.0f;
				facing = LEFT;
			}
			else if (keyEvent->status == IS_UP)
				velocity.x += 2.0f;
			break;
		case KEY_RIGHT:
			if (keyEvent->status == IS_DOWN) {
				velocity.x += 2.0f;
				facing = RIGHT;
			}
			else if (keyEvent->status == IS_UP)
				velocity.x -= 2.0f;
			break;
		case KEY_S:
			if (keyEvent->status == IS_DOWN) {
				if (status == JUMPING) {
					status = ATTACK_JMP;
					setCurrentAnimation(&jumpKick);
				}
				else if (status == IDLE || status == WALK) {
					prevVelocity = velocity;
					velocity.y = 0.0f;
					velocity.x = 0.0f;
					status = ATTACKING;
					attackTimer.reset();
					setCurrentAnimation(&chop);
				}
			}
			break;
		case KEY_D:
			if (keyEvent->status == IS_DOWN && (status == IDLE || status == WALK)) {
				jumpTimer.reset();
				status = JUMP_INI;
				setCurrentAnimation(&jumping);
			}
			break;
		}
	}

	if (App->input->keyEventQueue.empty() == false) {
		App->input->keyEventQueue.pop_front();
		RELEASE(keyEvent);
	}
}

// Update: draw background
update_status Player::Update()
{
	handleInput();
	Creature::Update();
	// Player should not be able to go back to the left
	if (position.x < -App->camera->coord.x)
		position.x = -App->camera->coord.x;
	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
