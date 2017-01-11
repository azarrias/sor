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
#include "parson/parson.h"
#include "ModuleAudio.h"

Player::Player()
	: Creature(Entity::Types::PLAYER, { 23, -30 }, 20, RIGHT)
{}

Player::~Player()
{}

bool Player::LoadConfigFromJSON(const char* fileName)
{
	JSON_Value* root_value;
	JSON_Object* moduleObject;

	root_value = json_parse_file(fileName);
	if (root_value != nullptr)
		moduleObject = json_object(root_value);
	else return false;

	graphics = App->textures->Load(json_object_dotget_string(moduleObject, "Player.graphicsFile"));
	if (soundFxHit == 0)
		soundFxHit = App->audio->LoadFx(json_object_dotget_string(moduleObject, "Player.soundFxHit"));
	if (soundFxAttack == 0)
		soundFxAttack = App->audio->LoadFx(json_object_dotget_string(moduleObject, "Player.soundFxAttack"));
	if (soundFxDie == 0)
		soundFxDie = App->audio->LoadFx(json_object_dotget_string(moduleObject, "Player.soundFxDie"));
	
	if (LoadAnimationFromJSONObject(moduleObject, "Player.animations.idle", idle) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.respawning", respawning) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.walking", walking) == false || 
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.jumping", jumping) == false || 
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.jumpKick", jumpKick) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.attack", attack) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.attack2", attack2) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.beingHit", beingHit) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.beingHit2", beingHit2) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.beingHit3", beingHit3) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.knockedOut", knockedOut) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.dying", dying) == false ||
		LoadAnimationFromJSONObject(moduleObject, "Player.animations.gettingUp", gettingUp) == false)
			return false;

	json_value_free(root_value);
	
	return true;
}

bool Player::Start()
{
	LOG("Loading player");
	if (LoadConfigFromJSON(CONFIG_FILE) == false)
		return false;
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
	position.x = iniPos.x - App->camera->coord.x;
	position.y = iniPos.y - App->camera->coord.y;
	depth = 23;
	height = 162;
	status = RESPAWNING;
	facing = RIGHT;
	setCurrentAnimation(&respawning);
	creatureTimer.reset();
	hp = 20;
	--lives;
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
			}
			else if (keyEvent->status == IS_UP)
				velocity.x += 2.0f;
			break;
		case KEY_RIGHT:
			if (keyEvent->status == IS_DOWN) {
				velocity.x += 2.0f;
			}
			else if (keyEvent->status == IS_UP)
				velocity.x -= 2.0f;
			break;
		case KEY_S:
			if (keyEvent->status == IS_DOWN) {
				App->audio->PlayFx(soundFxAttack);
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
					setCurrentAnimation(&attack);
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

	// debug player
	char integer_string[32];
	LOG("******");
	LOG("Status:");
	sprintf_s(integer_string, "%d", status);
	LOG(integer_string);
	LOG("Depth:");
	sprintf_s(integer_string, "%d", depth);
	LOG(integer_string);
	LOG("Height:");
	sprintf_s(integer_string, "%d", height);
	LOG(integer_string);
	LOG("VelX:");
	sprintf_s(integer_string, "%f", velocity.x);
	LOG(integer_string);
	LOG("PosX:");
	sprintf_s(integer_string, "%d", position.x);
	LOG(integer_string);

	return UPDATE_CONTINUE;
}

void Player::hit(Creature* c2) {
	App->audio->PlayFx(soundFxHit);
	Creature::hit(c2);
}

void Player::die() {
	App->audio->PlayFx(soundFxDie);
	Creature::die();
}
// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
