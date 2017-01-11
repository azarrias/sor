#include "NPC.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Player.h"
#include "ModuleCamera.h"
#include "ModuleAudio.h"
#include <random>

NPC::NPC(Entity::Types entityType, iPoint iniPos, short int hp, Direction facing)
	: Creature(entityType, iniPos, hp, facing)
{
	facing = LEFT;
}

NPC::~NPC()
{}

bool NPC::Start()
{
	if (NPC::LoadConfigFromJSON(CONFIG_FILE) == false)
		return false;
	return Creature::Start();
}

bool NPC::LoadConfigFromJSON(const char* fileName)
{
	JSON_Value* root_value;
	JSON_Object* moduleObject;

	root_value = json_parse_file(fileName);
	if (root_value != nullptr)
		moduleObject = json_object(root_value);
	else return false;

	if (soundFxNPCHit == 0)
		soundFxNPCHit = App->audio->LoadFx(json_object_dotget_string(moduleObject, "NPC.soundFxNPCHit"));
	if (soundFxNPCDie == 0)
		soundFxNPCDie = App->audio->LoadFx(json_object_dotget_string(moduleObject, "NPC.soundFxNPCDie"));

	json_value_free(root_value);

	return true;
}

// Update: draw background
update_status NPC::Update()
{
	behaviour();
	Creature::Update();
	return UPDATE_CONTINUE;
}

void NPC::hit(Creature* c2) {
	App->audio->PlayFx(soundFxNPCHit);
	Creature::hit(c2);
}

void NPC::die() {
	App->audio->PlayFx(soundFxNPCDie);
	Creature::die();
}

void NPC::behaviour() {
	// If the Player is in attack range then attack: attack or retreat
	// else if the Player is close be aggressive: attack, chase, retreat
	// else take it easy: patrol, wait
	switch (action) {
	case APPROACH:
		if (position.x > App->entities->player->position.x) velocity.x = -1.0f;
		else velocity.x = 1.0f;
		if (depth > App->entities->player->depth) --depth;
		else if (depth < App->entities->player->depth) ++depth;
		if (getDistanceToPlayer() < 50 && abs(depth - App->entities->player->depth) <= DEPTH_THRESHOLD + 2) {
			if (canBeAttacked()) {
				NPCTimer.reset();
				action = ATTACK;
			}
			else
			{
				NPCTimer.reset();
				action = WAIT;
			}
		}
		break;
	case WAIT:
		velocity = { 0.0f, 0.0f };
		if (status != UNAVAILABLE && NPCTimer.getDelta() > waitingTime) {
			action = APPROACH;
		}
		break;
	case ATTACK:
			velocity = { 0.0f, 0.0f };
			status = ATTACKING;
			setCurrentAnimation(&attack);
			attackTimer.reset();
			NPCTimer.reset();
			action = ATTACK_RECOVER;
		break;
	case ATTACK_RECOVER:
		// if the hit has landed continue to attack
		// else retreat
		if (NPCTimer.getDelta() > 300)
			if (getDistanceToPlayer() < 50 && abs(depth - App->entities->player->depth) <= DEPTH_THRESHOLD + 2) {
				if (canBeAttacked()) {
					NPCTimer.reset();
					action = ATTACK;
				}
				else
				{
					NPCTimer.reset();
					action = WAIT;
				}
			}
		break;
	}

}

void NPC::chooseNextAction() {
/*	if (getDistanceToPlayer() < 50) {
		if (rollDice(2) == 1) {
			action = ATTACK;
			attack();
		}
		else {
			action = RETREAT;
			retreat();
		}
	}
	else if (getDistanceToPlayer() < 150) {
	//	action = CHASE;
		chase();
	}
	else {
		if (rollDice(2) == 1) {
			action = PATROL;
			switch (rollDice(8)) {
			case 1:	velocity = { 0.0f, -1.0f };	break;
			case 2: velocity = { 1.0f, -1.0f }; break;
			case 3: velocity = { 1.0f, 0.0f }; break;
			case 4: velocity = { 1.0f, 1.0f }; break;
			case 5: velocity = { 0.0f, 1.0f }; break;
			case 6: velocity = { -1.0f, 1.0f }; break;
			case 7: velocity = { -1.0f, 0.0f }; break;
			case 8: velocity = { -1.0f, -1.0f }; break;
			}
		}
		else {
			action = WAIT;
			velocity = { 0.0f, 0.0f };
			waitingTime = 500 + 100 * rollDice(5);
		}
	}
	NPCTimer.reset();*/
}

float NPC::getDistanceToPlayer() const {
	short int depthDist = this->depth - App->entities->player->depth;
	short int xDist = this->position.x - App->entities->player->position.x;
	return (float)(sqrt(pow(depthDist, 2) + pow(xDist, 2)));
}

const unsigned short int NPC::rollDice(unsigned short int nrDiceFaces) const {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, nrDiceFaces);
	return dis(gen);
}

void NPC::retreat() {
	if (getDistanceToPlayer() < 100)
		getAwayFromPlayer();
}

void NPC::getAwayFromPlayer() {
	if (App->entities->player->position.x + App->camera->coord.x > (int)(SCREEN_WIDTH / 2))
		this->velocity.x = -2.0f;
	else
		this->velocity.x = 2.0f;
	if (App->entities->player->depth > (int)(53 / 2))
		this->velocity.y = 1.0f;
	else this->velocity.y = -1.0f;
}

void NPC::chase() {
	if (App->entities->player->position.x + App->camera->coord.x > (int)(SCREEN_WIDTH / 2))
		this->velocity.x = +2.0f;
	else
		this->velocity.x = -2.0f;
	if (App->entities->player->depth > (int)(53 / 2))
		this->velocity.y = -1.0f;
	else this->velocity.y = +1.0f;
}

/*void NPC::doAttack() {
	status = ATTACKING;
	setCurrentAnimation(&chop);
}*/